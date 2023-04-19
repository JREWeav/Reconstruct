#include "LoadingComponent.h"

LoadingComponent::LoadingComponent(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, GranularEngine *g) : waveForm{formatManager, thumbnailCache}, engines(g)
{
    addAndMakeVisible(loadButton);
    addAndMakeVisible(waveForm);
    loadButton.addListener(this);
    waveForm.addChangeListener(this);
    startTimerHz(2000);
}

LoadingComponent::~LoadingComponent()
{
}

void LoadingComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::dimgrey);
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
        engines->loadSampleFromUrl(fileURL);
    }
}

void LoadingComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &waveForm)
    {
        waveForm.setRelativePosition(waveForm.getLastRelativeClick());
        if (waveForm.isLooping())
        {
            engines->setRelativeSampleStart((float)waveForm.getLastRelativeClick());
            engines->setRelativeSampleEnd((float)waveForm.getLastRelativeClick() + (float)waveForm.getRelativeLoopLength());
        }
        else
        {
            engines->setRelativeSampleStart((float)waveForm.getLastRelativeClick());
            engines->setRelativeSampleEnd((float)waveForm.getLastRelativeClick());
        }
    }
}

void LoadingComponent::timerCallback()
{
    waveForm.clearGrains();
    auto grainParameters = engines->getGrainParameters();
    repaint();
    if (grainParameters.size() == 0)
        return;

    for (int i = 0; i < grainParameters.size(); i++)
    {
        float grainCurrentPosition = std::get<0>(grainParameters[i]);
        float grainVolume = std::get<1>(grainParameters[i]);
        float grainPan = std::get<2>(grainParameters[i]);
        waveForm.addGrain(grainCurrentPosition, grainVolume, grainPan);
    }
}
