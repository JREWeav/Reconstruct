#include "MainGUI.h"
#include "PluginProcessor.h"

MainGUI::MainGUI(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, AudioPluginAudioProcessor &p) : loadComponent{formatManager, thumbnailCache, p}
{
    addAndMakeVisible(loadComponent);
    addAndMakeVisible(keyboardComponent);
    keyboardComponent.setMidiChannel(1);
}

MainGUI::~MainGUI()
{
}

void MainGUI::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
}

void MainGUI::resized()
{
    loadComponent.setBounds(10, 10, getWidth() - 20, getHeight() / 2 + 20);
    keyboardComponent.setBounds(10, (3 * (getHeight() / 4) - 20), getWidth() / 2 - 20, getHeight() / 4);
}

void MainGUI::buttonClicked(juce::Button *button)
{
}

void MainGUI::sliderValueChanged(juce::Slider *slider)
{
}
