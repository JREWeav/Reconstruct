#pragma once

#include <JuceHeader.h>
#include "EnvelopeGUI.h"
#include "../AudioProcessing/GranularEngine.h"

class MainGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::ChangeListener
{
public:
    MainGUI(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, GranularEngine &g);
    ~MainGUI() override;

    void paint(juce::Graphics &) override;
    void resized() override;
    void buttonClicked(Button *) override;
    void sliderValueChanged(Slider *) override;

    void changeListenerCallback(ChangeBroadcaster *source) override;

private:
    // Look and feel
    const int numVoices = 8;
    URL audioURL;
    GranularEngine &engine;

    // Grains per second
    juce::Slider grainsPerSecondSlider;
    juce::Label grainsPerSecondLabel;

    // Grain parameters
    juce::Slider grainLengthSlider;
    juce::Label grainLengthLabel;
    juce::Slider grainSpeedSlider;
    juce::Label grainSpeedLabel;
    juce::Slider grainPanSlider;
    juce::Label grainPanLabel;
    juce::Slider grainVolumeSlider;
    juce::Label grainVolumeLabel;

    // Grain Randomness
    juce::Slider grainLengthRandomnessSlider;
    juce::Label grainLengthRandomnessLabel;
    juce::ToggleButton grainLengthRandomnessPlusToggle;
    juce::ToggleButton grainLengthRandomnessPlusMinusToggle;
    juce::ToggleButton grainLengthRandomnessMinusToggle;
    int grainLengthRandomState;

    juce::Slider grainSpeedRandomnessSlider;
    juce::Label grainSpeedRandomnessLabel;
    juce::ToggleButton grainSpeedRandomnessPlusToggle;
    juce::ToggleButton grainSpeedRandomnessPlusMinusToggle;
    juce::ToggleButton grainSpeedRandomnessMinusToggle;
    int grainSpeedRandomState;

    juce::Slider grainPanRandomnessSlider;
    juce::Label grainPanRandomnessLabel;
    juce::ToggleButton grainPanRandomnessPlusToggle;
    juce::ToggleButton grainPanRandomnessPlusMinusToggle;
    juce::ToggleButton grainPanRandomnessMinusToggle;
    int grainPanRandomState;

    juce::Slider grainVolumeRandomnessSlider;
    juce::Label grainVolumeRandomnessLabel;
    juce::ToggleButton grainVolumeRandomnessPlusToggle;
    juce::ToggleButton grainVolumeRandomnessPlusMinusToggle;
    juce::ToggleButton grainVolumeRandomnessMinusToggle;
    int grainVolumeRandomState;

    // Envelope
    EnvelopeGUI envelope;

    // Radio buttons
    enum RadioButtonIds
    {
        grainVolume = 1001,
        grainLength = 1002,
        grainSpeed = 1003,
        grainPan = 1004
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainGUI)
};