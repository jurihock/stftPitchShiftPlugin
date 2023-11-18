#pragma once

#include <StftPitchShiftPlugin/PluginProcessor.h>

class PluginEditor final : public juce::AudioProcessorEditor
{

public:

  explicit PluginEditor(PluginProcessor& process);
  ~PluginEditor() override;

  void paint(juce::Graphics& graphics) override;
  void resized() override;

private:

  PluginProcessor& process;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)

};
