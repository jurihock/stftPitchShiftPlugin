#include <StftPitchShiftPlugin/Processor.h>

#include <StftPitchShiftPlugin/Logger.h>

Processor::Processor() :
  AudioProcessor(
    BusesProperties()
      .withInput("Input",   juce::AudioChannelSet::stereo(), true)
      .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
  parameters = std::make_unique<Parameters>(*this);

  parameters->onnormalize([&]()
  {
    std::lock_guard lock(mutex);
    if (core) { core->normalize(parameters->normalize()); }
  });

  parameters->onquefrency([&]()
  {
    std::lock_guard lock(mutex);
    if (core) { core->quefrency(parameters->quefrency()); }
  });

  parameters->ontimbre([&]()
  {
    std::lock_guard lock(mutex);
    if (core) { core->timbre(parameters->timbre()); }
  });

  parameters->onpitch([&]()
  {
    std::lock_guard lock(mutex);
    if (core) { core->pitch(parameters->pitch()); }
  });

  parameters->onreset([&]()
  {
    std::lock_guard lock(mutex);
    if (state) { resetCore(state.value().samplerate, state.value().blocksize); }
  });
}

Processor::~Processor()
{
}

const juce::String Processor::getName() const
{
  return juce::String("stftPitchShift Plugin v") +
         juce::String(ProjectInfo::versionString);
}

bool Processor::hasEditor() const { return true; }
juce::AudioProcessorEditor* Processor::createEditor() { return new Editor(*this); }

bool Processor::isMidiEffect() const { return false; }
bool Processor::acceptsMidi() const { return false; }
bool Processor::producesMidi() const { return false; }
int  Processor::getNumPrograms() { return 1; }
int  Processor::getCurrentProgram() { return 0; }
void Processor::setCurrentProgram(int index) { juce::ignoreUnused(index); }
void Processor::changeProgramName(int index, const juce::String& name) { juce::ignoreUnused(index, name); }
const juce::String Processor::getProgramName(int index) { juce::ignoreUnused(index); return {}; }

double Processor::getTailLengthSeconds() const { return 0; }
juce::AudioProcessorParameter* Processor::getBypassParameter() const { return parameters->raw("bypass"); }

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
  LOG("Write plugin state");

  parameters->write(data);
}

void Processor::setStateInformation(const void* data, int size)
{
  LOG("Read plugin state");

  parameters->read(data, size);
}

void Processor::prepareToPlay(double samplerate, int blocksize)
{
  std::lock_guard lock(mutex);

  state = std::nullopt;
  core = nullptr;

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

  state = { samplerate, blocksize };

  try
  {
    resetCore(samplerate, blocksize);
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
  core = nullptr;
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
    auto input = std::span<float>(
      const_cast<float*>(audio.getReadPointer(0)),
      static_cast<size_t>(channel_samples));

    auto output = std::span<float>(
      audio.getWritePointer(0),
      static_cast<size_t>(channel_samples));

    core->process(input, output);
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

  if (parameters->bypass() || (channel_samples < 4))
  {
    process_stereo_output();
  }
  else if (!state)
  {
    process_stereo_output("state is not initialized");
  }
  else if (!core)
  {
    process_stereo_output("core is not initialized");
  }
  else if (!core->compatible(channel_samples))
  {
    const double samplerate = state.value().samplerate;

    const int blocksize0 = state.value().blocksize;
    const int blocksize1 = channel_samples;

    juce::ignoreUnused(blocksize0, blocksize1);

    LOG("Change block size from %d to %d", blocksize0, blocksize1);

    try
    {
      resetCore(samplerate, blocksize1);

      state = { samplerate, blocksize1 };

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
    const int blocksize  = state.value_or(nostate).blocksize;

    juce::ignoreUnused(samplerate, blocksize);

    LOG(CHRONOMETRY(samplerate, blocksize));
  }
}

void Processor::resetCore(const double samplerate, const int blocksize)
{
  const int dftsize = parameters->dftsize(blocksize);
  const int overlap = parameters->overlap(blocksize);

  LOG("Reset core (dftsize %d, overlap %d)", dftsize, overlap);

  core = std::make_unique<Core>(
    samplerate,
    blocksize,
    dftsize,
    overlap);

  core->normalize(parameters->normalize());
  core->quefrency(parameters->quefrency());
  core->timbre(parameters->timbre());
  core->pitch(parameters->pitch());
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new Processor(); }
