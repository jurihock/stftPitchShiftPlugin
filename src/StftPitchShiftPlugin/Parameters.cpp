#include <StftPitchShiftPlugin/Parameters.h>

#include <StftPitchShiftPlugin/Logger.h>

Parameters::Parameters(juce::AudioProcessor& process) :
  process(process)
{
  params.bypass = new juce::AudioParameterBool(
    "bypass", "Bypass pitch shifting", false);

  process.addParameter(params.bypass);

  params.normalize = new juce::AudioParameterBool(
    "normalize", "Normalize output", false);

  process.addParameter(params.normalize);

  params.quefrency = new juce::AudioParameterFloat(
    "quefrency", "Timbre quefrency",
    juce::NormalisableRange<float>(0.0f, 10.0f, 0.5f), 0.0f,
    AudioParameterFloatAttributes().withLabel("ms"));

  process.addParameter(params.quefrency);

  params.timbre = new juce::AudioParameterInt(
    "timbre", "Timbre shift", -12, +12, 0,
    AudioParameterIntAttributes().withLabel("st"));

  process.addParameter(params.timbre);

  for (size_t i = 0; i < params.pitch.size(); ++i)
  {
    const auto j = std::to_string(i + 1);

    params.pitch[i] = new juce::AudioParameterInt(
      "pitch" + j, "Pitch shift " + j, -12, +12, 0,
      AudioParameterIntAttributes().withLabel("st"));

    process.addParameter(params.pitch[i]);
  }
}

Parameters::~Parameters()
{
  juce::ignoreUnused(process);
}

void Parameters::read(const void* data, const int size)
{
  auto read_bool_value = [](juce::XmlElement& parent, juce::AudioParameterBool* parameter)
  {
    juce::XmlElement* child = parent.getChildByName(parameter->getParameterID());
    if (!child) { return; }

    juce::String value = child->getAllSubText();
    if (value.isEmpty()) { return; }

    *parameter = (value == "true");
  };

  auto read_int_value = [](juce::XmlElement& parent, juce::AudioParameterInt* parameter)
  {
    juce::XmlElement* child = parent.getChildByName(parameter->getParameterID());
    if (!child) { return; }

    juce::String value = child->getAllSubText();
    if (value.isEmpty()) { return; }

    *parameter = value.getIntValue();
  };

  auto read_float_value = [](juce::XmlElement& parent, juce::AudioParameterFloat* parameter)
  {
    juce::XmlElement* child = parent.getChildByName(parameter->getParameterID());
    if (!child) { return; }

    juce::String value = child->getAllSubText();
    if (value.isEmpty()) { return; }

    *parameter = value.getFloatValue();
  };

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

    read_bool_value(*xml, params.bypass);
    read_bool_value(*xml, params.normalize);
    read_float_value(*xml, params.quefrency);
    read_int_value(*xml, params.timbre);

    for (size_t i = 0; i < params.pitch.size(); ++i)
    {
      read_int_value(*xml, params.pitch[i]);
    }
  }
  catch(const std::exception& exception)
  {
    LOG(exception.what());
  }
}

void Parameters::write(juce::MemoryBlock& data)
{
  auto write_bool_value = [](juce::XmlElement& parent, juce::AudioParameterBool* parameter)
  {
    juce::XmlElement* child = parent.createNewChildElement(parameter->getParameterID());
    child->addTextElement(parameter->get() ? "true" : "false");
  };

  auto write_int_value = [](juce::XmlElement& parent, juce::AudioParameterInt* parameter)
  {
    juce::XmlElement* child = parent.createNewChildElement(parameter->getParameterID());
    child->addTextElement(juce::String(parameter->get()));
  };

  auto write_float_value = [](juce::XmlElement& parent, juce::AudioParameterFloat* parameter)
  {
    juce::XmlElement* child = parent.createNewChildElement(parameter->getParameterID());
    child->addTextElement(juce::String(parameter->get()));
  };

  try
  {
    auto xml = std::make_unique<juce::XmlElement>("StftPitchShiftPlugin");

    xml->setAttribute("schema", schema);

    write_bool_value(*xml, params.bypass);
    write_bool_value(*xml, params.normalize);
    write_float_value(*xml, params.quefrency);
    write_int_value(*xml, params.timbre);

    for (size_t i = 0; i < params.pitch.size(); ++i)
    {
      write_int_value(*xml, params.pitch[i]);
    }

    LOG(xml->toString(juce::XmlElement::TextFormat().withoutHeader()));

    juce::AudioProcessor::copyXmlToBinary(*xml, data);
  }
  catch(const std::exception& exception)
  {
    LOG(exception.what());
  }
}
