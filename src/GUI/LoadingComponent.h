#pragma once

#include <JuceHeader.h>
#include "AudioWaveform.h"
#include "../AudioProcessing/PluginProcessor.h"

class LoadingComponent : public juce::Component,
                         public juce::Button::Listener
{
public:
    LoadingComponent(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, AudioPluginAudioProcessor &p);
    ~LoadingComponent() override;

    void paint(juce::Graphics &) override;
    void resized() override;
    void loadFile();
    void buttonClicked(Button *) override;

private:
    juce::FileChooser fileChooser{"Browse for sample to open..."};
    juce::TextButton loadButton{"Load File"};
    AudioWaveform waveForm;
    AudioPluginAudioProcessor &processor;
};