#include "../AudioProcessing/PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p, GranularEngine *granularEngines, AudioFormatManager &formatManager)
    : AudioProcessorEditor(&p), mainComponent1(formatManager, thumbnailCache, granularEngines[0], "1"), mainComponent2(formatManager, thumbnailCache, granularEngines[1], "2"), loadComponent(formatManager, thumbnailCache, granularEngines)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1600, 750);
    addAndMakeVisible(mainComponent1);
    mainComponent1.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(mainComponent2);
    mainComponent2.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(loadComponent);
    loadComponent.setLookAndFeel(&customLookAndFeel);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    mainComponent1.setLookAndFeel(nullptr);
    mainComponent2.setLookAndFeel(nullptr);
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
    mainComponent1.setBounds(0, getHeight() / 2 - 10, getWidth() / 2, getHeight() / 2);
    mainComponent2.setBounds(getWidth() / 2, getHeight() / 2 - 10, getWidth() / 2, getHeight() / 2);
}