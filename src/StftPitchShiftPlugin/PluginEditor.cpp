#include <StftPitchShiftPlugin/PluginEditor.h>

PluginEditor::PluginEditor(PluginProcessor& theprocess) :
  AudioProcessorEditor(theprocess),
  process(theprocess)
{
  juce::ignoreUnused(process);

  setSize(400, 300);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint(juce::Graphics& graphics)
{
  graphics.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  graphics.setColour(juce::Colours::white);
  graphics.setFont(15.0f);
  graphics.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void PluginEditor::resized()
{
}
