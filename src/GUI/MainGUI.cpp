#include "MainGUI.h"

/*
TODO:
*/

MainGUI::MainGUI(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, GranularEngine &g) : engine{g}
{
    // Loading component

    // Grains per second
    addAndMakeVisible(grainsPerSecondSlider);
    grainsPerSecondSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainsPerSecondSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainsPerSecondSlider.setRange(0.0f, 500.0f, 0.1f);
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
    grainLengthSlider.setRange(1, 500, 1);
    grainLengthSlider.setValue(100);
    grainLengthSlider.addListener(this);
    addAndMakeVisible(grainLengthLabel);
    grainLengthLabel.setText("Grain length (ms)", juce::dontSendNotification);
    grainLengthLabel.attachToComponent(&grainLengthSlider, false);
    grainLengthLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainSpeedSlider);
    grainSpeedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainSpeedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainSpeedSlider.setRange(0, 400, 1);
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
    grainLengthRandomnessSlider.setRange(0, 200, 1);
    grainLengthRandomnessSlider.setValue(0);
    grainLengthRandomnessSlider.addListener(this);
    addAndMakeVisible(grainLengthRandomnessLabel);
    grainLengthRandomnessLabel.setText("Length Random Offset", juce::dontSendNotification);
    grainLengthRandomnessLabel.attachToComponent(&grainLengthRandomnessSlider, false);
    grainLengthRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainSpeedRandomnessSlider);
    grainSpeedRandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainSpeedRandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainSpeedRandomnessSlider.setRange(0, 300, 1);
    grainSpeedRandomnessSlider.setValue(0);
    grainSpeedRandomnessSlider.addListener(this);
    addAndMakeVisible(grainSpeedRandomnessLabel);
    grainSpeedRandomnessLabel.setText("Speed Random Offset", juce::dontSendNotification);
    grainSpeedRandomnessLabel.attachToComponent(&grainSpeedRandomnessSlider, false);
    grainSpeedRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainPanRandomnessSlider);
    grainPanRandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainPanRandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainPanRandomnessSlider.setRange(0, 50, 1);
    grainPanRandomnessSlider.setValue(0);
    grainPanRandomnessSlider.addListener(this);
    addAndMakeVisible(grainPanRandomnessLabel);
    grainPanRandomnessLabel.setText("Pan Random Offset", juce::dontSendNotification);
    grainPanRandomnessLabel.attachToComponent(&grainPanRandomnessSlider, false);
    grainPanRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainVolumeRandomnessSlider);
    grainVolumeRandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainVolumeRandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainVolumeRandomnessSlider.setRange(0, 100, 1);
    grainVolumeRandomnessSlider.setValue(0);
    grainVolumeRandomnessSlider.addListener(this);
    addAndMakeVisible(grainVolumeRandomnessLabel);
    grainVolumeRandomnessLabel.setText("Volume Random Offset", juce::dontSendNotification);
    grainVolumeRandomnessLabel.attachToComponent(&grainVolumeRandomnessSlider, false);
    grainVolumeRandomnessLabel.setJustificationType(juce::Justification::centred);

    // Envelope
    addAndMakeVisible(envelope);
    envelope.addChangeListener(this);
}

MainGUI::~MainGUI()
{
}

void MainGUI::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::dimgrey);
    g.setColour(juce::Colours::aliceblue);
}

void MainGUI::resized()
{
    // 16:10 setup
    int getW = (getWidth() / 32);
    int getH = (getHeight() / 10);

    // Grains per second
    grainsPerSecondSlider.setBounds(getW * 2, getH * 3, getW * 4, getH * 4);

    // Grain Parameters
    grainVolumeSlider.setBounds(getW * 8, getH * 1, getW * 3, getH * 3);
    grainLengthSlider.setBounds(getW * 12, getH * 1, getW * 3, getH * 3);
    grainSpeedSlider.setBounds(getW * 16, getH * 1, getW * 3, getH * 3);
    grainPanSlider.setBounds(getW * 20, getH * 1, getW * 3, getH * 3);

    // Grain Randomization
    grainVolumeRandomnessSlider.setBounds(getW * 8, getH * 6, getW * 3, getH * 3);
    grainLengthRandomnessSlider.setBounds(getW * 12, getH * 6, getW * 3, getH * 3);
    grainSpeedRandomnessSlider.setBounds(getW * 16, getH * 6, getW * 3, getH * 3);
    grainPanRandomnessSlider.setBounds(getW * 20, getH * 6, getW * 3, getH * 3);

    // Envelope
    if (envelope.getCollapseState())
    {
        envelope.setBounds(getW * 24, getH * 3, getW * 7, getH * 4);
    }
    else
    {
        envelope.setBounds(getW * 8, getH * 1, getW * 15, getH * 7);
    }
}

void MainGUI::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &envelope)
    {
        engine.setEnvelopeParameters(envelope.getEnvelopeType(), envelope.getAttack(), envelope.getPeak(), envelope.getDecay(), envelope.getSustain(), envelope.getRelease());
        repaint();
        resized();
    }
}

void MainGUI::buttonClicked(juce::Button *button)
{
}

void MainGUI::sliderValueChanged(juce::Slider *slider)
{
    // Grains per second
    if (slider == &grainsPerSecondSlider)
    {
        engine.setGrainsPerSecond((float)grainsPerSecondSlider.getValue());
    }

    // Grain Parameters
    else if (slider == &grainLengthSlider)
    {
        engine.setGrainLengthInMs(grainLengthSlider.getValue());
    }

    else if (slider == &grainPanSlider)
    {
        engine.setGrainPan(grainPanSlider.getValue());
    }

    else if (slider == &grainSpeedSlider)
    {
        engine.setGrainSpeed(grainSpeedSlider.getValue());
    }

    else if (slider == &grainVolumeSlider)
    {
        engine.setGrainVolume(grainVolumeSlider.getValue());
    }

    // Grain Randomization
    else if (slider == &grainVolumeRandomnessSlider)
    {
        engine.setRandomGrainVolume(grainVolumeRandomnessSlider.getValue());
    }

    else if (slider == &grainPanRandomnessSlider)
    {
        engine.setRandomGrainPan(grainPanRandomnessSlider.getValue());
    }

    else if (slider == &grainSpeedRandomnessSlider)
    {
        engine.setRandomGrainSpeed(grainSpeedRandomnessSlider.getValue());
    }

    else if (slider == &grainLengthRandomnessSlider)
    {
        engine.setRandomGrainLengthInMs(grainLengthRandomnessSlider.getValue());
    }
}
