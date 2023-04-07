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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

Grain::~Grain()
{
    delete grainBuffer;
}

//----------------------------------------------------------------------------------------------

// Init grain

void Grain::initGrain(AudioSampleBuffer *sample)
{
    const int numChannels = sample->getNumChannels();
    const int numSamples = sample->getNumSamples();

    int grainLengthInSamples = (grainLength / grainSampleRate) * numSamples;
    int startSample = grainStartPosition * numSamples;
    int endSample = startSample + grainLengthInSamples;

    int sampleLength = endSample - startSample;
    sampleLength = jmin(sampleLength, numSamples);

    grainBuffer = new AudioSampleBuffer(numChannels, sampleLength);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        grainBuffer->clear(channel, 0, grainBuffer->getNumSamples());
        grainBuffer->copyFrom(channel, 0, *sample, channel, startSample, sampleLength);
    }
}

//----------------------------------------------------------------------------------------------

// Setters

void Grain::setGrainSampleRate(float sampleRate)
{
    grainSampleRate = sampleRate;
}

void Grain::setGrainLengthInMs(float length)
{
    grainLength = length;
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

void Grain::setGrainPlaybackPosition(int position)
{
    grainPlaybackPosition = position;
}
//----------------------------------------------------------------------------------------------

// Getters
int Grain::getGrainPlaybackPosition()
{
    return grainPlaybackPosition;
}

float Grain::getGrainPitch()
{
    return grainPitch;
}

AudioSampleBuffer *Grain::getGrainBuffer()
{
    return grainBuffer;
}

//----------------------------------------------------------------------------------------------