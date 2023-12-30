#pragma once

#include <JuceHeader.h>

#include <StftPitchShiftPlugin/FFT.h>

#include <StftPitchShift/STFT.h>
#include <StftPitchShift/StftPitchShiftCore.h>

class Effect
{

public:

  Effect(const double samplerate, const int blocksize, const int dftsize, const int overlap);
  virtual ~Effect();

  void normalize(bool value);
  void quefrency(double value);
  void timbre(double value);
  void pitch(std::vector<double> values);

  virtual int latency() const = 0;
  virtual bool compatible(const int blocksize) const = 0;

  virtual void dry(const std::span<const float> input, const std::span<float> output) = 0;
  virtual void wet(const std::span<const float> input, const std::span<float> output) = 0;

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

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Effect)

};
