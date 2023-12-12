#include <StftPitchShiftPlugin/Parameters.h>

#include <StftPitchShiftPlugin/Logger.h>

Parameters::Parameters(juce::AudioProcessor& process) :
  parameters(process)
{
  parameters.add("bypass", new juce::AudioParameterBool(
    { "bypass", schema }, "Bypass pitch shifting", false,
    juce::AudioParameterBoolAttributes()));

  parameters.add("normalize", new juce::AudioParameterBool(
    { "normalize", schema }, "Normalize output", false,
    juce::AudioParameterBoolAttributes()));

  parameters.add("quefrency", new juce::AudioParameterFloat(
    { "quefrency", schema }, "Timbre quefrency",
    juce::NormalisableRange<float>(0.0f, 3.0f, 0.001f, 0.5f), 0.0f,
    juce::AudioParameterFloatAttributes().withLabel("ms")));

  parameters.add("timbre", new juce::AudioParameterInt(
    { "timbre", schema }, "Timbre shift", -12, +12, 0,
    juce::AudioParameterIntAttributes().withLabel("st")));

  for (int i = 0; i < maxstages; ++i)
  {
    const auto j = std::to_string(i + 1);

    parameters.add("pitch", new juce::AudioParameterInt(
      { "pitch" + j, schema }, "Pitch shift " + j, -12, +12, 0,
      juce::AudioParameterIntAttributes().withLabel("st")));
  }

  parameters.add("pitch", new juce::AudioParameterInt(
    { "stages", schema }, "Pitch stages", 1, maxstages, maxstages));

  parameters.add("reset", new juce::AudioParameterChoice(
    { "stftoverlap", schema }, "STFT overlap", { "4", "8", "16", "32", "64" }, 0,
    juce::AudioParameterChoiceAttributes()));

  parameters.add("reset", new juce::AudioParameterChoice(
    { "dftsize", schema }, "DFT size", { "512", "1024", "2048" }, 1,
    juce::AudioParameterChoiceAttributes()));

  parameters.add("reset", new juce::AudioParameterBool(
    { "lowlatency", schema }, "Low latency", false,
    juce::AudioParameterBoolAttributes()));
}

Parameters::~Parameters()
{
}

void Parameters::onbypass(std::function<void()> callback)
{
  parameters.call("bypass", callback);
}

void Parameters::onnormalize(std::function<void()> callback)
{
  parameters.call("normalize", callback);
}

void Parameters::onquefrency(std::function<void()> callback)
{
  parameters.call("quefrency", callback);
}

void Parameters::ontimbre(std::function<void()> callback)
{
  parameters.call("timbre", callback);
}

void Parameters::onpitch(std::function<void()> callback)
{
  parameters.call("pitch", callback);
}

void Parameters::onreset(std::function<void()> callback)
{
  parameters.call("reset", callback);
}

juce::AudioProcessorParameter* Parameters::raw(const std::string& id) const
{
  return parameters.raw(id);
}

bool Parameters::bypass() const
{
  return parameters.get<bool>("bypass");
}

bool Parameters::normalize() const
{
  return parameters.get<bool>("normalize");
}

double Parameters::quefrency() const
{
  const double milliseconds = parameters.get<float>("quefrency");
  const double seconds = milliseconds * 1e-3;

  return seconds;
}

double Parameters::timbre() const
{
  const double semitone = parameters.get<int>("timbre");
  const double factor = std::pow(2, semitone / 12);

  return factor;
}

std::vector<double> Parameters::pitch() const
{
  std::set<double> factors;

  for (int i = 0; i < std::min(parameters.get<int>("stages"), maxstages); ++i)
  {
    const auto j = std::to_string(i + 1);

    const double semitone = parameters.get<int>("pitch" + j);
    const double factor = std::pow(2, semitone / 12);

    factors.insert(factor);
  }

  return std::vector<double>(factors.begin(), factors.end());
}

int Parameters::dftsize(const int blocksize) const
{
  int dftsize = std::stoi(parameters.get<std::string>("dftsize"));

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
  int overlap = std::stoi(parameters.get<std::string>("stftoverlap"));

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
  return parameters.get<bool>("lowlatency");
}

void Parameters::read(const void* data, const int size)
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

    parameters.read<bool>("bypass", *xml);
    parameters.read<bool>("normalize", *xml);
    parameters.read<float>("quefrency", *xml);
    parameters.read<int>("timbre", *xml);

    for (int i = 0; i < maxstages; ++i)
    {
      const auto j = std::to_string(i + 1);

      parameters.read<int>("pitch" + j, *xml);
    }

    parameters.read<int>("stages", *xml);
    parameters.read<std::string>("stftoverlap", *xml);
    parameters.read<std::string>("dftsize", *xml);
    parameters.read<bool>("lowlatency", *xml);
  }
  catch(const std::exception& exception)
  {
    juce::ignoreUnused(exception);

    LOG(exception.what());
  }
}

void Parameters::write(juce::MemoryBlock& data)
{
  try
  {
    auto xml = std::make_unique<juce::XmlElement>("StftPitchShiftPlugin");

    xml->setAttribute("schema", schema);

    parameters.write<bool>("bypass", *xml);
    parameters.write<bool>("normalize", *xml);
    parameters.write<float>("quefrency", *xml);
    parameters.write<int>("timbre", *xml);

    for (int i = 0; i < maxstages; ++i)
    {
      const auto j = std::to_string(i + 1);

      parameters.write<int>("pitch" + j, *xml);
    }

    parameters.write<int>("stages", *xml);
    parameters.write<std::string>("stftoverlap", *xml);
    parameters.write<std::string>("dftsize", *xml);
    parameters.write<bool>("lowlatency", *xml);

    LOG(xml->toString(juce::XmlElement::TextFormat().withoutHeader()));

    juce::AudioProcessor::copyXmlToBinary(*xml, data);
  }
  catch(const std::exception& exception)
  {
    juce::ignoreUnused(exception);

    LOG(exception.what());
  }
}
