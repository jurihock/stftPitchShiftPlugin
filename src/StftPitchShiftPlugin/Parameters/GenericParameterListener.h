#pragma once

#include <JuceHeader.h>

class GenericParameterListener final : public juce::AudioProcessorParameter::Listener
{

public:

  GenericParameterListener(const std::function<void()> callback) : callback(callback) {}
  ~GenericParameterListener() {}

  void parameterValueChanged(int index, float value) override { callback(); }
  void parameterGestureChanged(int index, bool gesture) override {}

private:

  const std::function<void()> callback;

};
