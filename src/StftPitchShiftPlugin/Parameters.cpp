#include <StftPitchShiftPlugin/Parameters.h>

#include <StftPitchShiftPlugin/Logger.h>

Parameters::Parameters(juce::AudioProcessor& process) :
  process(process)
{
  bypass = new juce::AudioParameterBool(
    "bypass", "Bypass pitch shifting", false);

  process.addParameter(bypass);

  quefrency = new juce::AudioParameterFloat(
    "quefrency", "Quefrency milliseconds", 0.0f, 10.0f, 0.0f);

  process.addParameter(quefrency);

  timbre = new juce::AudioParameterInt(
    "timbre", "Timbre shifting semitones", -12, +12, 0);

  process.addParameter(timbre);
}

Parameters::~Parameters()
{
  juce::ignoreUnused(process);
}

void Parameters::write(juce::MemoryBlock& data)
{
  auto write_bool_value = [](juce::XmlElement* const parent, const juce::AudioParameterBool* parameter)
  {
    juce::XmlElement* child = parent->createNewChildElement(parameter->getParameterID());
    child->addTextElement(parameter->get() ? "true" : "false");
  };

  auto write_int_value = [](juce::XmlElement* const parent, const juce::AudioParameterInt* parameter)
  {
    juce::XmlElement* child = parent->createNewChildElement(parameter->getParameterID());
    child->addTextElement(juce::String(parameter->get()));
  };

  auto write_float_value = [](juce::XmlElement* const parent, const juce::AudioParameterFloat* parameter)
  {
    juce::XmlElement* child = parent->createNewChildElement(parameter->getParameterID());
    child->addTextElement(juce::String(parameter->get()));
  };

  auto xml = std::make_unique<juce::XmlElement>("StftPitchShiftPlugin");

  xml->setAttribute("schema", schema);

  write_bool_value(xml.get(), bypass);
  write_float_value(xml.get(), quefrency);
  write_int_value(xml.get(), timbre);

  LOG(xml->toString(juce::XmlElement::TextFormat().withoutHeader()));

  juce::AudioProcessor::copyXmlToBinary(*xml, data);
}

void Parameters::read(const void* data, const int size)
{
  auto read_bool_value = [](const juce::XmlElement* parent, const juce::AudioParameterBool* parameter)
  {
    juce::XmlElement* child = parent->getChildByName(parameter->getParameterID());
    if (!child) { return parameter->get(); }

    juce::String value = child->getAllSubText();
    if (value.isEmpty()) { return parameter->get(); }

    return value == "true";
  };

  auto read_int_value = [](const juce::XmlElement* parent, const juce::AudioParameterInt* parameter)
  {
    juce::XmlElement* child = parent->getChildByName(parameter->getParameterID());
    if (!child) { return parameter->get(); }

    juce::String value = child->getAllSubText();
    if (value.isEmpty()) { return parameter->get(); }

    return value.getIntValue();
  };

  auto read_float_value = [](const juce::XmlElement* parent, const juce::AudioParameterFloat* parameter)
  {
    juce::XmlElement* child = parent->getChildByName(parameter->getParameterID());
    if (!child) { return parameter->get(); }

    juce::String value = child->getAllSubText();
    if (value.isEmpty()) { return parameter->get(); }

    return value.getFloatValue();
  };

  auto xml = std::unique_ptr<juce::XmlElement>(
    juce::AudioProcessor::getXmlFromBinary(data, size));

  if (xml == nullptr) { return; }

  LOG(xml->toString(juce::XmlElement::TextFormat().withoutHeader()));

  if (xml->hasTagName("StftPitchShiftPlugin") == false) { return; }
  if (xml->getIntAttribute("schema") != schema) { return; }

  *bypass = read_bool_value(xml.get(), bypass);
  *quefrency = read_float_value(xml.get(), quefrency);
  *timbre = read_int_value(xml.get(), timbre);
}
