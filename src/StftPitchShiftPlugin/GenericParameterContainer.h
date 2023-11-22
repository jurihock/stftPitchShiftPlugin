#pragma once

#include <JuceHeader.h>

#include <StftPitchShiftPlugin/GenericParameterListener.h>

class GenericParameterContainer final
{

public:

  explicit GenericParameterContainer(juce::AudioProcessor& process) : process(process) {}
  ~GenericParameterContainer() { juce::ignoreUnused(process); }

  void add(const std::string& ns, juce::AudioProcessorParameterWithID* parameter)
  {
    const std::string id = parameter->getParameterID().toStdString();

    parameters[id] = parameter;

    auto listener = std::make_shared<GenericParameterListener>([ns, this]()
    {
      if (callbacks.contains(ns))
      {
        callbacks.at(ns)();
      }
    });

    listeners.push_back(listener);

    parameter->addListener(listener.get());

    process.addParameter(parameter);
  }

  void call(const std::string& ns, std::function<void()> callback)
  {
    callbacks[ns] = callback;
  }

  juce::AudioProcessorParameter* raw(const std::string& id) const
  {
    return parameters.at(id);
  }

  template<typename T>
  T get(const std::string& id) const
  {
    static_assert(missing_template_specialization<T>::value);
  }

  template<>
  bool get<bool>(const std::string& id) const
  {
    auto* parameter = dynamic_cast<juce::AudioParameterBool*>(parameters.at(id));

    return *parameter;
  }

  template<>
  int get<int>(const std::string& id) const
  {
    auto* parameter = dynamic_cast<juce::AudioParameterInt*>(parameters.at(id));

    return *parameter;
  }

  template<>
  float get<float>(const std::string& id) const
  {
    auto* parameter = dynamic_cast<juce::AudioParameterFloat*>(parameters.at(id));

    return *parameter;
  }

  template<>
  std::string get<std::string>(const std::string& id) const
  {
    auto* parameter = dynamic_cast<juce::AudioParameterChoice*>(parameters.at(id));

    return parameter->getCurrentChoiceName().toStdString();
  }

  template<typename T>
  void set(const std::string& id, const T value) const
  {
    static_assert(missing_template_specialization<T>::value);
  }

  template<>
  void set<bool>(const std::string& id, const bool value) const
  {
    auto* parameter = dynamic_cast<juce::AudioParameterBool*>(parameters.at(id));

    *parameter = value;
  }

  template<>
  void set<int>(const std::string& id, const int value) const
  {
    auto* parameter = dynamic_cast<juce::AudioParameterInt*>(parameters.at(id));

    *parameter = value;
  }

  template<>
  void set<float>(const std::string& id, const float value) const
  {
    auto* parameter = dynamic_cast<juce::AudioParameterFloat*>(parameters.at(id));

    *parameter = value;
  }

  template<>
  void set<std::string>(const std::string& id, const std::string value) const
  {
    auto* parameter = dynamic_cast<juce::AudioParameterChoice*>(parameters.at(id));

    const juce::StringArray& choices = parameter->choices;
    const int index = choices.indexOf(value, true);

    if (index >= 0)
    {
      *parameter = index;
    }
  }

  template<typename T>
  void read(const std::string& id, juce::XmlElement& parent) const
  {
    static_assert(missing_template_specialization<T>::value);
  }

  template<>
  void read<bool>(const std::string& id, juce::XmlElement& parent) const
  {
    juce::XmlElement* child = parent.getChildByName(id);
    if (!child) { return; }

    juce::String value = child->getAllSubText();
    if (value.isEmpty()) { return; }

    set<bool>(id, value == "true");
  }

  template<>
  void read<int>(const std::string& id, juce::XmlElement& parent) const
  {
    juce::XmlElement* child = parent.getChildByName(id);
    if (!child) { return; }

    juce::String value = child->getAllSubText();
    if (value.isEmpty()) { return; }

    set<int>(id, value.getIntValue());
  }

  template<>
  void read<float>(const std::string& id, juce::XmlElement& parent) const
  {
    juce::XmlElement* child = parent.getChildByName(id);
    if (!child) { return; }

    juce::String value = child->getAllSubText();
    if (value.isEmpty()) { return; }

    set<float>(id, value.getFloatValue());
  }

  template<>
  void read<std::string>(const std::string& id, juce::XmlElement& parent) const
  {
    juce::XmlElement* child = parent.getChildByName(id);
    if (!child) { return; }

    juce::String value = child->getAllSubText();
    if (value.isEmpty()) { return; }

    set<std::string>(id, value.toStdString());
  }

  template<typename T>
  void write(const std::string& id, juce::XmlElement& parent) const
  {
    static_assert(missing_template_specialization<T>::value);
  }

  template<>
  void write<bool>(const std::string& id, juce::XmlElement& parent) const
  {
    juce::XmlElement* child = parent.createNewChildElement(id);
    child->addTextElement(get<bool>(id) ? "true" : "false");
  }

  template<>
  void write<int>(const std::string& id, juce::XmlElement& parent) const
  {
    juce::XmlElement* child = parent.createNewChildElement(id);
    child->addTextElement(juce::String(get<int>(id)));
  }

  template<>
  void write<float>(const std::string& id, juce::XmlElement& parent) const
  {
    juce::XmlElement* child = parent.createNewChildElement(id);
    child->addTextElement(juce::String(get<float>(id)));
  }

  template<>
  void write<std::string>(const std::string& id, juce::XmlElement& parent) const
  {
    juce::XmlElement* child = parent.createNewChildElement(id);
    child->addTextElement(juce::String(get<std::string>(id)));
  }

private:

  template<typename T>
  struct missing_template_specialization : std::false_type {};

  juce::AudioProcessor& process;

  std::map<std::string, juce::AudioProcessorParameter*> parameters;
  std::map<std::string, std::function<void()>> callbacks;
  std::vector<std::shared_ptr<juce::AudioProcessorParameter::Listener>> listeners;

};
