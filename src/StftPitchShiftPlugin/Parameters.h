#pragma once

#include <JuceHeader.h>

class Parameters final
{

public:

  explicit Parameters(juce::AudioProcessor& process);
  ~Parameters();

  void write(juce::MemoryBlock& data);
  void read(const void* data, const int size);

private:

  const int schema = 1;

  juce::AudioProcessor& process;

  juce::AudioParameterBool*  bypass;
  juce::AudioParameterFloat* quefrency;
  juce::AudioParameterInt*   timbre;

};
