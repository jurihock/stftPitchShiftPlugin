#include <StftPitchShiftPlugin/Editor.h>

#include <StftPitchShiftPlugin/Logger.h>

Editor::Editor(juce::AudioProcessor& process) :
  GenericAudioProcessorEditor(process),
  process(process)
{
  setSize(500, 500);
}

Editor::~Editor()
{
  juce::ignoreUnused(process);
}
