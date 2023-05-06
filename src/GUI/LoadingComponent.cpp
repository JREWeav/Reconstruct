#include "LoadingComponent.h"

LoadingComponent::LoadingComponent(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, GranularEngine *g) : waveForm{formatManager, thumbnailCache}, engines(g)
{
    addAndMakeVisible(loadButton);
    addAndMakeVisible(waveForm);
    loadButton.addListener(this);
    waveForm.addChangeListener(this);
    engines->addChangeListener(this);

    // Loop start slider
    addAndMakeVisible(loopStartSlider);
    loopStartSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    loopStartSlider.addListener(this);
    loopStartAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(engines->vts, "SAMPLE_START", loopStartSlider);

    // Loop end slider
    addAndMakeVisible(loopEndSlider);
    loopEndSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    loopEndSlider.addListener(this);
    loopEndAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(engines->vts, "SAMPLE_END", loopEndSlider);

    if (engines->isFileLoaded())
    {
        waveForm.loadAudio(engines->getSampleBuffer(), engines->getStoredSampleRate());
        waveForm.setRelativePosition(loopStartSlider.getValue());
        waveForm.setRelativeClick(loopStartSlider.getValue());
        if (loopEndSlider.getValue() == loopStartSlider.getValue())
        {
            waveForm.setLooping(false);
        }
        else
        {
            waveForm.setLooping(true);
            loopStartSlider.setRange(0.0f, loopEndSlider.getValue(), 0.01f);
            waveForm.setRelativeLoopLength(loopEndSlider.getValue() - waveForm.getLastRelativeClick());
        }
    }
}

LoadingComponent::~LoadingComponent()
{
    engines->removeChangeListener(this);
    waveForm.removeChangeListener(this);
}

void LoadingComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::dimgrey);
}

void LoadingComponent::resized()
{
    loadButton.setBounds(10, 10, 100, 20);

    loopStartSlider.setBounds(10, 30, 100, 15);
    loopEndSlider.setBounds(110, 30, 100, 15);

    waveForm.setBounds(10, 45, getWidth() - 20, getHeight() - 50);
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
        auto fileURL = fileChooser.getURLResult();
        engines->loadSampleFromUrl(fileURL);
        auto *input = new juce::URLInputSource(fileChooser.getURLResult());
        waveForm.loadAudio(input);
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
            loopStartSlider.setValue(waveForm.getLastRelativeClick());
            loopEndSlider.setValue(waveForm.getLastRelativeClick() + waveForm.getRelativeLoopLength());
        }
        else
        {
            engines->setRelativeSampleStart((float)waveForm.getLastRelativeClick());
            engines->setRelativeSampleEnd((float)waveForm.getLastRelativeClick());
            loopStartSlider.setValue(waveForm.getLastRelativeClick());
            loopEndSlider.setValue(waveForm.getLastRelativeClick());
        }
    }
    if (source == engines)
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
}

void LoadingComponent::sliderValueChanged(Slider *slider)
{
    if (slider == &loopStartSlider)
    {
        waveForm.setRelativePosition(loopStartSlider.getValue());
        waveForm.setRelativeClick(loopStartSlider.getValue());
        loopEndSlider.setRange(waveForm.getLastRelativeClick(), 1.0f, 0.01f);
        if (waveForm.isLooping())
        {
            waveForm.setRelativeLoopLength(loopEndSlider.getValue() - waveForm.getLastRelativeClick());
        }
    }
    else if (slider == &loopEndSlider)
    {
        if (loopEndSlider.getValue() == loopStartSlider.getValue())
        {
            waveForm.setLooping(false);
        }
        else
        {
            waveForm.setLooping(true);
            loopStartSlider.setRange(0.0f, loopEndSlider.getValue(), 0.01f);
            waveForm.setRelativeLoopLength(loopEndSlider.getValue() - waveForm.getLastRelativeClick());
        }
    }
}