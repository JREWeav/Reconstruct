#include "../AudioProcessing/PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p, GranularEngine *granularEngines, AudioFormatManager &formatManager)
    : AudioProcessorEditor(&p), mainComponent(formatManager, thumbnailCache, granularEngines[0]), loadComponent(formatManager, thumbnailCache, granularEngines)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1200, 750);
    addAndMakeVisible(mainComponent);
    mainComponent.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(loadComponent);
    loadComponent.setLookAndFeel(&customLookAndFeel);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    mainComponent.setLookAndFeel(nullptr);
    loadComponent.setLookAndFeel(nullptr);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::dimgrey);
}

void AudioPluginAudioProcessorEditor::resized()
{
    loadComponent.setBounds(0, 0, getWidth(), getHeight() / 2);
    mainComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}