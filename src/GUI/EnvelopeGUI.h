#pragma once

#include <JuceHeader.h>

class EnvelopeGUI : public juce::Component,
                    public juce::ChangeBroadcaster,
                    public juce::ComboBox::Listener
{
public:
    EnvelopeGUI();
    ~EnvelopeGUI() override;

    void paint(juce::Graphics &) override;
    void resized() override;

    void comboBoxChanged(juce::ComboBox *) override;

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;

    void calculatePoints();

    // Getters
    bool getCollapseState();
    int getEnvelopeType();
    float getAttack();
    float getPeak();
    float getDecay();
    float getSustain();
    float getRelease();

private:
    juce::ComboBox envelopeType;

    juce::StringArray envelopeTypes{"ADSR", "ASR", "Hamming", "Hann", "Blackman", "White Noise"};

    bool isCollapsed = true;

    struct Envelope
    {
        int type;
        float attack;
        float peak;
        float decay;
        float sustain;
        float release;
    };

    Envelope envelope;

    float height;
    float heightOffset;

    struct Point
    {
        float x;
        float y;
    };

    Point attackPoint;
    Point decayPoint;
    Point releasePoint;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeGUI)
};