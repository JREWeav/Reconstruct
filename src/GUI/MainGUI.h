#pragma once

#include <JuceHeader.h>
#include "LoadingComponent.h"

class MainGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener
{
public:
    MainGUI(AudioFormatManager &formatManager, AudioThumbnailCache &thumbnailCache, AudioPluginAudioProcessor &p);
    ~MainGUI() override;

    void paint(juce::Graphics &) override;
    void resized() override;
    void buttonClicked(Button *) override;
    void sliderValueChanged(Slider *) override;

private:
    LoadingComponent loadComponent;
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent{keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard};
    const int numVoices = 8;
    URL audioURL;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainGUI)
};