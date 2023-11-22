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

  GenericParameterContainer parameters;

  static int prev_power_of_two(const int x)
  {
    jassert(x >= 0);
    const auto y = std::bit_floor(static_cast<unsigned int>(x));
    return static_cast<int>(y);
  }

  static int next_power_of_two(const int x)
  {
    jassert(x >= 0);
    const auto y = std::bit_ceil(static_cast<unsigned int>(x));
    jassert(y <= std::numeric_limits<int>::max());
    return static_cast<int>(y);
  }

};
