#pragma once

#include <JuceHeader.h>

#include <StftPitchShiftPlugin/Parameters.h>

class Editor final : public juce::AudioProcessorEditor
{

public:

  Editor(juce::AudioProcessor& process, std::shared_ptr<Parameters> parameters);
  ~Editor();

  void resized() override;

private:

  std::shared_ptr<Parameters> parameters;
  std::unique_ptr<jive::GuiItem> view;

  struct
  {
    std::vector<std::shared_ptr<juce::ButtonParameterAttachment>> button;
    std::vector<std::shared_ptr<juce::ComboBoxParameterAttachment>> combo;
    std::vector<std::shared_ptr<juce::SliderParameterAttachment>> slider;
  } attachments;

  std::vector<std::shared_ptr<GenericParameterSubscription>> subscriptions;

  template<typename T>
  static T* find(const juce::Component* root, const juce::String& id)
  {
    for (juce::Component* child : root->getChildren())
    {
      if (child->getComponentID() == id)
      {
        if (T* t = static_cast<T*>(child))
        {
          return t;
        }

        throw std::runtime_error(
          "Invalid component type!");
      }

      if (T* t = find<T>(child, id))
      {
        return t;
      }
    }

    return nullptr;
  }

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor)

};
