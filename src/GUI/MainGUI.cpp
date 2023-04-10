#include "MainGUI.h"

/*
TODO:
- Hook up the keyboard component to the synth
- Add a slider to control the volume of the synth
- Add a slider to control the pitch of the grains
- Add a slider to control the speed of the grains
- Add a slider to control the length of the grains
*/

MainGUI::MainGUI(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, AudioPluginAudioProcessor &p) : loadComponent{formatManager, thumbnailCache, p}, processor{p}
{
    addAndMakeVisible(loadComponent);
    addAndMakeVisible(keyboardComponent);
    keyboardComponent.setMidiChannel(1);

    addAndMakeVisible(grainsPerSecondSlider);
    grainsPerSecondSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainsPerSecondSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainsPerSecondSlider.setRange(0.0f, 440.0f, 0.1f);
    grainsPerSecondSlider.setValue(1.0f);
    grainsPerSecondSlider.addListener(this);
    addAndMakeVisible(grainsPerSecondLabel);
    grainsPerSecondLabel.setText("Grains per second (Hz)", juce::dontSendNotification);
    grainsPerSecondLabel.attachToComponent(&grainsPerSecondSlider, false);
    grainsPerSecondLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainLengthSlider);
    grainLengthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainLengthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainLengthSlider.setRange(1, 600, 1);
    grainLengthSlider.setValue(100);
    grainLengthSlider.addListener(this);
    addAndMakeVisible(grainLengthLabel);
    grainLengthLabel.setText("Grain length (ms)", juce::dontSendNotification);
    grainLengthLabel.attachToComponent(&grainLengthSlider, false);
    grainLengthLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainSpeedSlider);
    grainSpeedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainSpeedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainSpeedSlider.setRange(0, 200, 1);
    grainSpeedSlider.setValue(100);
    grainSpeedSlider.addListener(this);
    addAndMakeVisible(grainSpeedLabel);
    grainSpeedLabel.setText("Grain speed (%)", juce::dontSendNotification);
    grainSpeedLabel.attachToComponent(&grainSpeedSlider, false);
    grainSpeedLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainPanSlider);
    grainPanSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainPanSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainPanSlider.setRange(0, 100, 1);
    grainPanSlider.setValue(50);
    grainPanSlider.addListener(this);
    addAndMakeVisible(grainPanLabel);
    grainPanLabel.setText("Grain pan (%)", juce::dontSendNotification);
    grainPanLabel.attachToComponent(&grainPanSlider, false);
    grainPanLabel.setJustificationType(juce::Justification::centred);
}

MainGUI::~MainGUI()
{
}

void MainGUI::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
}

void MainGUI::resized()
{
    float getW = (getWidth() / 16);
    float getH = (getHeight() / 16);
    loadComponent.setBounds(0, 0, getW * 16, getH * 8);
    keyboardComponent.setBounds(0, getH * 9, getW * 6, getH * 4);

    grainsPerSecondSlider.setBounds(getW * 6, getH * 8, getW * 3, getH * 3);
    grainLengthSlider.setBounds(getW * 9, getH * 8, getW * 3, getH * 3);
    grainSpeedSlider.setBounds(getW * 6, getH * 12, getW * 3, getH * 3);
    grainPanSlider.setBounds(getW * 9, getH * 12, getW * 3, getH * 3);
}

void MainGUI::buttonClicked(juce::Button *button)
{
}

void MainGUI::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &grainsPerSecondSlider)
    {
        processor.setGrainParameters(grainsPerSecondSlider.getValue(), grainLengthSlider.getValue(), grainPanSlider.getValue(), grainSpeedSlider.getValue());
    }
    else if (slider == &grainLengthSlider)
    {
        processor.setGrainParameters(grainsPerSecondSlider.getValue(), grainLengthSlider.getValue(), grainPanSlider.getValue(), grainSpeedSlider.getValue());
    }
    else if (slider == &grainSpeedSlider)
    {
        processor.setGrainParameters(grainsPerSecondSlider.getValue(), grainLengthSlider.getValue(), grainPanSlider.getValue(), grainSpeedSlider.getValue());
    }
    else if (slider == &grainPanSlider)
    {
        processor.setGrainParameters(grainsPerSecondSlider.getValue(), grainLengthSlider.getValue(), grainPanSlider.getValue(), grainSpeedSlider.getValue());
    }
}
