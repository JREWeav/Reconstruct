#pragma once

#include <JuceHeader.h>

class EnvelopeGenerator
{
public:
    EnvelopeGenerator();
    ~EnvelopeGenerator();

    void generateADSR(std::vector<float> *, float attack, float peak, float decay, float sustain, float release, float totalSamples);
    void generateASR(std::vector<float> *, float attack, float sustain, float release, float totalSamples);
    void generateHamming(std::vector<float> *, float totalSamples);
    void generateHann(std::vector<float> *, float totalSamples);
    void generateBlackman(std::vector<float> *, float totalSamples);
    void generateWhiteNoise(std::vector<float> *, float totalSamples);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeGenerator)
};