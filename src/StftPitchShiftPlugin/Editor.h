#pragma once

#include <JuceHeader.h>

#include <StftPitchShiftPlugin/Processor.h>

class Editor final : public juce::AudioProcessorEditor
{

public:

  explicit Editor(Processor& process);
  ~Editor() override;

  void paint(juce::Graphics& graphics) override;
  void resized() override;

private:

  Processor& process;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor)

};
