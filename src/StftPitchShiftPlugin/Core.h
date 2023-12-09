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

  virtual int latency() const = 0;
  virtual bool compatible(const int blocksize) const = 0;
  virtual void process(const std::span<const float> input, const std::span<float> output) = 0;

protected:

  const double samplerate;
  const int blocksize;
  const int dftsize;
  const int overlap;
  const size_t analysis_window_size;
  const size_t synthesis_window_size;

  void stft_pitch_shift(const std::span<const double> input, const std::span<double> output) const;

private:

  std::unique_ptr<stftpitchshift::STFT<double>> stft;
  std::unique_ptr<stftpitchshift::StftPitchShiftCore<double>> core;

};
