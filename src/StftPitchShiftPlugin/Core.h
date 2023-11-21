#pragma once

#include <StftPitchShiftPlugin/FFT.h>

#include <StftPitchShift/STFT.h>
#include <StftPitchShift/StftPitchShiftCore.h>

class Core final
{

public:

  Core(const double samplerate, const int blocksize, const int dftsize, const int overlap);
  ~Core();

  void normalize(bool value);
  void quefrency(double value);
  void timbre(double value);
  void pitch(std::vector<double> values);

  bool compatible(const int blocksize) const;

  void process(const std::span<float> input, const std::span<float> output);

private:

  struct { size_t analysis_window_size, synthesis_window_size; } config;
  struct { std::vector<double> input, output; } buffer;

  std::unique_ptr<stftpitchshift::STFT<double>> stft;
  std::unique_ptr<stftpitchshift::StftPitchShiftCore<double>> core;

};
