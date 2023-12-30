#include <StftPitchShiftPlugin/Plugin.h>

#include <StftPitchShiftPlugin/Processor.h>

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  return new Processor();
}
