#include "RandomnessDial.h"

//==============================================================================

RandomnessDial::RandomnessDial(AudioProcessorValueTreeState &_vts, juce::String attachmentID) : vts(_vts)
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, attachmentID, slider);
    slider.addListener(this);
    slider.setValue(0);
    addAndMakeVisible(plusToggle);
    plusToggle.setButtonText("+");
    plusToggle.setRadioGroupId(1000);
    plusToggle.addListener(this);
    addAndMakeVisible(plusMinusToggle);
    plusMinusToggle.setButtonText("+/-");
    plusMinusToggle.setRadioGroupId(1000);
    plusMinusToggle.addListener(this);
    plusMinusToggle.setToggleState(true, juce::sendNotification);
    addAndMakeVisible(minusToggle);
    minusToggle.setButtonText("-");
    minusToggle.setRadioGroupId(1000);
    minusToggle.addListener(this);
}

RandomnessDial::~RandomnessDial()
{
}

void RandomnessDial::paint(Graphics &g)
{
    g.setColour(juce::Colours::aliceblue);
}

void RandomnessDial::resized()
{
    auto getW = getWidth() / 3;
    auto getH = getHeight() / 4;
    slider.setBounds(0, 10, getW * 3, getH * 3);
    plusToggle.setBounds(0, getH * 3, getW, getH);
    plusMinusToggle.setBounds(getW, getH * 3, getW, getH);
    minusToggle.setBounds(getW * 2, getH * 3, getW, getH);
}

void RandomnessDial::setValue(float value)
{
    slider.setValue(value);
}

double RandomnessDial::getValue()
{
    return slider.getValue();
}

int RandomnessDial::getToggleState()
{
    return toggleState;
}

void RandomnessDial::sliderValueChanged(Slider *s)
{
    if (s == &slider)
    {
        sendChangeMessage();
    }
}

void RandomnessDial::buttonClicked(Button *button)
{
    if (button == &plusToggle)
    {
        toggleState = 0;
        sendChangeMessage();
    }
    else if (button == &plusMinusToggle)
    {
        toggleState = 1;
        sendChangeMessage();
    }
    else if (button == &minusToggle)
    {
        toggleState = 2;
        sendChangeMessage();
    }
}