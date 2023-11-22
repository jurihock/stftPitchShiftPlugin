#pragma once

#include <JuceHeader.h>

#include <StftPitchShiftPlugin/GenericParameterContainer.h>

class Parameters final
{

public:

  explicit Parameters(juce::AudioProcessor& process);
  ~Parameters();

  void onbypass(std::function<void()> callback);
  void onnormalize(std::function<void()> callback);
  void onquefrency(std::function<void()> callback);
  void ontimbre(std::function<void()> callback);
  void onpitch(std::function<void()> callback);
  void onreset(std::function<void()> callback);

  juce::AudioProcessorParameter* raw(const std::string& id) const;

  bool bypass() const;
  bool normalize() const;
  double quefrency() const;
  double timbre() const;
  std::vector<double> pitch() const;
  int dftsize(const int blocksize) const;
  int overlap(const int blocksize) const;

  void read(const void* data, const int size);
  void write(juce::MemoryBlock& data);

private:

  const int schema = 1;
  const int maxstages = 5;

  juce::AudioProcessor& process;

  GenericParameterContainer parameters;

};
