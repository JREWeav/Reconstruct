#pragma once

#include <JuceHeader.h>

//==============================================================================
class AudioWaveform : public juce::Component,
                      public juce::ChangeListener,
                      public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    AudioWaveform(AudioFormatManager &formatManagerToUse, AudioThumbnailCache &cacheToUse);
    ~AudioWaveform();

    void loadAudio(InputSource *src);

    void changeListenerCallback(ChangeBroadcaster *source) override;

    // Set the playhead position
    void setRelativePosition(double pos);
    void setRelativeClick(double pos);
    void setRelativeLoopLength(double length);
    void setLooping(bool looping);
    double getLastRelativeClick();
    double getRelativeLoopLength();
    bool isLooping();

    // Mouse logic
    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;

    // Grain drawing
    void drawGrains(juce::Graphics &g);
    void addGrain(float relativePosition, float pan, float volume);
    void clearGrains();

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    //==============================================================================
    String timeFromSecs(double seconds);

    AudioThumbnail audioThumb;

    struct Grain
    {
        float relativePosition;
        float pan;
        float volume;
    };

    std::vector<Grain> grains;

    bool loaded;
    bool looping;
    double loopRelativeLength;
    double curPos;
    double lastRelativeClick;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioWaveform)
};
