#pragma once

#include "../AudioProcessing/PluginProcessor.h"
#include "../AudioProcessing/GranularEngine.h"
#include <JuceHeader.h>
#include "MainGUI.h"
#include "LoadingComponent.h"
#include "../LookAndFeel/ReconLookAndFeel.h"

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &, GranularEngine *, AudioFormatManager &);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReconLookAndFeel customLookAndFeel;
    AudioThumbnailCache thumbnailCache{10000};

    MainGUI mainComponent1;
    MainGUI mainComponent2;
    LoadingComponent loadComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
