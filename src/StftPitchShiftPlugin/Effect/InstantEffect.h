#pragma once

#include <StftPitchShiftPlugin/Effect.h>

class InstantEffect : public Effect
{

public:

  InstantEffect(const double samplerate, const int blocksize, const int dftsize, const int overlap);
  ~InstantEffect();

  int latency() const override;
  bool compatible(const int blocksize) const override;

  void dry(const std::span<const float> input, const std::span<float> output) override;
  void wet(const std::span<const float> input, const std::span<float> output) override;

private:

  struct { std::vector<double> input, output; } buffer;

  void process(const std::span<const float> input, const std::span<float> output,
               std::function<void(std::span<double> x, std::span<double> y)> callback);

  template<typename X, typename Y>
  static Y transform(const X x) { return static_cast<Y>(x); }

};
