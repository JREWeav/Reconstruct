#pragma once

#include <JuceHeader.h>
#include "AudioWaveform.h"
#include "../AudioProcessing/PluginProcessor.h"

class LoadingComponent : public juce::Component,
                         public juce::Button::Listener,
                         public juce::ChangeListener
{
public:
    LoadingComponent(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, AudioPluginAudioProcessor &p);
    ~LoadingComponent() override;

    void paint(juce::Graphics &) override;
    void resized() override;
    void loadFile();
    void buttonClicked(Button *) override;

    void changeListenerCallback(ChangeBroadcaster *source) override;

private:
    juce::FileChooser fileChooser{"Browse for sample to open..."};
    juce::TextButton loadButton{"Load File"};
    AudioWaveform waveForm;
    AudioPluginAudioProcessor &processor;
};