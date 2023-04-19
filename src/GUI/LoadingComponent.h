#pragma once

#include <JuceHeader.h>
#include "AudioWaveform.h"
#include "../AudioProcessing/GranularEngine.h"

class LoadingComponent : public juce::Component,
                         public juce::Button::Listener,
                         public juce::ChangeListener,
                         public juce::Timer
{
public:
    LoadingComponent(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, GranularEngine *g);
    ~LoadingComponent() override;

    void paint(juce::Graphics &) override;
    void resized() override;
    void loadFile();
    void buttonClicked(Button *) override;

    void changeListenerCallback(ChangeBroadcaster *source) override;

    // Timer
    void timerCallback() override;

private:
    juce::FileChooser fileChooser{"Browse for sample to open..."};
    juce::TextButton loadButton{"Load File"};
    AudioWaveform waveForm;
    GranularEngine *engines;
};