#pragma once

#include <JuceHeader.h>

#include <StftPitchShiftPlugin/Parameters/GenericParameterListener.h>

class GenericParameterContainer
{

public:

  GenericParameterContainer(juce::AudioProcessor& process) : process(process) {}
  virtual ~GenericParameterContainer() {}

  juce::RangedAudioParameter* get(const std::string& id) const
  {
    return parameters.at(id);
  }

  template<typename T>
  T get(const std::string& id) const
  {
    static_assert(missing_template_specialization<T>::value);
  }

  template<typename T>
  void set(const std::string& id, const T value) const
  {
    static_assert(missing_template_specialization<T>::value);
  }

  void on(const std::string& id_or_ns, std::function<void()> callback)
  {
    callbacks[id_or_ns].push_back(callback);
  }

protected:

  void add(const std::string& ns, juce::RangedAudioParameter* parameter)
  {
    const std::string id = parameter->getParameterID().toStdString();

    parameters[id] = parameter;

    auto listener = std::make_shared<GenericParameterListener>([id, ns, this]()
    {
      if (callbacks.contains(id))
      {
        for (const auto& callback : callbacks.at(id))
        {
          callback();
        }
      }

      if (ns == id)
      {
        return;
      }

      if (callbacks.contains(ns))
      {
        for (const auto& callback : callbacks.at(ns))
        {
          callback();
        }
      }
    });

    listeners.push_back(listener);

    parameter->addListener(listener.get());

    process.addParameter(parameter);
  }

  template<typename T>
  void read(const std::string& id, juce::XmlElement& parent) const
  {
    static_assert(missing_template_specialization<T>::value);
  }

  template<typename T>
  void write(const std::string& id, juce::XmlElement& parent) const
  {
    static_assert(missing_template_specialization<T>::value);
  }

private:

  template<typename T>
  struct missing_template_specialization : std::false_type {};

  juce::AudioProcessor& process;

  std::map<std::string, juce::RangedAudioParameter*> parameters;
  std::map<std::string, std::vector<std::function<void()>>> callbacks;
  std::vector<std::shared_ptr<juce::AudioProcessorParameter::Listener>> listeners;

};

template<>
inline bool GenericParameterContainer::get<bool>(const std::string& id) const
{
  auto* parameter = dynamic_cast<juce::AudioParameterBool*>(parameters.at(id));

  return *parameter;
}

template<>
inline int GenericParameterContainer::get<int>(const std::string& id) const
{
  auto* parameter = dynamic_cast<juce::AudioParameterInt*>(parameters.at(id));

  return *parameter;
}

template<>
inline float GenericParameterContainer::get<float>(const std::string& id) const
{
  auto* parameter = dynamic_cast<juce::AudioParameterFloat*>(parameters.at(id));

  return *parameter;
}

template<>
inline std::string GenericParameterContainer::get<std::string>(const std::string& id) const
{
  auto* parameter = dynamic_cast<juce::AudioParameterChoice*>(parameters.at(id));

  return parameter->getCurrentChoiceName().toStdString();
}

template<>
inline void GenericParameterContainer::set<bool>(const std::string& id, const bool value) const
{
  auto* parameter = dynamic_cast<juce::AudioParameterBool*>(parameters.at(id));

  *parameter = value;
}

template<>
inline void GenericParameterContainer::set<int>(const std::string& id, const int value) const
{
  auto* parameter = dynamic_cast<juce::AudioParameterInt*>(parameters.at(id));

  *parameter = value;
}

template<>
inline void GenericParameterContainer::set<float>(const std::string& id, const float value) const
{
  auto* parameter = dynamic_cast<juce::AudioParameterFloat*>(parameters.at(id));

  *parameter = value;
}

template<>
inline void GenericParameterContainer::set<std::string>(const std::string& id, const std::string value) const
{
  auto* parameter = dynamic_cast<juce::AudioParameterChoice*>(parameters.at(id));

  const juce::StringArray& choices = parameter->choices;
  const int index = choices.indexOf(value, true);

  if (index >= 0)
  {
    *parameter = index;
  }
}

template<>
inline void GenericParameterContainer::read<bool>(const std::string& id, juce::XmlElement& parent) const
{
  juce::XmlElement* child = parent.getChildByName(id);
  if (!child) { return; }

  juce::String value = child->getAllSubText();
  if (value.isEmpty()) { return; }

  set<bool>(id, value == "true");
}

template<>
inline void GenericParameterContainer::read<int>(const std::string& id, juce::XmlElement& parent) const
{
  juce::XmlElement* child = parent.getChildByName(id);
  if (!child) { return; }

  juce::String value = child->getAllSubText();
  if (value.isEmpty()) { return; }

  set<int>(id, value.getIntValue());
}

template<>
inline void GenericParameterContainer::read<float>(const std::string& id, juce::XmlElement& parent) const
{
  juce::XmlElement* child = parent.getChildByName(id);
  if (!child) { return; }

  juce::String value = child->getAllSubText();
  if (value.isEmpty()) { return; }

  set<float>(id, value.getFloatValue());
}

template<>
inline void GenericParameterContainer::read<std::string>(const std::string& id, juce::XmlElement& parent) const
{
  juce::XmlElement* child = parent.getChildByName(id);
  if (!child) { return; }

  juce::String value = child->getAllSubText();
  if (value.isEmpty()) { return; }

  set<std::string>(id, value.toStdString());
}

template<>
inline void GenericParameterContainer::write<bool>(const std::string& id, juce::XmlElement& parent) const
{
  juce::XmlElement* child = parent.createNewChildElement(id);
  child->addTextElement(get<bool>(id) ? "true" : "false");
}

template<>
inline void GenericParameterContainer::write<int>(const std::string& id, juce::XmlElement& parent) const
{
  juce::XmlElement* child = parent.createNewChildElement(id);
  child->addTextElement(juce::String(get<int>(id)));
}

template<>
inline void GenericParameterContainer::write<float>(const std::string& id, juce::XmlElement& parent) const
{
  juce::XmlElement* child = parent.createNewChildElement(id);
  child->addTextElement(juce::String(get<float>(id)));
}

template<>
inline void GenericParameterContainer::write<std::string>(const std::string& id, juce::XmlElement& parent) const
{
  juce::XmlElement* child = parent.createNewChildElement(id);
  child->addTextElement(juce::String(get<std::string>(id)));
}
