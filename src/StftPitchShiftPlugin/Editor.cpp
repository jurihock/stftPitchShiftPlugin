#include <StftPitchShiftPlugin/Editor.h>

#include <StftPitchShiftPlugin/Logger.h>

Editor::Editor(juce::AudioProcessor& process, std::shared_ptr<Parameters> parameters) :
  juce::AudioProcessorEditor(process),
  parameters(parameters)
{
  jive::Interpreter interpreter;

  juce::String xml(
    R"(
      <Component {flex-col} width="550" height="600" padding="10">

        <Component {flex-col} {flex-fill}>

          <Component {flex-row} {fill}>
            <Component {flex-col} {fill}>
              <Name  id="quefrency-name"/>
              <Knob  id="quefrency-slider" {flex-fill}/>
              <Value id="quefrency-value"/>
            </Component>
            <Component {flex-col} {fill}>
              <Name  id="timbre-name"/>
              <Knob  id="timbre-slider" {flex-fill}/>
              <Value id="timbre-value"/>
            </Component>
          </Component>

          <Component {flex-row} {fill}>
            <Component {flex-col} {fill}>
              <Name  id="pitch1-name"/>
              <Knob  id="pitch1-slider" {flex-fill}/>
              <Value id="pitch1-value"/>
            </Component>
            <Component {flex-col} {fill}>
              <Name  id="pitch2-name"/>
              <Knob  id="pitch2-slider" {flex-fill}/>
              <Value id="pitch2-value"/>
            </Component>
            <Component {flex-col} {fill}>
              <Name  id="pitch3-name"/>
              <Knob  id="pitch3-slider" {flex-fill}/>
              <Value id="pitch3-value"/>
            </Component>
          </Component>

          <Component {flex-row} {fill}>
            <Component {flex-col} {fill}>
              <Name  id="pitch4-name"/>
              <Knob  id="pitch4-slider" {flex-fill}/>
              <Value id="pitch4-value"/>
            </Component>
            <Component {flex-col} {fill}>
              <Name  id="pitch5-name"/>
              <Knob  id="pitch5-slider" {flex-fill}/>
              <Value id="pitch5-value"/>
            </Component>
            <Component {flex-col} {fill}>
              <Name  id="stages-name"/>
              <Knob  id="stages-slider" {flex-fill}/>
              <Value id="stages-value"/>
            </Component>
          </Component>

        </Component>

        <Component {flex-row}>
          <Component {flex-col} {flex-fill}>
            <Name     id="bypass-name"/>
            <Checkbox id="bypass-button"/>
          </Component>
          <Component {flex-col} {flex-fill}>
            <Name     id="normalize-name"/>
            <Checkbox id="normalize-button"/>
          </Component>
          <Component {flex-col} {flex-fill}>
            <Name     id="lowlatency-name"/>
            <Checkbox id="lowlatency-button"/>
          </Component>
          <Component {flex-col} {flex-fill}>
            <Name     id="stftoverlap-name"/>
            <ComboBox id="stftoverlap-combo"/>
          </Component>
          <Component {flex-col} {flex-fill}>
            <Name     id="dftsize-name"/>
            <ComboBox id="dftsize-combo"/>
          </Component>
        </Component>

      </Component>
    )");

  xml = xml.replace("<Name",       R"(<Label min-height="40")");
  xml = xml.replace("<Value",      R"(<Label min-height="20" opacity="0.6")");
  xml = xml.replace("<Checkbox",   R"(<Checkbox toggle-on-click="true")");
  xml = xml.replace("<ComboBox",   R"(<ComboBox width="90%")");
  xml = xml.replace("<Knob",       R"(<Knob width="100%" height="100%")");
  xml = xml.replace("{fill}",      R"(width="100%" height="100%")");
  xml = xml.replace("{flex-fill}", R"(flex-grow="1")");
  xml = xml.replace("{flex-row}",  R"(display="flex" flex-direction="row")");
  xml = xml.replace("{flex-col}",  R"(display="flex" flex-direction="column")");

  auto* root = (view = interpreter.interpret(xml))->getComponent().get();

  auto attach_checkbox = [&](const std::string& id)
  {
    auto* parameter = parameters->get(id);

    auto* button = find<juce::Button>(root, id + "-button");
    auto* name   = find<juce::Label>(root,  id + "-name");

    auto notify = juce::NotificationType::dontSendNotification;

    attachments.button.push_back(std::make_shared<juce::ButtonParameterAttachment>(*parameter, *button));

    name->setJustificationType(juce::Justification::centredLeft);
    name->setText(parameter->getName(42), notify);
  };

  auto attach_combobox = [&](const std::string& id)
  {
    auto* parameter = parameters->get(id);

    auto* combo = find<juce::ComboBox>(root, id + "-combo");
    auto* name  = find<juce::Label>(root,    id + "-name");

    auto choices = static_cast<juce::AudioParameterChoice*>(parameter)->choices;
    auto notify = juce::NotificationType::dontSendNotification;

    combo->addItemList(choices, 1);

    attachments.combo.push_back(std::make_shared<juce::ComboBoxParameterAttachment>(*parameter, *combo));

    name->setJustificationType(juce::Justification::centredLeft);
    name->setText(parameter->getName(42), notify);
  };

  auto attach_slider = [&](const std::string& id)
  {
    auto* parameter = parameters->get(id);

    auto* slider = find<juce::Slider>(root, id + "-slider");
    auto* name   = find<juce::Label>(root,  id + "-name");
    auto* value  = find<juce::Label>(root,  id + "-value");

    auto unit = parameter->getLabel().isEmpty() ? juce::String("") : (" " + parameter->getLabel());
    auto notify = juce::NotificationType::dontSendNotification;

    slider->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);

    attachments.slider.push_back(std::make_shared<juce::SliderParameterAttachment>(*parameter, *slider));

    name->setJustificationType(juce::Justification::centred);
    value->setJustificationType(juce::Justification::centred);

    name->setText(parameter->getName(42), notify);
    value->setText(parameter->getText(parameter->getValue(), 42) + unit, notify);

    auto update_value_sync = [parameter, value, unit, notify]()
    {
      value->setText(parameter->getText(parameter->getValue(), 42) + unit, notify);
    };

    auto update_value_async = [update_value_sync]()
    {
      juce::MessageManager::callAsync(update_value_sync);
    };

    subscriptions.push_back(parameters->subscribe(id, update_value_async));

    // FIXME: how to change font size via style in XML string?
    {
      auto font = name->getFont();
      font.setHeight(font.getHeight() * 1.1f);
      name->setFont(font);
    }
  };

  const std::array<juce::Slider*, 1> timbre_slider
  {
    find<juce::Slider>(root, "timbre-slider")
  };

  auto update_timbre_slider_sync = [timbre_slider, parameters]()
  {
    auto quefrency = parameters->get<float>("quefrency");

    auto* component = timbre_slider.at(0)->getParentComponent();
    auto enabled = quefrency > 0;

    component->setEnabled(enabled);
  };

  auto update_timbre_slider_async = [update_timbre_slider_sync]()
  {
    juce::MessageManager::callAsync(update_timbre_slider_sync);
  };

  const std::array<juce::Slider*, 5> pitch_sliders
  {
    find<juce::Slider>(root, "pitch1-slider"),
    find<juce::Slider>(root, "pitch2-slider"),
    find<juce::Slider>(root, "pitch3-slider"),
    find<juce::Slider>(root, "pitch4-slider"),
    find<juce::Slider>(root, "pitch5-slider")
  };

  auto update_pitch_sliders_sync = [pitch_sliders, parameters]()
  {
    auto maxstages = static_cast<int>(pitch_sliders.size());
    auto stages = parameters->get<int>("stages");

    for (int i = 0; i < maxstages; ++i)
    {
      auto* component = pitch_sliders.at(i)->getParentComponent();
      auto enabled = i < stages;

      component->setEnabled(enabled);
    }
  };

  auto update_pitch_sliders_async = [update_pitch_sliders_sync]()
  {
    juce::MessageManager::callAsync(update_pitch_sliders_sync);
  };

  attach_slider("quefrency");
  attach_slider("timbre");
  attach_slider("pitch1");
  attach_slider("pitch2");
  attach_slider("pitch3");
  attach_slider("pitch4");
  attach_slider("pitch5");
  attach_slider("stages");

  update_timbre_slider_sync();
  subscriptions.push_back(parameters->subscribe(
    "quefrency", update_timbre_slider_async));

  update_pitch_sliders_sync();
  subscriptions.push_back(parameters->subscribe(
    "stages", update_pitch_sliders_async));

  attach_checkbox("bypass");
  attach_checkbox("normalize");
  attach_checkbox("lowlatency");
  attach_combobox("stftoverlap");
  attach_combobox("dftsize");

  addAndMakeVisible(*root);
  setSize(root->getWidth(), root->getHeight());
  setResizable(true, true);
}

Editor::~Editor()
{
}

void Editor::resized()
{
  view->getComponent()->setSize(getWidth(), getHeight());
}
