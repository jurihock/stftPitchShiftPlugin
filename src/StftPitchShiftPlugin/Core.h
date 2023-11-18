#pragma once

#include <StftPitchShift/STFT.h>
#include <StftPitchShift/StftPitchShiftCore.h>

using namespace stftpitchshift;

class Core final
{

public:

  Core(const double samplerate, const int framesize, const int dftsize = 1024, const int overlap = 4);
  ~Core();

  bool compatible(const int framesize) const;

  void process(const std::span<float> input, const std::span<float> output);

private:

  struct { size_t analysis_window_size, synthesis_window_size; } config;
  struct { std::vector<double> input, output; } buffer;

  std::unique_ptr<STFT<double>> stft;
  std::unique_ptr<StftPitchShiftCore<double>> core;

};
