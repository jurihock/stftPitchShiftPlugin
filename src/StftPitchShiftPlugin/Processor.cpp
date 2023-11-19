#include <StftPitchShiftPlugin/Processor.h>

#include <StftPitchShiftPlugin/Logger.h>

Processor::Processor() :
  AudioProcessor(
    BusesProperties()
      .withInput("Input",   juce::AudioChannelSet::mono(),   true)
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
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions,
  // will only load plugins that support stereo bus layouts.

  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  // This checks if the input layout matches the output layout.

  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
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

  state.samplerate = std::nullopt;
  state.blocksize  = std::nullopt;

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

  state.samplerate = samplerate;
  state.blocksize  = blocksize;

  try
  {
    core = std::make_unique<Core>(
      state.samplerate.value(),
      state.blocksize.value(),
      state.dftsize,
      state.overlap);

    core->normalize(parameters->normalize());
    core->quefrency(parameters->quefrency());
    core->timbre(parameters->timbre());
    core->pitch(parameters->pitch());
  }
  catch(const std::exception& exception)
  {
    LOG(exception.what());
  }
}

void Processor::releaseResources()
{
  std::lock_guard lock(mutex);

  LOG("Release resources");

  state.samplerate = std::nullopt;
  state.blocksize  = std::nullopt;

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

  auto copy_input_to_output = [&](const std::string& reason = "")
  {
    if (!reason.empty())
    {
      LOG("Copy input to output (%s)", reason.c_str());
    }

    for (int channel = 1; channel < output_channels; ++channel)
    {
      audio.copyFrom(channel, 0, audio, 0, 0, channel_samples);
    }
  };

  // This is the place where you'd normally do the guts of your plugin's audio processing.
  // Make sure to reset the state if your inner loop is processing the samples
  // and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.

  if (parameters->bypass())
  {
    copy_input_to_output();
  }
  else if (!core)
  {
    copy_input_to_output("core is uninitialized");
  }
  else if (!core->compatible(channel_samples))
  {
    copy_input_to_output("core is incompatible");
  }
  else
  {
    try
    {
      auto input = std::span<float>(
        const_cast<float*>(audio.getReadPointer(0)),
        static_cast<size_t>(channel_samples));

      auto output = std::span<float>(
        audio.getWritePointer(0),
        static_cast<size_t>(channel_samples));

      core->process(input, output);

      copy_input_to_output();
    }
    catch(const std::exception& exception)
    {
      copy_input_to_output(exception.what());
    }
  }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new Processor(); }
