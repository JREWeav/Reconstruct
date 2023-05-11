#include "MainGUI.h"

MainGUI::MainGUI(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, GranularEngine &g, String indexOfSelf) : engine{g},
                                                                                                                                  indexOfSelf(indexOfSelf),
                                                                                                                                  grainVolumeRandomnessSlider{engine.granularVTS, "GRAIN_VOLUME_RANDOMNESS" + indexOfSelf, "GRAIN_VOLUME_DIRECTION" + indexOfSelf, [](double value)
                                                                                                                                                              {
                                                                                                                                                                  return juce::String((int)ceil(value)) + "%";
                                                                                                                                                              }},
                                                                                                                                  grainLengthRandomnessSlider{engine.granularVTS, "GRAIN_LENGTH_RANDOMNESS" + indexOfSelf, "GRAIN_LENGTH_DIRECTION" + indexOfSelf, [](double value)
                                                                                                                                                              {
                                                                                                                                                                  return juce::String((int)ceil(value)) + "ms";
                                                                                                                                                              }},
                                                                                                                                  grainSpeedRandomnessSlider{engine.granularVTS, "GRAIN_SPEED_RANDOMNESS" + indexOfSelf, "GRAIN_SPEED_DIRECTION" + indexOfSelf, [](double value)
                                                                                                                                                             {
                                                                                                                                                                 return juce::String((int)ceil(value)) + "%";
                                                                                                                                                             }},
                                                                                                                                  grainPanRandomnessSlider{engine.granularVTS, "GRAIN_PAN_RANDOMNESS" + indexOfSelf, "GRAIN_PAN_DIRECTION" + indexOfSelf, [](double value)
                                                                                                                                                           {
                                                                                                                                                               return "L " + juce::String((int)ceil(value)) + " R";
                                                                                                                                                           }},
                                                                                                                                  envelope{engine.granularVTS, indexOfSelf}
{
    // Toggle button
    addAndMakeVisible(onOffButton);
    onOffButton.addListener(this);
    onOffButton.setToggleState(engine.granularVTS.getRawParameterValue("ON_OFF" + indexOfSelf)->load(), juce::dontSendNotification);
    addAndMakeVisible(onOffLabel);
    onOffLabel.setText("On/Off", juce::dontSendNotification);
    onOffLabel.attachToComponent(&onOffButton, false);
    onOffLabel.setJustificationType(juce::Justification::centred);

    // Grains per second
    addAndMakeVisible(grainDensitySlider);
    grainDensitySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    grainDensitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    DBG("GRAIN_DENSITY" + indexOfSelf);
    grainDensityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(engine.granularVTS, "GRAIN_DENSITY" + indexOfSelf, grainDensitySlider);
    grainDensitySlider.addListener(this);
    grainDensitySlider.setValue(engine.granularVTS.getRawParameterValue("GRAIN_DENSITY" + indexOfSelf)->load());
    grainDensitySlider.textFromValueFunction = [](double value)
    {
        return juce::String(value, 2);
    };
    grainDensitySlider.updateText();
    addAndMakeVisible(grainDensityLabel);
    grainDensityLabel.setText("Grain Density", juce::dontSendNotification);
    grainDensityLabel.attachToComponent(&grainDensitySlider, false);
    grainDensityLabel.setJustificationType(juce::Justification::centred);

    // Grain parameters
    addAndMakeVisible(grainVolumeSlider);
    grainVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    grainVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(engine.granularVTS, "GRAIN_VOLUME" + indexOfSelf, grainVolumeSlider);
    grainVolumeSlider.addListener(this);
    grainVolumeSlider.setValue(engine.granularVTS.getRawParameterValue("GRAIN_VOLUME" + indexOfSelf)->load());
    grainVolumeSlider.textFromValueFunction = [](double value)
    {
        return juce::String((int)ceil(value)) + "%";
    };
    grainVolumeSlider.updateText();
    addAndMakeVisible(grainVolumeLabel);
    grainVolumeLabel.setText("Grain Volume", juce::dontSendNotification);
    grainVolumeLabel.attachToComponent(&grainVolumeSlider, false);
    grainVolumeLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainLengthSlider);
    grainLengthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    grainLengthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainLengthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(engine.granularVTS, "GRAIN_LENGTH" + indexOfSelf, grainLengthSlider);
    grainLengthSlider.addListener(this);
    grainLengthSlider.setValue(engine.granularVTS.getRawParameterValue("GRAIN_LENGTH" + indexOfSelf)->load());
    grainLengthSlider.textFromValueFunction = [](double value)
    {
        return juce::String((int)ceil(value)) + "ms";
    };
    grainLengthSlider.updateText();
    addAndMakeVisible(grainLengthLabel);
    grainLengthLabel.setText("Grain Length", juce::dontSendNotification);
    grainLengthLabel.attachToComponent(&grainLengthSlider, false);
    grainLengthLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainSpeedSlider);
    grainSpeedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    grainSpeedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainSpeedAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(engine.granularVTS, "GRAIN_SPEED" + indexOfSelf, grainSpeedSlider);
    grainSpeedSlider.addListener(this);
    grainSpeedSlider.setValue(engine.granularVTS.getRawParameterValue("GRAIN_SPEED" + indexOfSelf)->load());
    grainSpeedSlider.textFromValueFunction = [](double value)
    {
        return juce::String((int)ceil(value)) + "%";
    };
    grainSpeedSlider.updateText();
    addAndMakeVisible(grainSpeedLabel);
    grainSpeedLabel.setText("Grain Speed", juce::dontSendNotification);
    grainSpeedLabel.attachToComponent(&grainSpeedSlider, false);
    grainSpeedLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainPanSlider);
    grainPanSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    grainPanSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    grainPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(engine.granularVTS, "GRAIN_PAN" + indexOfSelf, grainPanSlider);
    grainPanSlider.addListener(this);
    grainPanSlider.setValue(engine.granularVTS.getRawParameterValue("GRAIN_PAN" + indexOfSelf)->load());
    grainPanSlider.textFromValueFunction = [](double value)
    {
        return "L  " + juce::String((int)ceil(value)) + "  R";
    };
    grainPanSlider.updateText();
    addAndMakeVisible(grainPanLabel);
    grainPanLabel.setText("Grain Pan", juce::dontSendNotification);
    grainPanLabel.attachToComponent(&grainPanSlider, false);
    grainPanLabel.setJustificationType(juce::Justification::centred);

    // Grain Randomization

    addAndMakeVisible(grainVolumeRandomnessSlider);
    grainVolumeRandomnessSlider.addChangeListener(this);
    grainVolumeRandomnessSlider.setValue(engine.granularVTS.getRawParameterValue("GRAIN_VOLUME_RANDOMNESS" + indexOfSelf)->load());
    grainVolumeRandomnessSlider.updateText();
    addAndMakeVisible(grainVolumeRandomnessLabel);
    grainVolumeRandomnessLabel.setText("Volume Offset", juce::dontSendNotification);
    grainVolumeRandomnessLabel.attachToComponent(&grainVolumeRandomnessSlider, false);
    grainVolumeRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainLengthRandomnessSlider);
    grainLengthRandomnessSlider.addChangeListener(this);
    grainLengthRandomnessSlider.setValue(engine.granularVTS.getRawParameterValue("GRAIN_LENGTH_RANDOMNESS" + indexOfSelf)->load());
    grainLengthRandomnessSlider.updateText();
    addAndMakeVisible(grainLengthRandomnessLabel);
    grainLengthRandomnessLabel.setText("Length Offset", juce::dontSendNotification);
    grainLengthRandomnessLabel.attachToComponent(&grainLengthRandomnessSlider, false);
    grainLengthRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainSpeedRandomnessSlider);
    grainSpeedRandomnessSlider.addChangeListener(this);
    grainSpeedRandomnessSlider.setValue(engine.granularVTS.getRawParameterValue("GRAIN_SPEED_RANDOMNESS" + indexOfSelf)->load());
    grainSpeedRandomnessSlider.updateText();
    addAndMakeVisible(grainSpeedRandomnessLabel);
    grainSpeedRandomnessLabel.setText("Speed Offset", juce::dontSendNotification);
    grainSpeedRandomnessLabel.attachToComponent(&grainSpeedRandomnessSlider, false);
    grainSpeedRandomnessLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(grainPanRandomnessSlider);
    grainPanRandomnessSlider.addChangeListener(this);
    grainPanRandomnessSlider.setValue(engine.granularVTS.getRawParameterValue("GRAIN_PAN_RANDOMNESS" + indexOfSelf)->load());
    grainPanRandomnessSlider.updateText();
    addAndMakeVisible(grainPanRandomnessLabel);
    grainPanRandomnessLabel.setText("Pan Offset", juce::dontSendNotification);
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

    // on / off
    onOffButton.setBounds(getW * 2, getH, getW * 2, getH);

    // Grains per second
    grainDensitySlider.setBounds(getW * 2, getH * 3, getW * 4, getH * 4);

    // Grain Parameters
    grainVolumeSlider.setBounds(getW * 8, getH * 1, getW * 3, getH * 3);
    grainLengthSlider.setBounds(getW * 12, getH * 1, getW * 3, getH * 3);
    grainSpeedSlider.setBounds(getW * 16, getH * 1, getW * 3, getH * 3);
    grainPanSlider.setBounds(getW * 20, getH * 1, getW * 3, getH * 3);

    // Grain Randomization
    grainVolumeRandomnessSlider.setBounds(getW * 8, getH * 6, getW * 3, getH * 5);
    grainLengthRandomnessSlider.setBounds(getW * 12, getH * 6, getW * 3, getH * 5);
    grainSpeedRandomnessSlider.setBounds(getW * 16, getH * 6, getW * 3, getH * 5);
    grainPanRandomnessSlider.setBounds(getW * 20, getH * 6, getW * 3, getH * 5);

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
    if (button == &onOffButton)
    {
        engine.setEngineOn(onOffButton.getToggleState());
        engine.granularVTS.getParameter("ON_OFF" + indexOfSelf)->beginChangeGesture();
        engine.granularVTS.getParameter("ON_OFF" + indexOfSelf)->setValueNotifyingHost(onOffButton.getToggleState());
        engine.granularVTS.getParameter("ON_OFF" + indexOfSelf)->endChangeGesture();
    }
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
