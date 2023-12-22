#pragma once

#include <JuceHeader.h>

#include <StftPitchShiftPlugin/Parameters/GenericParameterContainer.h>

#include <bit>

class Parameters final : public GenericParameterContainer
{

public:

  Parameters(juce::AudioProcessor& process);
  ~Parameters();

  void onbypass(std::function<void()> callback);
  void onnormalize(std::function<void()> callback);
  void onquefrency(std::function<void()> callback);
  void ontimbre(std::function<void()> callback);
  void onpitch(std::function<void()> callback);
  void onreset(std::function<void()> callback);

  bool bypass() const;
  bool normalize() const;
  double quefrency() const;
  double timbre() const;
  std::vector<double> pitch() const;
  int dftsize(const int blocksize) const;
  int overlap(const int blocksize) const;
  bool lowlatency() const;

  void load(const void* data, const int size);
  void save(juce::MemoryBlock& data);

private:

  const int schema = 1;
  const int maxstages = 5;

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
    jassert(y <= static_cast<unsigned int>(std::numeric_limits<int>::max()));
    return static_cast<int>(y);
  }

};
