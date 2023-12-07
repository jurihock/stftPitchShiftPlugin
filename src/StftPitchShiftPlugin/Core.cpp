#include <StftPitchShiftPlugin/Core.h>

Core::Core(const double samplerate, const int blocksize, const int dftsize, const int overlap)
{
  const auto analysis_window_size  = static_cast<size_t>(dftsize + dftsize);
  const auto synthesis_window_size = static_cast<size_t>(blocksize);

  const auto winsize = std::make_tuple(analysis_window_size, synthesis_window_size);
  const auto hopsize = synthesis_window_size / static_cast<size_t>(overlap);

  const auto fft = std::make_shared<FFT>();

  config.analysis_window_size = analysis_window_size;
  config.synthesis_window_size = synthesis_window_size;

  stft = std::make_unique<stftpitchshift::STFT<double>>(fft, winsize, hopsize);
  core = std::make_unique<stftpitchshift::StftPitchShiftCore<double>>(fft, winsize, hopsize, samplerate);
}

Core::~Core()
{
}

void Core::normalize(bool value)
{
  core->normalization(value);
}

void Core::quefrency(double value)
{
  core->quefrency(value);
}

void Core::timbre(double value)
{
  core->distortion(value);
}

void Core::pitch(std::vector<double> values)
{
  core->factors(values);
}

size_t Core::get_analysis_window_size() const
{
  return config.analysis_window_size;
}

size_t Core::get_synthesis_window_size() const
{
  return config.synthesis_window_size;
}

void Core::stft_pitch_shift(const std::span<const double> input, const std::span<double> output) const
{
  (*stft)(input, output, [&](std::span<std::complex<double>> dft)
  {
    core->shiftpitch(dft);
  });
}
