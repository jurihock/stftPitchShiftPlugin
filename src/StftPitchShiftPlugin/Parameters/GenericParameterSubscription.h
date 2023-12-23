#pragma once

#include <JuceHeader.h>

#include <StftPitchShiftPlugin/Parameters/GenericParameterListener.h>

class GenericParameterSubscription final
{

public:

  GenericParameterSubscription(juce::AudioProcessorParameter* parameter,
                               std::function<void()> callback) :
    GenericParameterSubscription(parameter, std::make_shared<GenericParameterListener>(callback))
  {
  }

  GenericParameterSubscription(juce::AudioProcessorParameter* parameter,
                               std::shared_ptr<juce::AudioProcessorParameter::Listener> listener) :
    parameter(parameter),
    listener(listener)
  {
    subscribe();
  }

  ~GenericParameterSubscription()
  {
    unsubscribe();
  }

private:

  juce::AudioProcessorParameter* parameter;
  std::shared_ptr<juce::AudioProcessorParameter::Listener> listener;

  void subscribe()
  {
    if (parameter && listener)
    {
      parameter->addListener(listener.get());
    }
  }

  void unsubscribe()
  {
    if (parameter && listener)
    {
      parameter->removeListener(listener.get());
    }

    parameter = nullptr;
    listener = nullptr;
  }

};
