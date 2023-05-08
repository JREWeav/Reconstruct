#pragma once

#include <JuceHeader.h>

class EnvelopeGUI : public juce::Component,
                    public juce::ChangeBroadcaster,
                    public juce::ComboBox::Listener
{
public:
    EnvelopeGUI(AudioProcessorValueTreeState &vts);
    ~EnvelopeGUI() override;

    void paint(juce::Graphics &) override;
    void resized() override;

    void comboBoxChanged(juce::ComboBox *) override;

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;

    void calculatePoints();

    void updateVTS();
    void recallVTS();

    // Getters
    bool getCollapseState();
    int getEnvelopeType();
    float getAttack();
    float getPeak();
    float getDecay();
    float getSustain();
    float getRelease();

private:
    ComboBox envelopeType;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> envelopeTypeAttachment;

    StringArray envelopeTypes{"ADSR", "ASR", "AR", "AHR", "AHDSR", "AHDSR2"};

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

    Point *selectedPoint;

    bool firstCall;

    AudioProcessorValueTreeState &vts;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeGUI)
};