#pragma once

#include <StftPitchShiftPlugin/FFT.h>

#include <StftPitchShift/STFT.h>
#include <StftPitchShift/StftPitchShiftCore.h>

class Core
{

public:

  Core(const double samplerate, const int blocksize, const int dftsize, const int overlap);
  virtual ~Core();

  void normalize(bool value);
  void quefrency(double value);
  void timbre(double value);
  void pitch(std::vector<double> values);

  virtual bool compatible(const int blocksize) const = 0;
  virtual void process(const std::span<const float> input, const std::span<float> output) = 0;

protected:

  size_t get_analysis_window_size() const;
  size_t get_synthesis_window_size() const;

  void stft_pitch_shift(const std::span<const double> input, const std::span<double> output) const;

private:

  struct { size_t analysis_window_size, synthesis_window_size; } config;

  std::unique_ptr<stftpitchshift::STFT<double>> stft;
  std::unique_ptr<stftpitchshift::StftPitchShiftCore<double>> core;

};
