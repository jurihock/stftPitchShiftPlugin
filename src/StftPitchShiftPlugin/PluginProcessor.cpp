#include <StftPitchShiftPlugin/PluginProcessor.h>

#include <StftPitchShiftPlugin/PluginEditor.h>

PluginProcessor::PluginProcessor() :
  AudioProcessor(
    BusesProperties()
      .withInput("Input",   juce::AudioChannelSet::mono(), true)
      .withOutput("Output", juce::AudioChannelSet::mono(), true))
{
}

PluginProcessor::~PluginProcessor()
{
}

const juce::String PluginProcessor::getName() const
{
  return juce::String("stftPitchShift Plugin v") +
         juce::String(ProjectInfo::versionString);
}

bool PluginProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* PluginProcessor::createEditor() { return new PluginEditor(*this); }

bool PluginProcessor::isMidiEffect() const { return false; }
bool PluginProcessor::acceptsMidi() const { return false; }
bool PluginProcessor::producesMidi() const { return false; }
int  PluginProcessor::getNumPrograms() { return 1; }
int  PluginProcessor::getCurrentProgram() { return 0; }
void PluginProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }
void PluginProcessor::changeProgramName(int index, const juce::String& name) { juce::ignoreUnused(index, name); }
const juce::String PluginProcessor::getProgramName(int index) { juce::ignoreUnused(index); return {}; }

double PluginProcessor::getTailLengthSeconds() const { return 0; }

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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

void PluginProcessor::prepareToPlay(double samplerate, int framesize)
{
  // Use this method as the place to do any pre-playback initialisation that you need.

  juce::ignoreUnused(samplerate, framesize);
}

void PluginProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any spare memory, etc.
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
{
  juce::ScopedNoDenormals dontcare;

  juce::ignoreUnused(midi);

  auto input_channels  = getTotalNumInputChannels();
  auto output_channels = getTotalNumOutputChannels();

  // In case we have more outputs than inputs,
  // this code clears any output channels that didn't contain input data,
  // (because these aren't guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback when they first compile a plugin,
  // but obviously you don't need to keep this code if your algorithm always overwrites all the output channels.
  for (auto i = input_channels; i < output_channels; ++i)
  {
    audio.clear(i, 0, audio.getNumSamples());
  }

  // This is the place where you'd normally do the guts of your plugin's audio processing.
  // Make sure to reset the state if your inner loop is processing the samples
  // and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.
  for (int channel = 0; channel < input_channels; ++channel)
  {
    auto* channelData = audio.getWritePointer(channel);

    // Do something to the data...
    juce::ignoreUnused(channelData);
  }
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& data)
{
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.

  juce::ignoreUnused(data);
}

void PluginProcessor::setStateInformation(const void* data, int size)
{
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.

  juce::ignoreUnused(data, size);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new PluginProcessor(); }
