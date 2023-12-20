#pragma once

#include <JuceHeader.h>

class Editor final : public juce::AudioProcessorEditor
{

public:

  explicit Editor(juce::AudioProcessor& process);
  ~Editor() override;

private:

  juce::AudioProcessor& process;

  std::unique_ptr<jive::GuiItem> view;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor)

};
