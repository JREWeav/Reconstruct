#pragma once

#include <JuceHeader.h>
#include "LoadingComponent.h"
#include "../AudioProcessing/PluginProcessor.h"

class MainGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener
{
public:
    MainGUI(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, AudioPluginAudioProcessor &p);
    ~MainGUI() override;

    void paint(juce::Graphics &) override;
    void resized() override;
    void buttonClicked(Button *) override;
    void sliderValueChanged(Slider *) override;

private:
    LoadingComponent loadComponent;
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent{keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard};
    const int numVoices = 8;
    URL audioURL;
    AudioPluginAudioProcessor &processor;

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
    juce::Slider grainSpeedRandomnessSlider;
    juce::Label grainSpeedRandomnessLabel;
    juce::Slider grainPanRandomnessSlider;
    juce::Label grainPanRandomnessLabel;
    juce::Slider grainVolumeRandomnessSlider;
    juce::Label grainVolumeRandomnessLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainGUI)
};