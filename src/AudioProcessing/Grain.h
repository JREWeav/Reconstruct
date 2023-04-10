#pragma once

#include <JuceHeader.h>

class Grain
{
public:
    Grain();
    ~Grain();
    // Setters
    void setGrainLengthInMs(float length);
    void setGrainStartPosition(float position);
    void setGrainVolume(float volume);
    void setGrainPitch(float pitch);
    void setGrainSpeed(float speed);
    void setGrainSampleRate(float sampleRate);
    void setGrainLoop(bool loop);
    void setGrainReverse(bool reverse);
    void setGrainRandomLength(bool randomLength);
    void setGrainRandomPosition(bool randomPosition);
    void setGrainRandomSpeed(bool randomSpeed);
    void setGrainRandomPitch(bool randomPitch);
    void setGrainRandomLoop(bool randomLoop);
    void setGrainRandomReverse(bool randomReverse);
    void setGrainPan(float pan);

    // Getters
    float getGrainLengthInMs();
    float getGrainPitch();
    int getGrainPlaybackPositionInSamples();
    int getGrainLengthInSamples();

    // Other
    void initGrain(AudioSampleBuffer *sampleBuffer);
    void updateGrain(AudioSampleBuffer &audioBlock, AudioSampleBuffer *sampleBuffer);
    float cubicInterpolation(float x, float y0, float y1, float y2, float y3);

private:
    // Grain parameters
    float grainLengthInMs;
    float grainStartPosition;
    float grainSpeed;
    float grainVolume;
    float grainPitch;
    float grainPan;
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
};