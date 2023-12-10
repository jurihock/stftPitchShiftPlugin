#include <StftPitchShiftPlugin/Editor.h>

#include <StftPitchShiftPlugin/Logger.h>

Editor::Editor(juce::AudioProcessor& process) :
  GenericAudioProcessorEditor(process),
  process(process)
{
  const int w = 550;
  const int h = 550;

  setSize(w, h);
}

Editor::~Editor()
{
  juce::ignoreUnused(process);
}
