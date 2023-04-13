#include "../AudioProcessing/PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p, juce::AudioFormatManager &formatManager)
    : AudioProcessorEditor(&p), processor(p), mainComponent(formatManager, thumbnailCache, processor)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1200, 750);
    addAndMakeVisible(mainComponent);
    formatManager.registerBasicFormats();
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::dimgrey);
}

void AudioPluginAudioProcessorEditor::resized()
{
    mainComponent.setBounds(getLocalBounds());
}