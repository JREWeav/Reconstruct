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
    void setGrainSpeed(float speed);
    void setGrainPitch(float pitch);
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
    void setGrainPlaybackPosition(int position);

    // Getters
    float getGrainLengthInMs();
    int getGrainPlaybackPosition();

    AudioSampleBuffer *getGrainBuffer();

    void deleteGrain();

    void initGrain(AudioSampleBuffer *sample);

private:
    float grainLength = 0.1f;
    float grainStartPosition = 0.0f;
    float grainSpeed = 1.0f;
    float grainVolume = 1.0f;
    float grainPitch = 1.0f;
    float grainPan = 0.0f;
    float grainSampleRate = 44100.0f;
    bool grainLoop = false;
    bool grainReverse = false;
    bool grainRandomLength = false;
    bool grainRandomPosition = false;
    bool grainRandomSpeed = false;
    bool grainRandomPitch = false;
    bool grainRandomLoop = false;
    bool grainRandomReverse = false;
    int grainPlaybackPosition = 0;

    AudioSampleBuffer *grainBuffer;
};