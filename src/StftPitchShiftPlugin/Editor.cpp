#include <StftPitchShiftPlugin/Editor.h>

#include <StftPitchShiftPlugin/Logger.h>

Editor::Editor(juce::AudioProcessor& process) :
  GenericAudioProcessorEditor(process),
  process(process)
{
  setSize(400, 300);
}

Editor::~Editor()
{
  juce::ignoreUnused(process);
}
