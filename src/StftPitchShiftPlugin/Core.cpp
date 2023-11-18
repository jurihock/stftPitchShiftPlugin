#include <StftPitchShiftPlugin/Core.h>

Core::Core(const double samplerate, const int blocksize, const int dftsize, const int overlap)
{
  const auto analysis_window_size  = static_cast<size_t>(dftsize + dftsize);
  const auto synthesis_window_size = static_cast<size_t>(blocksize);

  const auto winsize = std::make_tuple(analysis_window_size, synthesis_window_size);
  const auto hopsize = synthesis_window_size / static_cast<size_t>(overlap);

  config.analysis_window_size = analysis_window_size;
  config.synthesis_window_size = synthesis_window_size;

  buffer.input.resize(analysis_window_size + synthesis_window_size);
  buffer.output.resize(analysis_window_size + synthesis_window_size);

  stft = std::make_unique<STFT<double>>(winsize, hopsize);
  core = std::make_unique<StftPitchShiftCore<double>>(winsize, hopsize, samplerate);

  // preset: neutral
  // core->factors({ 1 });
  // core->quefrency(0 * 1e-3);
  // core->distortion(1);
  // core->normalization(false);

  // preset: major chord with preserved timbre
  core->factors({ 0.5, 1, 1.25, 1.5, 2 });
  core->quefrency(1 * 1e-3);
  core->distortion(1);
  core->normalization(true);
}

Core::~Core()
{
}

bool Core::compatible(const int blocksize) const
{
  return blocksize == config.synthesis_window_size;
}

void Core::process(const std::span<float> input, const std::span<float> output)
{
  const auto analysis_window_size  = config.analysis_window_size;
  const auto synthesis_window_size = config.synthesis_window_size;

  // shift input buffer
  std::copy(
    buffer.input.begin() + synthesis_window_size,
    buffer.input.end(),
    buffer.input.begin());

  // copy new input samples
  std::copy(
    input.begin(),
    input.end(),
    buffer.input.begin() + analysis_window_size);

  // apply pitch shifting within the built-in STFT routine
  (*stft)(buffer.input, buffer.output, [&](std::span<std::complex<double>> dft)
  {
    core->shiftpitch(dft);
  });

  // copy new output samples back
  std::copy(
    buffer.output.begin() - synthesis_window_size + analysis_window_size,
    buffer.output.end() - synthesis_window_size,
    output.begin());

  // shift output buffer
  std::copy(
    buffer.output.begin() + synthesis_window_size,
    buffer.output.end(),
    buffer.output.begin());

  // prepare for the next callback
  std::fill(
    buffer.output.begin() + analysis_window_size,
    buffer.output.end(),
    0);
}
