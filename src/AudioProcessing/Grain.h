#pragma once

#include <JuceHeader.h>
#include "EnvelopeGenerator.h"

class Grain
{
public:
    Grain();
    ~Grain();
    // Start Position
    void setGrainStartPosition(float position);
    void setGrainSampleRate(float sampleRate);
    void setGrainLoop(bool loop);
    void setGrainPitch(float pitch);

    // Grain Params
    void setGrainLengthInMs(int length);
    void setGrainVolume(float volume);
    void setGrainSpeed(float speed);
    void setGrainPan(float pan);

    // Randomness
    void setGrainRandomLength(bool randomLength);
    void setGrainRandomPosition(bool randomPosition);
    void setGrainRandomSpeed(bool randomSpeed);
    void setGrainRandomPan(bool randomPan);

    // Reverse
    void setGrainReverse(bool reverse);

    // Getters
    float getGrainLengthInMs();
    float getGrainPitch();
    int getGrainPlaybackPositionInSamples();
    int getGrainLengthInSamples();

    // Other
    void initGrain(AudioSampleBuffer *sampleBuffer, int type, float attack, float peak, float decay, float sustain, float release);
    void updateGrain(AudioSampleBuffer &audioBlock, AudioSampleBuffer *sampleBuffer);
    float cubicInterpolation(float x, float y0, float y1, float y2, float y3);
    void generateEnvelope(int type, float attack, float peak, float decay, float sustain, float release, int lengthInSamples);

private:
    // Grain parameters
    float grainVolume;
    int grainLengthInMs;
    float grainSpeed;
    float grainPan;

    // Misc parameters
    float grainStartPosition;
    float grainPitch;
    float grainSampleRate;

    // Randomization
    bool grainLoop;
    bool grainReverse;
    bool grainRandomLength;
    bool grainRandomPosition;
    bool grainRandomSpeed;
    bool grainRandomPitch;
    bool grainRandomLoop;
    bool grainRandomReverse;

    float grainPlaybackRate = 1.0f;
    int grainStartPositionInSamples;
    int grainEndPositionInSamples;
    int grainLengthInSamples;
    int grainPlaybackPositionInSamples;

    // Envelope
    EnvelopeGenerator envelopeGenerator;
    std::vector<float> grainEnvelope;
};