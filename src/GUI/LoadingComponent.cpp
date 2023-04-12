#include "LoadingComponent.h"

LoadingComponent::LoadingComponent(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, AudioPluginAudioProcessor &p) : waveForm{formatManager, thumbnailCache}, processor(p)
{
    addAndMakeVisible(loadButton);
    addAndMakeVisible(waveForm);
    loadButton.addListener(this);
    waveForm.addChangeListener(this);
}

LoadingComponent::~LoadingComponent()
{
}

void LoadingComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
}

void LoadingComponent::resized()
{
    loadButton.setBounds(10, 10, 100, 20);
    waveForm.setBounds(10, 40, getWidth() - 20, getHeight() - 50);
}

void LoadingComponent::buttonClicked(juce::Button *button)
{
    if (button == &loadButton)
    {
        loadFile();
    }
}

void LoadingComponent::loadFile()
{
    if (fileChooser.browseForFileToOpen())
    {
        auto *input = new juce::URLInputSource(fileChooser.getURLResult());
        waveForm.loadAudio(input);
        auto fileURL = fileChooser.getURLResult();
        processor.loadSampleFromUrl(fileURL);
    }
}

void LoadingComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &waveForm)
    {
        waveForm.setRelativePosition(waveForm.getLastRelativeClick());
        if (waveForm.isLooping())
        {
            processor.setSampleParameters((float)waveForm.getLastRelativeClick(), ((float)waveForm.getLastRelativeClick() + (float)waveForm.getRelativeLoopLength()));
        }
        else
        {
            processor.setSampleParameters((float)waveForm.getLastRelativeClick(), (float)waveForm.getLastRelativeClick());
        }
    }
}
