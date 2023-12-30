#include <StftPitchShiftPlugin/Effect/DelayedEffect.h>

DelayedEffect::DelayedEffect(const double samplerate, const int blocksize, const int dftsize, const int overlap) :
  InstantEffect(samplerate, dftsize + dftsize, dftsize, overlap), host_block_size(blocksize)
{
  const auto total_buffer_size = analysis_window_size + synthesis_window_size;

  buffer.input.resize(total_buffer_size);
  buffer.output.resize(total_buffer_size);

  samples = 0;
}

DelayedEffect::~DelayedEffect()
{
}

int DelayedEffect::latency() const
{
  return 6 * dftsize - host_block_size;
}

bool DelayedEffect::compatible(const int blocksize) const
{
  return static_cast<size_t>(blocksize) <= synthesis_window_size;
}

void DelayedEffect::dry(const std::span<const float> input, const std::span<float> output)
{
  process(input, output, [&](std::span<float> x, std::span<float> y)
  {
    InstantEffect::dry(x, y);
  });
}

void DelayedEffect::wet(const std::span<const float> input, const std::span<float> output)
{
  process(input, output, [&](std::span<float> x, std::span<float> y)
  {
    InstantEffect::wet(x, y);
  });
}

void DelayedEffect::process(const std::span<const float> input, const std::span<float> output,
                          std::function<void(std::span<float> x, std::span<float> y)> callback)
{
  const auto minsamples = input.size();
  const auto maxsamples = synthesis_window_size;

  // shift input buffer
  std::copy(
    buffer.input.begin() + minsamples,
    buffer.input.end(),
    buffer.input.begin());

  // copy new input samples
  std::copy(
    input.begin(),
    input.end(),
    buffer.input.end() - minsamples);

  // start processing as soon as enough samples are buffered
  if ((samples += minsamples) >= maxsamples)
  {
    const auto x = buffer.input.data() + buffer.input.size();
    const auto y = buffer.output.data() + buffer.output.size();

    callback(
      std::span(x - samples, maxsamples),
      std::span(y - samples, maxsamples));

    samples %= maxsamples;
  }

  // copy new output samples back
  std::copy(
    buffer.output.begin(),
    buffer.output.begin() + minsamples,
    output.begin());

  // shift output buffer
  std::copy(
    buffer.output.begin() + minsamples,
    buffer.output.end(),
    buffer.output.begin());
}
