#pragma once

#include <JuceHeader.h>

#include <StftPitchShiftPlugin/Core.h>
#include <StftPitchShiftPlugin/Logger.h>

class Processor final : public juce::AudioProcessor
{

public:

  Processor();
  ~Processor() override;

  const juce::String getName() const override;

  bool hasEditor() const override;
  juce::AudioProcessorEditor* createEditor() override;

  bool isMidiEffect() const override;
  bool acceptsMidi() const override;
  bool producesMidi() const override;
  int  getNumPrograms() override;
  int  getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  void changeProgramName(int index, const juce::String& name) override;
  const juce::String getProgramName (int index) override;

  double getTailLengthSeconds() const override;
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

  void prepareToPlay(double samplerate, int blocksize) override;
  void releaseResources() override;

  void processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi) override;

  void getStateInformation(juce::MemoryBlock& data) override;
  void setStateInformation(const void* data, int size) override;

private:

  struct
  {
    bool bypass {false};

    int dftsize {1024};
    int overlap {4};

    std::optional<double> samplerate {std::nullopt};
    std::optional<int>    blocksize  {std::nullopt};
  }
  state;

  std::unique_ptr<Core> core;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Processor)

};
