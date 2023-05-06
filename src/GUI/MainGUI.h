#pragma once

#include <JuceHeader.h>
#include "Components/EnvelopeGUI.h"
#include "../AudioProcessing/GranularEngine.h"
#include "Components/RandomnessDial.h"

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
    Slider grainDensitySlider;
    Label grainDensityLabel;

    // Grain parameters
    Slider grainLengthSlider;
    Label grainLengthLabel;
    Slider grainSpeedSlider;
    Label grainSpeedLabel;
    Slider grainPanSlider;
    Label grainPanLabel;
    Slider grainVolumeSlider;
    Label grainVolumeLabel;

    // Grain Randomness

    RandomnessDial grainVolumeRandomnessSlider;
    Label grainVolumeRandomnessLabel;
    RandomnessDial grainLengthRandomnessSlider;
    Label grainLengthRandomnessLabel;
    RandomnessDial grainSpeedRandomnessSlider;
    Label grainSpeedRandomnessLabel;
    RandomnessDial grainPanRandomnessSlider;
    Label grainPanRandomnessLabel;

    // Envelope
    EnvelopeGUI envelope;

    // Slider attachments
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> grainDensityAttachment;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> grainVolumeAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> grainLengthAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> grainSpeedAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> grainPanAttachment;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> grainVolumeRandomnessAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> grainLengthRandomnessAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> grainSpeedRandomnessAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> grainPanRandomnessAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainGUI)
};