#include "Grain.h"

/*
TODO:
- Add randomisation to all parameters
- Add AMP envelopes (Start with ADSR)
- Add reverse?
*/

Grain::Grain()
{
    grainPlaybackPositionInSamples = 0;
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    grainVolume = 0.5;
}

Grain::~Grain()
{
}

//----------------------------------------------------------------------------------------------

// Init grain

void Grain::initGrain(AudioSampleBuffer *sampleBuffer)
{
    const int numSamples = sampleBuffer->getNumSamples();

    float grainSpeedInFloat = grainSpeed / 100.0f;

    // Setting a max pitch of 4000Hz so that the grain doesn't get too high pitched or skipped entirely
    const float maxPitch = 4000.0f;
    grainPlaybackRate = jmin(grainPitch / 261.62, maxPitch / 261.62);

    grainPlaybackRate *= grainSpeedInFloat;

    grainLengthInSamples = (int)ceil((grainSampleRate * (grainLengthInMs / 1000)) * grainPlaybackRate);
    grainStartPositionInSamples = (int)ceil(grainStartPosition * numSamples);
    grainEndPositionInSamples = grainStartPositionInSamples + grainLengthInSamples;
    grainEndPositionInSamples = jmin(grainEndPositionInSamples, numSamples);
    grainLengthInSamples = grainEndPositionInSamples - grainStartPositionInSamples;
}

void Grain::updateGrain(AudioSampleBuffer &audioBlock, AudioSampleBuffer *sampleBuffer)
{
    const int blockSamples = audioBlock.getNumSamples();
    const int samplesTotalSamples = sampleBuffer->getNumSamples();

    // Get the sample data from the file

    int playbackAmount = grainLengthInSamples - grainPlaybackPositionInSamples;
    playbackAmount = jmin(playbackAmount, blockSamples);

    // Mix the grain's waveform with the output buffer
    for (int channel = 0; channel < audioBlock.getNumChannels(); ++channel)
    {
        const float *sampleData = sampleBuffer->getReadPointer(channel);
        float *outputData = audioBlock.getWritePointer(channel);

        for (int i = 0; i < playbackAmount; ++i)
        {
            float positionInFloat = ((float)(grainStartPositionInSamples + grainPlaybackPositionInSamples + i) * grainPlaybackRate);
            int positionInInt = (int)ceil(positionInFloat);
            float remainder = positionInFloat - positionInInt;

            if (positionInInt - 1 < 0 || positionInInt + 1 >= samplesTotalSamples)
                continue;

            // Cubic interpolation
            float y0 = sampleData[positionInInt - 1];
            float y1 = sampleData[positionInInt];
            float y2 = sampleData[positionInInt + 1];
            float y3 = sampleData[positionInInt + 2];

            float nextSample = cubicInterpolation(remainder, y0, y1, y2, y3);

            // Panning
            float grainPanInFloat = grainPan / 100.0f;
            float grainPanL = sqrt(1.0f - grainPanInFloat);
            float grainPanR = sqrt(grainPanInFloat);

            if (audioBlock.getNumChannels() == 2)
            {

                if (channel == 0)
                    nextSample *= grainPanL;
                else if (channel == 1)
                {
                    nextSample *= grainPanR;
                }
            }
            outputData[i] += nextSample * grainVolume;
        }

        grainPlaybackPositionInSamples += playbackAmount;
    }
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