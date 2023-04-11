#include "MainGUI.h"

/*
TODO:
- Hook up the keyboard component to the synth
*/

MainGUI::MainGUI(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, AudioPluginAudioProcessor &p) : loadComponent{formatManager, thumbnailCache, p}, processor{p}
{
    addAndMakeVisible(loadComponent);
    addAndMakeVisible(keyboardComponent);
    keyboardComponent.setMidiChannel(1);

    // Grains per second
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

    // Grain parameters
    addAndMakeVisible(grainVolumeSlider);
    grainVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainVolumeSlider.setRange(0, 100, 1);
    grainVolumeSlider.setValue(50);
    grainVolumeSlider.addListener(this);
    addAndMakeVisible(grainVolumeLabel);
    grainVolumeLabel.setText("Grain Volume", juce::dontSendNotification);
    grainVolumeLabel.attachToComponent(&grainVolumeSlider, false);
    grainVolumeLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainLengthSlider);
    grainLengthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainLengthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainLengthSlider.setRange(1, 4000, 1);
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

    // Grain Randomization
    addAndMakeVisible(grainLengthRandomnessSlider);
    grainLengthRandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainLengthRandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainLengthRandomnessSlider.setRange(0, 100, 1);
    grainLengthRandomnessSlider.setValue(0);
    grainLengthRandomnessSlider.addListener(this);
    addAndMakeVisible(grainLengthRandomnessLabel);
    grainLengthRandomnessLabel.setText("Grain Length Random Offset +/-", juce::dontSendNotification);
    grainLengthRandomnessLabel.attachToComponent(&grainLengthRandomnessSlider, false);
    grainLengthRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainSpeedRandomnessSlider);
    grainSpeedRandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainSpeedRandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainSpeedRandomnessSlider.setRange(0, 50, 1);
    grainSpeedRandomnessSlider.setValue(0);
    grainSpeedRandomnessSlider.addListener(this);
    addAndMakeVisible(grainSpeedRandomnessLabel);
    grainSpeedRandomnessLabel.setText("Grain Speed Random Offset +/-", juce::dontSendNotification);
    grainSpeedRandomnessLabel.attachToComponent(&grainSpeedRandomnessSlider, false);
    grainSpeedRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainPanRandomnessSlider);
    grainPanRandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainPanRandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainPanRandomnessSlider.setRange(0, 50, 1);
    grainPanRandomnessSlider.setValue(0);
    grainPanRandomnessSlider.addListener(this);
    addAndMakeVisible(grainPanRandomnessLabel);
    grainPanRandomnessLabel.setText("Grain Pan Random Offset +/-", juce::dontSendNotification);
    grainPanRandomnessLabel.attachToComponent(&grainPanRandomnessSlider, false);
    grainPanRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainVolumeRandomnessSlider);
    grainVolumeRandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainVolumeRandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainVolumeRandomnessSlider.setRange(0, 100, 1);
    grainVolumeRandomnessSlider.setValue(0);
    grainVolumeRandomnessSlider.addListener(this);
    addAndMakeVisible(grainVolumeRandomnessLabel);
    grainVolumeRandomnessLabel.setText("Grain Volume Random Offset +/-", juce::dontSendNotification);
    grainVolumeRandomnessLabel.attachToComponent(&grainVolumeRandomnessSlider, false);
    grainVolumeRandomnessLabel.setJustificationType(juce::Justification::centred);
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
    // 16:10 setup
    float getW = (getWidth() / 16);
    float getH = (getHeight() / 10);

    // Load Component
    loadComponent.setBounds(0, 0, getW * 16, getH * 5);
    // Keyboard Component
    keyboardComponent.setBounds(getW, getH * 6, getW * 6, getH * 3);

    // Grains per second
    grainsPerSecondSlider.setBounds(getW * 7, getH * 6, getW * 2, getH * 2);

    // Grain Parameters
    grainVolumeSlider.setBounds(getW * 11, getH * 6, getW * 1, getH * 1);
    grainLengthSlider.setBounds(getW * 12, getH * 6, getW * 1, getH * 1);
    grainSpeedSlider.setBounds(getW * 13, getH * 6, getW * 1, getH * 1);
    grainPanSlider.setBounds(getW * 14, getH * 6, getW * 1, getH * 1);

    // Grain Randomization
    grainVolumeRandomnessSlider.setBounds(getW * 11, getH * 8, getW * 1, getH * 1);
    grainLengthRandomnessSlider.setBounds(getW * 12, getH * 8, getW * 1, getH * 1);
    grainSpeedRandomnessSlider.setBounds(getW * 13, getH * 8, getW * 1, getH * 1);
    grainPanRandomnessSlider.setBounds(getW * 14, getH * 8, getW * 1, getH * 1);
}

void MainGUI::buttonClicked(juce::Button *button)
{
}

void MainGUI::sliderValueChanged(juce::Slider *slider)
{
    // Grains per second
    if (slider == &grainsPerSecondSlider)
    {
        processor.setGrainsPerSecond(grainsPerSecondSlider.getValue());
    }

    // Grain Parameters
    else if (slider == &grainLengthSlider || slider == &grainPanSlider || slider == &grainSpeedSlider || slider == &grainVolumeSlider)
    {
        processor.setGrainParameters(grainVolumeSlider.getValue(), grainLengthSlider.getValue(), grainPanSlider.getValue(), grainSpeedSlider.getValue());
    }

    // Grain Randomization
    else if (slider == &grainVolumeRandomnessSlider || slider == &grainPanRandomnessSlider || slider == &grainSpeedRandomnessSlider || slider == &grainLengthRandomnessSlider)
    {
        processor.setRandomParameters(grainVolumeRandomnessSlider.getValue(), grainLengthRandomnessSlider.getValue(), grainSpeedRandomnessSlider.getValue(), grainPanRandomnessSlider.getValue());
    }
}
