#include <StftPitchShiftPlugin/Parameters.h>

#include <StftPitchShiftPlugin/Logger.h>

Parameters::Parameters(juce::AudioProcessor& process) :
  GenericParameterContainer(process)
{
  add("bypass", new juce::AudioParameterBool(
    { "bypass", schema }, "Bypass", false,
    juce::AudioParameterBoolAttributes()));

  add("normalize", new juce::AudioParameterBool(
    { "normalize", schema }, "Normalize", false,
    juce::AudioParameterBoolAttributes()));

  add("quefrency", new juce::AudioParameterFloat(
    { "quefrency", schema }, "Timbre quefrency",
    juce::NormalisableRange<float>(0.0f, 3.0f, 0.001f, 0.5f), 0.0f,
    juce::AudioParameterFloatAttributes().withLabel("ms")));

  add("timbre", new juce::AudioParameterInt(
    { "timbre", schema }, "Timbre shift", -12, +12, 0,
    juce::AudioParameterIntAttributes().withLabel("st")));

  for (int i = 0; i < maxstages; ++i)
  {
    const auto j = std::to_string(i + 1);

    add("pitch", new juce::AudioParameterInt(
      { "pitch" + j, schema }, "Pitch shift " + j, -12, +12, 0,
      juce::AudioParameterIntAttributes().withLabel("st")));
  }

  add("pitch", new juce::AudioParameterInt(
    { "stages", schema }, "Pitch stages", 1, maxstages, maxstages));

  add("reset", new juce::AudioParameterChoice(
    { "stftoverlap", schema }, "Overlap", { "4", "8", "16", "32", "64" }, 0,
    juce::AudioParameterChoiceAttributes()));

  add("reset", new juce::AudioParameterChoice(
    { "dftsize", schema }, "Resolution", { "512", "1024", "2048", "4096", "8192" }, 1,
    juce::AudioParameterChoiceAttributes()));

  add("reset", new juce::AudioParameterBool(
    { "lowlatency", schema }, "Low latency", false,
    juce::AudioParameterBoolAttributes()));
}

Parameters::~Parameters()
{
}

void Parameters::onbypass(std::function<void()> callback)
{
  call("bypass", callback);
}

void Parameters::onnormalize(std::function<void()> callback)
{
  call("normalize", callback);
}

void Parameters::onquefrency(std::function<void()> callback)
{
  call("quefrency", callback);
}

void Parameters::ontimbre(std::function<void()> callback)
{
  call("timbre", callback);
}

void Parameters::onpitch(std::function<void()> callback)
{
  call("pitch", callback);
}

void Parameters::onreset(std::function<void()> callback)
{
  call("reset", callback);
}

bool Parameters::bypass() const
{
  return get<bool>("bypass");
}

bool Parameters::normalize() const
{
  return get<bool>("normalize");
}

double Parameters::quefrency() const
{
  const double milliseconds = get<float>("quefrency");
  const double seconds = milliseconds * 1e-3;

  return seconds;
}

double Parameters::timbre() const
{
  const double semitone = get<int>("timbre");
  const double factor = std::pow(2, semitone / 12);

  return factor;
}

std::vector<double> Parameters::pitch() const
{
  std::set<double> factors;

  for (int i = 0; i < std::min(get<int>("stages"), maxstages); ++i)
  {
    const auto j = std::to_string(i + 1);

    const double semitone = get<int>("pitch" + j);
    const double factor = std::pow(2, semitone / 12);

    factors.insert(factor);
  }

  return std::vector<double>(factors.begin(), factors.end());
}

int Parameters::dftsize(const int blocksize) const
{
  int dftsize = std::stoi(get<std::string>("dftsize"));

  dftsize = std::max(dftsize, 512);
  dftsize = next_power_of_two(dftsize);

  while ((dftsize * 2 < blocksize) && (dftsize < 65536))
  {
    dftsize = next_power_of_two(dftsize + 1);
  }

  return dftsize;
}

int Parameters::overlap(const int blocksize) const
{
  int overlap = std::stoi(get<std::string>("stftoverlap"));

  overlap = std::max(overlap, 1);
  overlap = prev_power_of_two(overlap);

  while ((overlap * 4 > blocksize) && (overlap > 1))
  {
    overlap = prev_power_of_two(overlap - 1);
  }

  return overlap;
}

bool Parameters::lowlatency() const
{
  return get<bool>("lowlatency");
}

void Parameters::load(const void* data, const int size)
{
  try
  {
    auto xml = std::unique_ptr<juce::XmlElement>(
      juce::AudioProcessor::getXmlFromBinary(data, size));

    if (xml)
    {
      LOG(xml->toString(juce::XmlElement::TextFormat().withoutHeader()));
    }
    else
    {
      return;
    }

    if (xml->hasTagName("StftPitchShiftPlugin") == false) { return; }
    if (xml->getIntAttribute("schema") != schema) { return; }

    read<bool>("bypass", *xml);
    read<bool>("normalize", *xml);
    read<float>("quefrency", *xml);
    read<int>("timbre", *xml);

    for (int i = 0; i < maxstages; ++i)
    {
      const auto j = std::to_string(i + 1);

      read<int>("pitch" + j, *xml);
    }

    read<int>("stages", *xml);
    read<std::string>("stftoverlap", *xml);
    read<std::string>("dftsize", *xml);
    read<bool>("lowlatency", *xml);
  }
  catch(const std::exception& exception)
  {
    juce::ignoreUnused(exception);

    LOG(exception.what());
  }
}

void Parameters::save(juce::MemoryBlock& data)
{
  try
  {
    auto xml = std::make_unique<juce::XmlElement>("StftPitchShiftPlugin");

    xml->setAttribute("schema", schema);

    write<bool>("bypass", *xml);
    write<bool>("normalize", *xml);
    write<float>("quefrency", *xml);
    write<int>("timbre", *xml);

    for (int i = 0; i < maxstages; ++i)
    {
      const auto j = std::to_string(i + 1);

      write<int>("pitch" + j, *xml);
    }

    write<int>("stages", *xml);
    write<std::string>("stftoverlap", *xml);
    write<std::string>("dftsize", *xml);
    write<bool>("lowlatency", *xml);

    LOG(xml->toString(juce::XmlElement::TextFormat().withoutHeader()));

    juce::AudioProcessor::copyXmlToBinary(*xml, data);
  }
  catch(const std::exception& exception)
  {
    juce::ignoreUnused(exception);

    LOG(exception.what());
  }
}
