#include "AudioWaveform.h"

/*
TODO:
- Add grains to the waveform
*/

//==============================================================================
AudioWaveform::AudioWaveform(AudioFormatManager &formatManagerToUse, AudioThumbnailCache &cacheToUse) : audioThumb(10000, formatManagerToUse, cacheToUse)
{
    audioThumb.addChangeListener(this);

    loaded = false;
    looping = false;
    curPos = 0;
    lastRelativeClick = 0;
}

AudioWaveform::~AudioWaveform()
{
}

void AudioWaveform::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &audioThumb)
    {
        repaint();
    }
}

//==============================================================================
void AudioWaveform::paint(juce::Graphics &g)
{
    if (loaded)
    {
        // Draw selection
        if (looping)
        {
            g.setColour(Colours::maroon);
            g.setOpacity(0.6);
            g.fillRect(lastRelativeClick * getWidth(), 0, loopRelativeLength * getWidth(), getHeight());
        }

        // Draw Waveform
        g.setOpacity(1);
        g.setColour(Colours::aliceblue);

        Rectangle<int> channelRect{0, 15, getWidth(), getHeight() - 20};
        audioThumb.drawChannel(g, channelRect, 0, audioThumb.getTotalLength(), 0, 1.0f);

        // Draw Playhead
        g.setColour(Colours::red);
        g.drawRect((int)(curPos * getWidth()), 0, 2, getHeight());

        // Draw Time
        g.setOpacity(1);
        g.setColour(Colours::slategrey);
        g.fillRect(0, 0, 100, 15);
        g.setColour(Colours::white);
        g.drawText(timeFromSecs(curPos * audioThumb.getTotalLength()) + "/" + timeFromSecs(audioThumb.getTotalLength()), 0, 0, 100, 15, Justification::centred);
    }
    else
    {
        g.setColour(Colours::aliceblue);
        g.drawText("Audio Not Loaded", getLocalBounds(), Justification::centred);
    }
    drawGrains(g);
}

void AudioWaveform::drawGrains(juce::Graphics &g)
{
    for (auto grain : grains)
    {
        g.setOpacity(0.3);
        g.setColour(Colours::goldenrod);
        float relativePosition = jmax(grain.relativePosition, 0.0f);
        int x = (int)(grain.relativePosition * getWidth());
        x = jmin(x, getWidth());
        x = jmax(x, 0);
        int y = (int)(getHeight() * (grain.pan / 100.0f));
        y = jmin(y, getHeight());
        y = jmax(y, 0);
        float size = 20.0f * sqrt(grain.volume);
        size = jmax(size, 0.0f);
        if (size > 0)
            g.fillEllipse(x, y, size, size);
    }
}

void AudioWaveform::addGrain(float relativePosition, float volume, float pan)
{
    Grain grain;
    grain.relativePosition = relativePosition;
    grain.volume = volume;
    grain.pan = pan;
    grains.push_back(grain);
}

void AudioWaveform::clearGrains()
{
    grains.clear();
}

void AudioWaveform::resized()
{
}

void AudioWaveform::loadAudio(InputSource *src)
{
    audioThumb.clear();
    loaded = audioThumb.setSource(src);
    if (loaded)
    {
        audioThumb.sendChangeMessage();
    }
    looping = false;
}

void AudioWaveform::setRelativePosition(double pos)
{
    if (pos != curPos)
    {
        curPos = pos;
        repaint();
    }
}

void AudioWaveform::mouseDown(const MouseEvent &event)
{
    if (loaded)
    {
        looping = false;
        lastRelativeClick = (double)event.getMouseDownX() / (double)getWidth();
        sendChangeMessage();
    }
}

void AudioWaveform::mouseDrag(const MouseEvent &event)
{
    if (loaded)
    {
        looping = true;
        loopRelativeLength = (double)event.getDistanceFromDragStartX() / (double)getWidth();
        repaint();
        sendChangeMessage();
    }
}

double AudioWaveform::getLastRelativeClick()
{
    return lastRelativeClick;
}

double AudioWaveform::getRelativeLoopLength()
{
    return loopRelativeLength;
}

bool AudioWaveform::isLooping()
{
    return looping;
}

String AudioWaveform::timeFromSecs(double seconds)
{
    String minString;
    String secString;
    int secondsLeft = (int)seconds % 60;
    int mins = (int)(seconds - secondsLeft) / 60;
    if (mins < 10)
    {
        minString = "0" + (String)mins;
    }
    else
    {
        minString = (String)mins;
    }
    if (secondsLeft < 10)
    {
        secString = "0" + (String)secondsLeft;
    }
    else
    {
        secString = (String)secondsLeft;
    }
    return minString + ":" + secString;
}