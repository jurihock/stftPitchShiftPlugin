#pragma once

#include <StftPitchShiftPlugin/Effect/InstantEffect.h>

class DelayedEffect : public InstantEffect
{

public:

  DelayedEffect(const double samplerate, const int blocksize, const int dftsize, const int overlap);
  ~DelayedEffect();

  int latency() const override;
  bool compatible(const int blocksize) const override;

  void dry(const std::span<const float> input, const std::span<float> output) override;
  void wet(const std::span<const float> input, const std::span<float> output) override;

private:

  const int host_block_size;

  struct { std::vector<float> input, output; } buffer;

  size_t samples;

  void process(const std::span<const float> input, const std::span<float> output,
               std::function<void(std::span<float> x, std::span<float> y)> callback);

};
