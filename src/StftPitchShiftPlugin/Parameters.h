#pragma once

#include <JuceHeader.h>

class Parameters final
{

public:

  explicit Parameters(juce::AudioProcessor& process);
  ~Parameters();

  void read(const void* data, const int size);
  void write(juce::MemoryBlock& data);

private:

  const int schema = 1;

  juce::AudioProcessor& process;

  struct
  {
    juce::AudioParameterBool*  bypass;
    juce::AudioParameterBool*  normalize;
    juce::AudioParameterFloat* quefrency;
    juce::AudioParameterInt*   timbre;
    std::array<juce::AudioParameterInt*, 5> pitch;
  }
  params;

};
