#include "EnvelopeGenerator.h"

EnvelopeGenerator::EnvelopeGenerator()
{
}

EnvelopeGenerator::~EnvelopeGenerator()
{
}

void EnvelopeGenerator::generateADSR(std::vector<float> *envelope, float attack, float peak, float decay, float sustain, float release, float totalSamples)
{
    float attackSamples = attack * totalSamples;
    float decaySamples = decay * totalSamples;
    float releaseSamples = release * totalSamples;
    float sustainSamples = sustain * totalSamples;

    envelope->clear();
    envelope->resize(totalSamples);

    for (int i = 0; i < totalSamples; i++)
    {
        if (i <= attackSamples)
        {
            envelope->at(i) = i / attackSamples * peak;
        }
        else if (i <= attackSamples + decaySamples)
        {
            envelope->at(i) = peak - (peak - sustain) * (i - attackSamples) / decaySamples;
        }
        else if (i <= totalSamples - releaseSamples)
        {
            envelope->at(i) = sustain;
        }
        else
        {
            envelope->at(i) = sustain * (totalSamples - i) / releaseSamples;
        }
    }
}

void EnvelopeGenerator::generateASR(std::vector<float> *envelope, float attack, float sustain, float release, float totalSamples)
{
    float attackSamples = attack * totalSamples;
    float releaseSamples = release * totalSamples;

    envelope->clear();
    envelope->resize(totalSamples);

    for (int i = 0; i < totalSamples; i++)
    {
        if (i <= attackSamples)
        {
            envelope->at(i) = i / attackSamples * sustain;
        }
        else if (i <= totalSamples - releaseSamples)
        {
            envelope->at(i) = sustain;
        }
        else
        {
            envelope->at(i) = sustain * (totalSamples - i) / releaseSamples;
        }
    }
}

void EnvelopeGenerator::generateHamming(std::vector<float> *envelope, float totalSamples)
{
    juce::dsp::WindowingFunction<float> window(totalSamples, juce::dsp::WindowingFunction<float>::hamming);
    envelope->resize(totalSamples);
    std::fill(envelope->begin(), envelope->end(), 1);
    window.multiplyWithWindowingTable(envelope->data(), totalSamples);
}

void EnvelopeGenerator::generateHann(std::vector<float> *envelope, float totalSamples)
{
    juce::dsp::WindowingFunction<float> window(totalSamples, juce::dsp::WindowingFunction<float>::hann);
    envelope->resize(totalSamples);
    std::fill(envelope->begin(), envelope->end(), 1);
    window.multiplyWithWindowingTable(envelope->data(), totalSamples);
}

void EnvelopeGenerator::generateBlackman(std::vector<float> *envelope, float totalSamples)
{
    juce::dsp::WindowingFunction<float> window(totalSamples, juce::dsp::WindowingFunction<float>::blackman);
    envelope->resize(totalSamples);
    std::fill(envelope->begin(), envelope->end(), 1);
    window.multiplyWithWindowingTable(envelope->data(), totalSamples);
}

void EnvelopeGenerator::generateWhiteNoise(std::vector<float> *envelope, float totalSamples)
{
    envelope->resize(totalSamples);
    juce::Random rand;
    for (int i = 0; i < totalSamples; i++)
    {
        envelope->at(i) = rand.nextFloat();
    }
}