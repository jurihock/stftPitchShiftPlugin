#include <StftPitchShiftPlugin/Editor.h>

Editor::Editor(Processor& process) :
  AudioProcessorEditor(process),
  process(process)
{
  setSize(400, 300);
}

Editor::~Editor()
{
  juce::ignoreUnused(process);
}

void Editor::paint(juce::Graphics& graphics)
{
  graphics.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  graphics.setColour(juce::Colours::white);
  graphics.setFont(15.0f);
  graphics.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void Editor::resized()
{
}
