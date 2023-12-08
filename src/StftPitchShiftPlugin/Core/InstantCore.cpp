#include <StftPitchShiftPlugin/Core/InstantCore.h>

InstantCore::InstantCore(const double samplerate, const int blocksize, const int dftsize, const int overlap) :
  Core(samplerate, blocksize, dftsize, overlap)
{
  const auto total_buffer_size = get_analysis_window_size() + get_synthesis_window_size();

  buffer.input.resize(total_buffer_size);
  buffer.output.resize(total_buffer_size);
}

InstantCore::~InstantCore()
{
}

int InstantCore::latency() const
{
  return get_synthesis_window_size();
}

bool InstantCore::compatible(const int blocksize) const
{
  return static_cast<size_t>(blocksize) == get_synthesis_window_size();
}

void InstantCore::process(const std::span<const float> input, const std::span<float> output)
{
  const auto analysis_window_size  = get_analysis_window_size();
  const auto synthesis_window_size = get_synthesis_window_size();

  // shift input buffer
  std::copy(
    buffer.input.begin() + synthesis_window_size,
    buffer.input.end(),
    buffer.input.begin());

  // copy new input samples
  std::transform(
    input.begin(),
    input.end(),
    buffer.input.begin() + analysis_window_size,
    transform<float, double>);

  // apply pitch shifting within the built-in STFT routine
  stft_pitch_shift(buffer.input, buffer.output);

  // copy new output samples back
  std::transform(
    buffer.output.begin() - synthesis_window_size + analysis_window_size,
    buffer.output.end() - synthesis_window_size,
    output.begin(),
    transform<double, float>);

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
