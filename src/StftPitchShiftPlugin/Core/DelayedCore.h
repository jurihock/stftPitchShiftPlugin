#pragma once

#include <StftPitchShiftPlugin/Core/InstantCore.h>

class DelayedCore : public InstantCore
{

public:

  DelayedCore(const double samplerate, const int blocksize, const int dftsize, const int overlap);
  ~DelayedCore();

  int latency() const override;
  bool compatible(const int blocksize) const override;
  void process(const std::span<const float> input, const std::span<float> output) override;

private:

  struct { std::vector<float> input, output; } buffer;

  size_t samples;

};
