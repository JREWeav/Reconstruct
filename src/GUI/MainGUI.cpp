#include "MainGUI.h"

/*
TODO:
*/

MainGUI::MainGUI(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, GranularEngine &g) : engine{g},
                                                                                                              grainVolumeRandomnessSlider{engine.vts, "GRAIN_VOLUME_RANDOMNESS"},
                                                                                                              grainLengthRandomnessSlider{engine.vts, "GRAIN_LENGTH_RANDOMNESS"},
                                                                                                              grainSpeedRandomnessSlider{engine.vts, "GRAIN_SPEED_RANDOMNESS"},
                                                                                                              grainPanRandomnessSlider{engine.vts, "GRAIN_PAN_RANDOMNESS"},
                                                                                                              envelope{engine.vts}
{
    // Grains per second
    addAndMakeVisible(grainDensitySlider);
    grainDensitySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainDensitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainDensityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(engine.vts, "GRAIN_DENSITY", grainDensitySlider);
    grainDensitySlider.addListener(this);
    grainDensitySlider.setValue(engine.vts.getRawParameterValue("GRAIN_DENSITY")->load());
    addAndMakeVisible(grainDensityLabel);
    grainDensityLabel.setText("Grain Density", juce::dontSendNotification);
    grainDensityLabel.attachToComponent(&grainDensitySlider, false);
    grainDensityLabel.setJustificationType(juce::Justification::centred);

    // Grain parameters
    addAndMakeVisible(grainVolumeSlider);
    grainVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(engine.vts, "GRAIN_VOLUME", grainVolumeSlider);
    grainVolumeSlider.addListener(this);
    grainVolumeSlider.setValue(engine.vts.getRawParameterValue("GRAIN_VOLUME")->load());
    addAndMakeVisible(grainVolumeLabel);
    grainVolumeLabel.setText("Grain Volume", juce::dontSendNotification);
    grainVolumeLabel.attachToComponent(&grainVolumeSlider, false);
    grainVolumeLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainLengthSlider);
    grainLengthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainLengthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainLengthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(engine.vts, "GRAIN_LENGTH", grainLengthSlider);
    grainLengthSlider.addListener(this);
    grainLengthSlider.setValue(engine.vts.getRawParameterValue("GRAIN_LENGTH")->load());
    addAndMakeVisible(grainLengthLabel);
    grainLengthLabel.setText("Grain length (ms)", juce::dontSendNotification);
    grainLengthLabel.attachToComponent(&grainLengthSlider, false);
    grainLengthLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainSpeedSlider);
    grainSpeedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainSpeedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainSpeedAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(engine.vts, "GRAIN_SPEED", grainSpeedSlider);
    grainSpeedSlider.addListener(this);
    grainSpeedSlider.setValue(engine.vts.getRawParameterValue("GRAIN_SPEED")->load());
    addAndMakeVisible(grainSpeedLabel);
    grainSpeedLabel.setText("Grain speed (%)", juce::dontSendNotification);
    grainSpeedLabel.attachToComponent(&grainSpeedSlider, false);
    grainSpeedLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainPanSlider);
    grainPanSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    grainPanSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(engine.vts, "GRAIN_PAN", grainPanSlider);
    grainPanSlider.addListener(this);
    grainPanSlider.setValue(engine.vts.getRawParameterValue("GRAIN_PAN")->load());
    addAndMakeVisible(grainPanLabel);
    grainPanLabel.setText("Grain pan (%)", juce::dontSendNotification);
    grainPanLabel.attachToComponent(&grainPanSlider, false);
    grainPanLabel.setJustificationType(juce::Justification::centred);

    // Grain Randomization

    addAndMakeVisible(grainVolumeRandomnessSlider);
    grainVolumeRandomnessSlider.addChangeListener(this);
    grainVolumeRandomnessSlider.setValue(engine.vts.getRawParameterValue("GRAIN_VOLUME_RANDOMNESS")->load());
    addAndMakeVisible(grainVolumeRandomnessLabel);
    grainVolumeRandomnessLabel.setText("Volume Random Offset", juce::dontSendNotification);
    grainVolumeRandomnessLabel.attachToComponent(&grainVolumeRandomnessSlider, false);
    grainVolumeRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainLengthRandomnessSlider);
    grainLengthRandomnessSlider.addChangeListener(this);
    grainLengthRandomnessSlider.setValue(engine.vts.getRawParameterValue("GRAIN_LENGTH_RANDOMNESS")->load());
    addAndMakeVisible(grainLengthRandomnessLabel);
    grainLengthRandomnessLabel.setText("Length Random Offset", juce::dontSendNotification);
    grainLengthRandomnessLabel.attachToComponent(&grainLengthRandomnessSlider, false);
    grainLengthRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainSpeedRandomnessSlider);
    grainSpeedRandomnessSlider.addChangeListener(this);
    grainSpeedRandomnessSlider.setValue(engine.vts.getRawParameterValue("GRAIN_SPEED_RANDOMNESS")->load());
    addAndMakeVisible(grainSpeedRandomnessLabel);
    grainSpeedRandomnessLabel.setText("Speed Random Offset", juce::dontSendNotification);
    grainSpeedRandomnessLabel.attachToComponent(&grainSpeedRandomnessSlider, false);
    grainSpeedRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainPanRandomnessSlider);
    grainPanRandomnessSlider.addChangeListener(this);
    grainPanRandomnessSlider.setValue(engine.vts.getRawParameterValue("GRAIN_PAN_RANDOMNESS")->load());
    addAndMakeVisible(grainPanRandomnessLabel);
    grainPanRandomnessLabel.setText("Pan Random Offset", juce::dontSendNotification);
    grainPanRandomnessLabel.attachToComponent(&grainPanRandomnessSlider, false);
    grainPanRandomnessLabel.setJustificationType(juce::Justification::centred);

    // Envelope
    addAndMakeVisible(envelope);
    envelope.addChangeListener(this);

    // Update Engine
    engine.setGrainDensity(grainDensitySlider.getValue());
    engine.setGrainVolume(grainVolumeSlider.getValue());
    engine.setGrainLengthInMs(grainLengthSlider.getValue());
    engine.setGrainSpeed(grainSpeedSlider.getValue());
    engine.setGrainPan(grainPanSlider.getValue());
    engine.setRandomGrainVolume(grainVolumeRandomnessSlider.getValue(), grainVolumeRandomnessSlider.getToggleState());
    engine.setRandomGrainLengthInMs(grainLengthRandomnessSlider.getValue(), grainLengthRandomnessSlider.getToggleState());
    engine.setRandomGrainSpeed(grainSpeedRandomnessSlider.getValue(), grainSpeedRandomnessSlider.getToggleState());
    engine.setRandomGrainPan(grainPanRandomnessSlider.getValue(), grainPanRandomnessSlider.getToggleState());
    engine.setEnvelopeParameters(envelope.getEnvelopeType(), envelope.getAttack(), envelope.getPeak(), envelope.getDecay(), envelope.getSustain(), envelope.getRelease());
}

MainGUI::~MainGUI()
{
    envelope.removeChangeListener(this);
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
    grainDensitySlider.setBounds(getW * 2, getH * 3, getW * 4, getH * 4);

    // Grain Parameters
    grainVolumeSlider.setBounds(getW * 8, getH * 1, getW * 3, getH * 3);
    grainLengthSlider.setBounds(getW * 12, getH * 1, getW * 3, getH * 3);
    grainSpeedSlider.setBounds(getW * 16, getH * 1, getW * 3, getH * 3);
    grainPanSlider.setBounds(getW * 20, getH * 1, getW * 3, getH * 3);

    // Grain Randomization
    grainVolumeRandomnessSlider.setBounds(getW * 8, getH * 6, getW * 3, getH * 4);
    grainLengthRandomnessSlider.setBounds(getW * 12, getH * 6, getW * 3, getH * 4);
    grainSpeedRandomnessSlider.setBounds(getW * 16, getH * 6, getW * 3, getH * 4);
    grainPanRandomnessSlider.setBounds(getW * 20, getH * 6, getW * 3, getH * 4);

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
    // Envelope
    if (source == &envelope)
    {
        engine.setEnvelopeParameters(envelope.getEnvelopeType(), envelope.getAttack(), envelope.getPeak(), envelope.getDecay(), envelope.getSustain(), envelope.getRelease());
        repaint();
        resized();
    }
    // Grain Randomization
    else if (source == &grainVolumeRandomnessSlider)
    {
        engine.setRandomGrainVolume(grainVolumeRandomnessSlider.getValue(), grainVolumeRandomnessSlider.getToggleState());
    }
    else if (source == &grainPanRandomnessSlider)
    {
        engine.setRandomGrainPan(grainPanRandomnessSlider.getValue(), grainPanRandomnessSlider.getToggleState());
    }
    else if (source == &grainSpeedRandomnessSlider)
    {
        engine.setRandomGrainSpeed(grainSpeedRandomnessSlider.getValue(), grainSpeedRandomnessSlider.getToggleState());
    }
    else if (source == &grainLengthRandomnessSlider)
    {
        engine.setRandomGrainLengthInMs(grainLengthRandomnessSlider.getValue(), grainLengthRandomnessSlider.getToggleState());
    }
}

void MainGUI::buttonClicked(juce::Button *button)
{
}

void MainGUI::sliderValueChanged(juce::Slider *slider)
{
    // Grains per second
    if (slider == &grainDensitySlider)
    {
        engine.setGrainDensity((float)grainDensitySlider.getValue());
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
}
