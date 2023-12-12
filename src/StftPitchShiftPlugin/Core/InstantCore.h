#pragma once

#include <StftPitchShiftPlugin/Core.h>

class InstantCore : public Core
{

public:

  InstantCore(const double samplerate, const int blocksize, const int dftsize, const int overlap);
  ~InstantCore();

  int latency() const override;
  bool compatible(const int blocksize) const override;

  void dry(const std::span<const float> input, const std::span<float> output) override;
  void wet(const std::span<const float> input, const std::span<float> output) override;

private:

  struct { std::vector<double> input, output; } buffer;

  template<typename X, typename Y>
  static Y transform(const X x) { return static_cast<Y>(x); }

  void process(const std::span<const float> input, const std::span<float> output,
               std::function<void(std::span<double> x, std::span<double> y)> callback);

};
