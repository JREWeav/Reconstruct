#include "AudioWaveform.h"

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
        if (looping)
        {
            g.setColour(Colours::cornflowerblue);
            g.setOpacity(0.6);
            g.fillRect(lastRelativeClick * getWidth(), 0, loopRelativeLength * getWidth(), getHeight());
        }
        g.setOpacity(1);
        g.setColour(Colours::aliceblue);

        Rectangle<int> channelRect{0, 15, getWidth(), getHeight() - 20};
        audioThumb.drawChannel(g, channelRect, 0, audioThumb.getTotalLength(), 0, 1.0f);

        // Draw Playhead
        g.setColour(Colours::cyan);
        g.drawRect((int)(curPos * getWidth()), 0, 2, getHeight());

        // Draw Time
        g.setColour(Colours::black);
        g.fillRect(0, 0, 100, 15);
        g.setColour(Colours::white);
        g.drawText(timeFromSecs(curPos * audioThumb.getTotalLength()) + "/" + timeFromSecs(audioThumb.getTotalLength()), 0, 0, 100, 15, Justification::centred);
    }
    else
    {
        g.setColour(Colours::aliceblue);
        g.drawText("Audio Not Loaded", getLocalBounds(), Justification::centred);
    }
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
    }
}

void AudioWaveform::mouseDrag(const MouseEvent &event)
{
    if (loaded)
    {
        looping = true;
        loopRelativeLength = (double)event.getDistanceFromDragStartX() / (double)getWidth();
        repaint();
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