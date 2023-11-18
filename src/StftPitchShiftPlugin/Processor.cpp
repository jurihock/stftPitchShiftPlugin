#include <StftPitchShiftPlugin/Processor.h>

#include <StftPitchShiftPlugin/Editor.h>

Processor::Processor() :
  AudioProcessor(
    BusesProperties()
      .withInput("Input",   juce::AudioChannelSet::mono(), true)
      .withOutput("Output", juce::AudioChannelSet::mono(), true))
{
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

void Processor::prepareToPlay(double samplerate, int framesize)
{
  // Use this method as the place to do any pre-playback initialisation that you need.

  juce::ignoreUnused(samplerate, framesize);

  LOG("Prepare to play (samplerate %g, framesize %d)", samplerate, framesize);

  try
  {
    core = std::make_unique<Core>(samplerate, framesize);
  }
  catch(const std::exception& exception)
  {
    core = nullptr;

    LOG(exception.what());
  }
}

void Processor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any spare memory, etc.

  LOG("Release resources");

  core = nullptr;
}

void Processor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  juce::ScopedNoDenormals dontcare;

  juce::ignoreUnused(midi);

  const int input_channels  = getTotalNumInputChannels();
  const int output_channels = getTotalNumOutputChannels();
  const int channel_samples = audio.getNumSamples();

  if (input_channels < 1)
  {
    LOG("Skip block (invalid number of input channels)");
    return;
  }

  if (output_channels < 1)
  {
    LOG("Skip block (invalid number of output channels)");
    return;
  }

  if (channel_samples < 1)
  {
    LOG("Skip block (invalid number of samples)");
    return;
  }

  // In case we have more outputs than inputs,
  // clear any output channels that didn't contain input data,
  // because these aren't guaranteed to be empty and may contain garbage.

  for (int i = input_channels; i < output_channels; ++i)
  {
    audio.clear(i, 0, channel_samples);
  }

  // This is the place where you'd normally do the guts of your plugin's audio processing.
  // Make sure to reset the state if your inner loop is processing the samples
  // and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.

  bool bypass = !core || !core->compatible(channel_samples);

  if (!bypass)
  {
    for (int channel = 0; channel < std::min(input_channels, output_channels); ++channel)
    {
      const auto samples = static_cast<size_t>(channel_samples);

      auto input  = std::span<float>(const_cast<float*>(audio.getReadPointer(channel)), samples);
      auto output = std::span<float>(audio.getWritePointer(channel), samples);

      try
      {
        core->process(input, output);
      }
      catch(const std::exception& exception)
      {
        bypass = true;

        LOG(exception.what());
      }
    }
  }

  if (bypass)
  {
    LOG("Bypass block");

    for (int channel = 0; channel < std::min(input_channels, output_channels); ++channel)
    {
      const auto samples = static_cast<size_t>(channel_samples);

      auto input  = std::span<const float>(audio.getReadPointer(channel), samples);
      auto output = std::span<float>(audio.getWritePointer(channel), samples);

      for (size_t i = 0; i < samples; ++i)
      {
        output[i] = input[i];
      }
    }
  }
}

void Processor::getStateInformation(juce::MemoryBlock& data)
{
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.

  juce::ignoreUnused(data);
}

void Processor::setStateInformation(const void* data, int size)
{
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.

  juce::ignoreUnused(data, size);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new Processor(); }
