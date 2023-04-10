#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
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
    double getLastRelativeClick();
    double getRelativeLoopLength();
    bool isLooping();

    // Mouse logic
    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    String timeFromSecs(double seconds);

    juce::AudioThumbnail audioThumb;

    bool loaded;
    bool looping;
    double loopRelativeLength;
    double curPos;
    double lastRelativeClick;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioWaveform)
};
