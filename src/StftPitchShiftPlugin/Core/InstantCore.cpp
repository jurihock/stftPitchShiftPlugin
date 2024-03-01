#include <StftPitchShiftPlugin/Core/InstantCore.h>

InstantCore::InstantCore(const double samplerate, const int blocksize, const int dftsize, const int overlap) :
  Core(samplerate, blocksize, dftsize, overlap)
{
  const auto total_buffer_size = analysis_window_size + synthesis_window_size;

  buffer.input.resize(total_buffer_size);
  buffer.output.resize(total_buffer_size);
}

InstantCore::~InstantCore()
{
}

int InstantCore::latency() const
{
  return static_cast<int>(synthesis_window_size);
}

bool InstantCore::compatible(const int blocksize) const
{
  return static_cast<size_t>(blocksize) == synthesis_window_size;
}

void InstantCore::dry(const std::span<const float> input, const std::span<float> output)
{
  process(input, output, [](std::span<double> x, std::span<double> y)
  {
    std::copy(x.begin(), x.end(), y.begin());
  });
}

void InstantCore::wet(const std::span<const float> input, const std::span<float> output)
{
  process(input, output, [&](std::span<double> x, std::span<double> y)
  {
    stft_pitch_shift(x, y);
  });
}

void InstantCore::process(const std::span<const float> input, const std::span<float> output,
                          std::function<void(std::span<double> x, std::span<double> y)> callback)
{
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

  // start processing
  callback(buffer.input, buffer.output);

  // copy new output samples back
  std::transform(
    (buffer.output.begin() + analysis_window_size) - synthesis_window_size,
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
