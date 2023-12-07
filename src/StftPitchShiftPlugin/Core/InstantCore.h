#pragma once

#include <StftPitchShiftPlugin/Core.h>

class InstantCore : public Core
{

public:

  InstantCore(const double samplerate, const int blocksize, const int dftsize, const int overlap);
  ~InstantCore();

  bool compatible(const int blocksize) const override;
  void process(const std::span<const float> input, const std::span<float> output) override;

private:

  struct { std::vector<double> input, output; } buffer;

  template<typename X, typename Y>
  static Y transform(const X x) { return static_cast<Y>(x); }

};
