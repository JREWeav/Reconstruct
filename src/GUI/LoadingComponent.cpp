#include "LoadingComponent.h"

LoadingComponent::LoadingComponent(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, GranularEngine *g) : waveForm{formatManager, thumbnailCache}, engines(g)
{
    addAndMakeVisible(loadButton);
    addAndMakeVisible(waveForm);
    loadButton.addListener(this);
    waveForm.addChangeListener(this);
    engines[0].addChangeListener(this);
    engines[1].addChangeListener(this);

    // Loop start slider
    addAndMakeVisible(loopStartSlider);
    loopStartAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(engines->globalVTS, "SAMPLE_START", loopStartSlider);
    loopStartSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    loopStartSlider.textFromValueFunction = [](double value)
    { return String(value, 2); };
    loopStartSlider.addListener(this);
    loopStartSlider.updateText();

    // Loop end slider
    addAndMakeVisible(loopEndSlider);
    loopEndAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(engines->globalVTS, "SAMPLE_END", loopEndSlider);
    loopEndSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    loopEndSlider.textFromValueFunction = [](double value)
    { return String(value, 2); };
    loopEndSlider.addListener(this);
    loopEndSlider.updateText();

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
    engines->globalVTS.getParameter("SAMPLE_START")->beginChangeGesture();
    engines->globalVTS.getParameter("SAMPLE_START")->setValueNotifyingHost(waveForm.getLastRelativeClick());
    engines->globalVTS.getParameter("SAMPLE_START")->endChangeGesture();
    engines->globalVTS.getParameter("SAMPLE_END")->beginChangeGesture();
    engines->globalVTS.getParameter("SAMPLE_END")->setValueNotifyingHost(waveForm.getLastRelativeClick() + waveForm.getRelativeLoopLength());
    engines->globalVTS.getParameter("SAMPLE_END")->endChangeGesture();
    engines[0].removeChangeListener(this);
    engines[1].removeChangeListener(this);
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

        engines[0].loadSampleFromUrl(fileURL);
        engines[1].loadSampleFromUrl(fileURL);

        auto *input = new juce::URLInputSource(fileChooser.getURLResult());
        waveForm.loadAudio(input);
    }
}

void LoadingComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &waveForm)
    {
        if (waveForm.isLooping())
        {
            loopStartSlider.setValue(waveForm.getLastRelativeClick(), juce::dontSendNotification);
            loopEndSlider.setValue(waveForm.getLastRelativeClick() + waveForm.getRelativeLoopLength(), juce::dontSendNotification);
            engines[0].setRelativeSampleStart((float)waveForm.getLastRelativeClick());
            engines[1].setRelativeSampleStart((float)waveForm.getLastRelativeClick());
            engines[0].setRelativeSampleEnd((float)waveForm.getLastRelativeClick() + (float)waveForm.getRelativeLoopLength());
            engines[1].setRelativeSampleEnd((float)waveForm.getLastRelativeClick() + (float)waveForm.getRelativeLoopLength());
        }
        else
        {
            engines[0].setRelativeSampleStart((float)waveForm.getLastRelativeClick());
            engines[1].setRelativeSampleStart((float)waveForm.getLastRelativeClick());
            engines[0].setRelativeSampleEnd((float)waveForm.getLastRelativeClick());
            engines[1].setRelativeSampleEnd((float)waveForm.getLastRelativeClick());
            loopStartSlider.setValue(waveForm.getLastRelativeClick(), juce::dontSendNotification);
            loopEndSlider.setValue(waveForm.getLastRelativeClick(), juce::dontSendNotification);
        }
    }
    if (source == &engines[0])
    {
        auto grainParameters = engines[0].getGrainParameters();
        repaint();
        if (grainParameters.size() == 0)
            return;

        for (int i = 0; i < grainParameters.size(); i++)
        {
            float grainCurrentPosition = std::get<0>(grainParameters[i]);
            float grainVolume = std::get<1>(grainParameters[i]);
            float grainPan = std::get<2>(grainParameters[i]);
            waveForm.addGrain(grainCurrentPosition, grainVolume, grainPan, juce::Colours::goldenrod);
        }
    }
    if (source == &engines[1])
    {
        auto grainParameters = engines[1].getGrainParameters();
        repaint();
        if (grainParameters.size() == 0)
            return;

        for (int i = 0; i < grainParameters.size(); i++)
        {
            float grainCurrentPosition = std::get<0>(grainParameters[i]);
            float grainVolume = std::get<1>(grainParameters[i]);
            float grainPan = std::get<2>(grainParameters[i]);
            waveForm.addGrain(grainCurrentPosition, grainVolume, grainPan, juce::Colours::skyblue);
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