#include <StftPitchShiftPlugin/Processor.h>

#include <StftPitchShiftPlugin/Effect/InstantEffect.h>
#include <StftPitchShiftPlugin/Effect/DelayedEffect.h>

#include <StftPitchShiftPlugin/Logger.h>

Processor::Processor() :
  juce::AudioProcessor(
    juce::AudioProcessor::BusesProperties()
      .withInput("Input",   juce::AudioChannelSet::stereo(), true)
      .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
  parameters = std::make_shared<Parameters>(*this);

  parameters->onnormalize([&]()
  {
    std::lock_guard lock(mutex);
    if (effect) { effect->normalize(parameters->normalize()); }
  });

  parameters->onquefrency([&]()
  {
    std::lock_guard lock(mutex);
    if (effect) { effect->quefrency(parameters->quefrency()); }
  });

  parameters->ontimbre([&]()
  {
    std::lock_guard lock(mutex);
    if (effect) { effect->timbre(parameters->timbre()); }
  });

  parameters->onpitch([&]()
  {
    std::lock_guard lock(mutex);
    if (effect) { effect->pitch(parameters->pitch()); }
  });

  parameters->onreset([&]()
  {
    std::lock_guard lock(mutex);
    if (state) { resetEffect(state.value()); }
  });
}

Processor::~Processor()
{
}

const juce::String Processor::getName() const
{
  return juce::String::formatted(
    "%s v%s",
    ProjectInfo::projectName,
    ProjectInfo::versionString);
}

bool Processor::hasEditor() const { return true; }
juce::AudioProcessorEditor* Processor::createEditor() { return new Editor(*this, parameters); }

bool Processor::isMidiEffect() const { return false; }
bool Processor::acceptsMidi() const { return false; }
bool Processor::producesMidi() const { return false; }
int  Processor::getNumPrograms() { return 1; }
int  Processor::getCurrentProgram() { return 0; }
void Processor::setCurrentProgram(int index) { juce::ignoreUnused(index); }
void Processor::changeProgramName(int index, const juce::String& name) { juce::ignoreUnused(index, name); }
const juce::String Processor::getProgramName(int index) { juce::ignoreUnused(index); return {}; }

double Processor::getTailLengthSeconds() const { return 0; }
juce::AudioProcessorParameter* Processor::getBypassParameter() const { return parameters->get("bypass"); }

bool Processor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  return true;
}

void Processor::getStateInformation(juce::MemoryBlock& data)
{
  LOG("Save plugin state");

  parameters->save(data);
}

void Processor::setStateInformation(const void* data, int size)
{
  LOG("Load plugin state");

  parameters->load(data, size);
}

void Processor::prepareToPlay(double samplerate, int blocksize)
{
  std::lock_guard lock(mutex);

  state = std::nullopt;
  effect = nullptr;

  if (samplerate < 1)
  {
    LOG("Prepare to play (invalid samplerate %g)", samplerate);
    return;
  }

  if (blocksize < 1)
  {
    LOG("Prepare to play (invalid blocksize %d)", blocksize);
    return;
  }

  LOG("Prepare to play (samplerate %g, blocksize %d)", samplerate, blocksize);

  state = { samplerate, { blocksize, blocksize } };

  try
  {
    resetEffect(state.value());
  }
  catch(const std::exception& exception)
  {
    juce::ignoreUnused(exception);

    LOG(exception.what());
  }
}

void Processor::releaseResources()
{
  std::lock_guard lock(mutex);

  LOG("Release resources");

  state = std::nullopt;
  effect = nullptr;
}

void Processor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  juce::ignoreUnused(midi);

  juce::ScopedNoDenormals dontcare;

  std::lock_guard lock(mutex);

  const int input_channels  = getTotalNumInputChannels();
  const int output_channels = getTotalNumOutputChannels();
  const int channel_samples = audio.getNumSamples();

  if (input_channels < 1)
  {
    LOG("Skip block (invalid number of input channels %d)", input_channels);
    return;
  }

  if (output_channels < 1)
  {
    LOG("Skip block (invalid number of output channels %d)", output_channels);
    return;
  }

  if (channel_samples < 1)
  {
    LOG("Skip block (invalid number of samples per block %d)", channel_samples);
    return;
  }

  const auto process_mono_input = [&]()
  {
    auto input = std::span<const float>(
      audio.getReadPointer(0),
      static_cast<size_t>(channel_samples));

    auto output = std::span<float>(
      audio.getWritePointer(0),
      static_cast<size_t>(channel_samples));

    if (parameters->bypass())
    {
      effect->dry(input, output);
    }
    else
    {
      effect->wet(input, output);
    }
  };

  const auto process_stereo_output = [&](const std::string& error = "")
  {
    if (!error.empty())
    {
      LOG("Copy input to output (%s)", error.c_str());
    }

    for (int channel = 1; channel < output_channels; ++channel)
    {
      audio.copyFrom(channel, 0, audio, 0, 0, channel_samples);
    }
  };

  TIC();

  if (!state)
  {
    process_stereo_output("state is not initialized");
  }
  else if (!effect)
  {
    process_stereo_output("effect is not initialized");
  }
  else if (!effect->compatible(channel_samples))
  {
    State oldstate = state.value();
    State newstate = oldstate;

    newstate.blocksize.min = channel_samples;

    LOG("Change blocksize from %d to %d", oldstate.blocksize.min, newstate.blocksize.min);

    try
    {
      resetEffect(newstate);

      state = newstate;

      process_mono_input();
      process_stereo_output();
    }
    catch(const std::exception& exception)
    {
      process_stereo_output(exception.what());
    }
  }
  else
  {
    try
    {
      process_mono_input();
      process_stereo_output();
    }
    catch(const std::exception& exception)
    {
      process_stereo_output(exception.what());
    }
  }

  TOC();

  if (LAP())
  {
    const double samplerate = state.value_or(nostate).samplerate;
    const int blocksize  = state.value_or(nostate).blocksize.min;

    juce::ignoreUnused(samplerate, blocksize);

    LOG(CHRONOMETRY(samplerate, blocksize));
  }
}

void Processor::resetEffect(const State& state)
{
  const bool lowlatency = parameters->lowlatency();

  const double samplerate = state.samplerate;
  const int blocksize = lowlatency ? state.blocksize.min : state.blocksize.max;
  const int dftsize = parameters->dftsize(blocksize);
  const int overlap = parameters->overlap(blocksize);

  LOG("Reset effect (dftsize %d, overlap %d)", dftsize, overlap);

  if (lowlatency)
  {
    effect = std::make_unique<InstantEffect>(
      samplerate,
      blocksize,
      dftsize,
      overlap);
  }
  else
  {
    effect = std::make_unique<DelayedEffect>(
      samplerate,
      blocksize,
      dftsize,
      overlap);
  }

  effect->normalize(parameters->normalize());
  effect->quefrency(parameters->quefrency());
  effect->timbre(parameters->timbre());
  effect->pitch(parameters->pitch());

  const int latency = effect->latency();

  LOG("Latency %d (%d ms)", latency, static_cast<int>(1e+3 * latency / samplerate));

  setLatencySamples(latency);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new Processor(); }
