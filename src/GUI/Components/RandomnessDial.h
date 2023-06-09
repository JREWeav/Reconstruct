#pragma once

#include <JuceHeader.h>

//==============================================================================
class RandomnessDial : public Component, public Slider::Listener, public Button::Listener, public ChangeBroadcaster
{
public:
    RandomnessDial(AudioProcessorValueTreeState &_vts, juce::String sliderID, juce::String buttonID, std::function<String(double value)> textFromValue = nullptr);
    ~RandomnessDial();

    void paint(Graphics &) override;
    void resized() override;
    void setValue(float value);
    double getValue();
    int getToggleState();
    void updateText();

    void sliderValueChanged(Slider *s) override;
    void buttonClicked(Button *) override;

private:
    Slider slider;
    ToggleButton plusToggle;
    ToggleButton plusMinusToggle;
    ToggleButton minusToggle;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attachment;

    int toggleState = 0;
    AudioProcessorValueTreeState &vts;

    juce::String sliderID;
    juce::String buttonID;
};