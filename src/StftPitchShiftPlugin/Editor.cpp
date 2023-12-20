#include <StftPitchShiftPlugin/Editor.h>

#include <StftPitchShiftPlugin/Logger.h>

Editor::Editor(juce::AudioProcessor& process) :
  juce::AudioProcessorEditor(process),
  process(process)
{
  jive::Interpreter interpreter;

  view = interpreter.interpret(juce::ValueTree
  {
    "Button",
    {
      { "width", 200 },
      { "height", 100 },
    }
  });

  addAndMakeVisible(*view->getComponent());

  // setSize(
  //   view->getComponent()->getWidth(),
  //   view->getComponent()->getHeight());

  const int w = 500;
  const int h = 500;

  setSize(w, h);
}

Editor::~Editor()
{
  juce::ignoreUnused(process);
}
