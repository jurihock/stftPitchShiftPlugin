#pragma once

#include <JuceHeader.h>

class Editor final : public juce::GenericAudioProcessorEditor
{

public:

  explicit Editor(juce::AudioProcessor& process);
  ~Editor() override;

private:

  juce::AudioProcessor& process;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor)

};
