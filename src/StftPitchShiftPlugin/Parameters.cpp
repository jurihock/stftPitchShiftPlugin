#include <StftPitchShiftPlugin/Parameters.h>

#include <StftPitchShiftPlugin/Logger.h>

Parameters::Parameters(juce::AudioProcessor& process) :
  process(process),
  parameters(process)
{
  parameters.add("bypass", new juce::AudioParameterBool(
    "bypass", "Bypass pitch shifting", false));

  parameters.add("normalize", new juce::AudioParameterBool(
    "normalize", "Normalize output", false));

  parameters.add("quefrency", new juce::AudioParameterFloat(
    "quefrency", "Timbre quefrency",
    juce::NormalisableRange<float>(0.0f, 10.0f, 0.5f), 0.0f,
    AudioParameterFloatAttributes().withLabel("ms")));

  parameters.add("timbre", new juce::AudioParameterInt(
    "timbre", "Timbre shift", -12, +12, 0,
    AudioParameterIntAttributes().withLabel("st")));

  for (size_t i = 0; i < maxstages; ++i)
  {
    const auto j = std::to_string(i + 1);

    parameters.add("pitch", new juce::AudioParameterInt(
      "pitch" + j, "Pitch shift " + j, -12, +12, 0,
      AudioParameterIntAttributes().withLabel("st")));
  }

  parameters.add("pitch", new juce::AudioParameterInt(
    "stages", "Pitch stages", 1, maxstages, maxstages));
}

Parameters::~Parameters()
{
  juce::ignoreUnused(process);
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

  for (size_t i = 0; i < parameters.get<int>("stages"); ++i)
  {
    const auto j = std::to_string(i + 1);

    const double semitone = parameters.get<int>("pitch" + j);
    const double factor = std::pow(2, semitone / 12);

    factors.insert(factor);
  }

  return std::vector<double>(factors.begin(), factors.end());
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

    for (size_t i = 0; i < maxstages; ++i)
    {
      const auto j = std::to_string(i + 1);

      parameters.read<int>("pitch" + j, *xml);
    }

    parameters.read<int>("stages", *xml);
  }
  catch(const std::exception& exception)
  {
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

    for (size_t i = 0; i < maxstages; ++i)
    {
      const auto j = std::to_string(i + 1);

      parameters.write<int>("pitch" + j, *xml);
    }

    parameters.write<int>("stages", *xml);

    LOG(xml->toString(juce::XmlElement::TextFormat().withoutHeader()));

    juce::AudioProcessor::copyXmlToBinary(*xml, data);
  }
  catch(const std::exception& exception)
  {
    LOG(exception.what());
  }
}
