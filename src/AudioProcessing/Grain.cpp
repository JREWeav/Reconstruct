#include "Grain.h"

/*
TODO:
- Make the waveform control grain start and end points
- Control pitch and speed of the grain
- Add Panning
- Add randomisation to all parameters
- Add looping
- Add AMP envelopes (Start with ADSR)
- Add reverse?
*/

Grain::Grain()
{
    grainPlaybackPositionInSamples = 0;
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

Grain::~Grain()
{
}

//----------------------------------------------------------------------------------------------

// Init grain

void Grain::initGrain(AudioSampleBuffer *sampleBuffer)
{
    const int numSamples = sampleBuffer->getNumSamples();

    grainLengthInSamples = (int)ceil(grainSampleRate * (grainLengthInMs / 1000));
    grainStartPositionInSamples = (int)ceil(grainStartPosition * numSamples);
    grainEndPositionInSamples = grainStartPositionInSamples + grainLengthInSamples;
    grainLengthInSamples = grainEndPositionInSamples - grainStartPositionInSamples;
}

void Grain::updateGrain(AudioSampleBuffer &audioBlock, AudioSampleBuffer *sampleBuffer)
{
    const int blockSamples = audioBlock.getNumSamples();

    int playbackAmount = grainLengthInSamples - grainPlaybackPositionInSamples;
    playbackAmount = jmin(playbackAmount, blockSamples);
    int positionInSamples = grainStartPositionInSamples + grainPlaybackPositionInSamples;

    // float position = grainStartPosition + (grainPlaybackPosition / (float)grainNumSamples);
    // int intPosition = (int)ceil(position);

    // Mix the grain's waveform with the output buffer
    for (int channel = 0; channel < audioBlock.getNumChannels(); ++channel)
    {
        audioBlock.addFrom(channel, 0, *(sampleBuffer), channel, positionInSamples, playbackAmount, 0.3f);
    }

    grainPlaybackPositionInSamples += blockSamples;
}

// Cubic interpolation

float Grain::cubicInterpolation(float x, float y0, float y1, float y2, float y3)
{
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c2 = y0 - 2.5f * y1 + 2.f * y2 - 0.5f * y3;
    float c3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);

    return ((c3 * x + c2) * x + c1) * x + c0;
}

//----------------------------------------------------------------------------------------------

// Setters

void Grain::setGrainSampleRate(float sampleRate)
{
    grainSampleRate = sampleRate;
}

void Grain::setGrainLengthInMs(float length)
{
    grainLengthInMs = length;
}

void Grain::setGrainVolume(float volume)
{
    grainVolume = volume;
}

void Grain::setGrainStartPosition(float position)
{
    grainStartPosition = position;
}

void Grain::setGrainSpeed(float speed)
{
    grainSpeed = speed;
}

void Grain::setGrainPitch(float pitch)
{
    grainPitch = pitch;
}

void Grain::setGrainLoop(bool loop)
{
    grainLoop = loop;
}

void Grain::setGrainReverse(bool reverse)
{
    grainReverse = reverse;
}

void Grain::setGrainRandomLength(bool randomLength)
{
    grainRandomLength = randomLength;
}

void Grain::setGrainRandomPosition(bool randomPosition)
{
    grainRandomPosition = randomPosition;
}

void Grain::setGrainRandomSpeed(bool randomSpeed)
{
    grainRandomSpeed = randomSpeed;
}

void Grain::setGrainRandomPitch(bool randomPitch)
{
    grainRandomPitch = randomPitch;
}

void Grain::setGrainRandomLoop(bool randomLoop)
{
    grainRandomLoop = randomLoop;
}

void Grain::setGrainRandomReverse(bool randomReverse)
{
    grainRandomReverse = randomReverse;
}

void Grain::setGrainPan(float pan)
{
    grainPan = pan;
}
//----------------------------------------------------------------------------------------------

// Getters
int Grain::getGrainPlaybackPositionInSamples()
{
    return grainPlaybackPositionInSamples;
}

int Grain::getGrainLengthInSamples()
{
    return grainLengthInSamples;
}

float Grain::getGrainPitch()
{
    return grainPitch;
}

//----------------------------------------------------------------------------------------------