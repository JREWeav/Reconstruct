#include "RandomnessDial.h"

//==============================================================================

RandomnessDial::RandomnessDial(AudioProcessorValueTreeState &_vts, juce::String sliderID, juce::String buttonID, std::function<String(double value)> textFromValue) : vts(_vts), sliderID(sliderID), buttonID(buttonID)
{
    DBG(sliderID);
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, sliderID, slider);
    slider.setNumDecimalPlacesToDisplay(0);
    slider.addListener(this);
    slider.textFromValueFunction = textFromValue;
    addAndMakeVisible(plusToggle);
    plusToggle.setButtonText("+");
    plusToggle.setRadioGroupId(1000);
    plusToggle.addListener(this);
    addAndMakeVisible(plusMinusToggle);
    plusMinusToggle.setButtonText("+/-");
    plusMinusToggle.setRadioGroupId(1000);
    plusMinusToggle.addListener(this);
    addAndMakeVisible(minusToggle);
    minusToggle.setButtonText("-");
    minusToggle.setRadioGroupId(1000);
    minusToggle.addListener(this);

    if ((int)floor(vts.getRawParameterValue(buttonID)->load()) == 0)
    {
        plusToggle.setToggleState(true, juce::dontSendNotification);
        toggleState = 0;
    }
    else if ((int)floor(vts.getRawParameterValue(buttonID)->load()) == 1)
    {
        plusMinusToggle.setToggleState(true, juce::dontSendNotification);
        toggleState = 1;
    }
    else if ((int)floor(vts.getRawParameterValue(buttonID)->load()) == 2)
    {
        minusToggle.setToggleState(true, juce::dontSendNotification);
        toggleState = 2;
    }
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
    slider.setBounds(0, 0, getW * 3, getH * 3);
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
    }
    else if (button == &plusMinusToggle)
    {
        toggleState = 1;
    }
    else if (button == &minusToggle)
    {
        toggleState = 2;
    }
    vts.getParameter(buttonID)->beginChangeGesture();
    vts.getParameter(buttonID)->setValueNotifyingHost(0.5f * (float)toggleState);
    vts.getParameter(buttonID)->endChangeGesture();
    sendChangeMessage();
}

void RandomnessDial::updateText()
{
    slider.updateText();
}