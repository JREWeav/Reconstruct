#pragma once

#include <JuceHeader.h>
#include "Components/AudioWaveform.h"
#include "../AudioProcessing/GranularEngine.h"

class LoadingComponent : public juce::Component,
                         public juce::Button::Listener,
                         public juce::Slider::Listener,
                         public juce::ChangeListener

{
public:
    LoadingComponent(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, GranularEngine *g);
    ~LoadingComponent() override;

    void paint(juce::Graphics &) override;
    void resized() override;
    void loadFile();
    void buttonClicked(Button *) override;

    // Slider logic
    void sliderValueChanged(Slider *slider) override;

    void changeListenerCallback(ChangeBroadcaster *source) override;

private:
    juce::FileChooser fileChooser{"Browse for sample to open..."};
    juce::TextButton loadButton{"Load File"};
    AudioWaveform waveForm;
    GranularEngine *engines;

    Slider loopStartSlider;
    Slider loopEndSlider;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> loopStartAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> loopEndAttachment;
};