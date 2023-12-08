#include <StftPitchShiftPlugin/Core/DelayedCore.h>

DelayedCore::DelayedCore(const double samplerate, const int blocksize, const int dftsize, const int overlap) :
  InstantCore(samplerate, blocksize, dftsize, overlap)
{
  const auto total_buffer_size = get_synthesis_window_size() + get_synthesis_window_size();

  buffer.input.resize(total_buffer_size);
  buffer.output.resize(total_buffer_size);

  samples = 0;
}

DelayedCore::~DelayedCore()
{
}

int DelayedCore::latency() const
{
  return static_cast<int>(get_synthesis_window_size()) + InstantCore::latency();
}

bool DelayedCore::compatible(const int blocksize) const
{
  return static_cast<size_t>(blocksize) <= get_synthesis_window_size();
}

void DelayedCore::process(const std::span<const float> input, const std::span<float> output)
{
  const auto minsamples = input.size();
  const auto maxsamples = get_synthesis_window_size();

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

    InstantCore::process(
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
