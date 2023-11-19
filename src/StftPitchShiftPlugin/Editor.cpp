#include <StftPitchShiftPlugin/Editor.h>

#include <StftPitchShiftPlugin/Logger.h>

Editor::Editor(juce::AudioProcessor& process) :
  GenericAudioProcessorEditor(process),
  process(process)
{
}

Editor::~Editor()
{
  juce::ignoreUnused(process);
}
