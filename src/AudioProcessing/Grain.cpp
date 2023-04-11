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
    grainVolume = 50;
    grainLengthInMs = 100;
    grainSpeed = 100;
    grainPan = 50;

    // Misc parameters
    grainStartPosition = 0;
    grainPitch = 261.62;
    grainSampleRate = 44100;
}

Grain::~Grain()
{
}

//----------------------------------------------------------------------------------------------

// Init grain

void Grain::initGrain(AudioSampleBuffer *sampleBuffer)
{
    const int numSamples = sampleBuffer->getNumSamples();

    // Setting a max pitch of 4000Hz so that the grain doesn't get too high pitched or skipped entirely
    const float maxPitch = 4000.0f;
    grainPlaybackRate = jmin(grainPitch / 261.62, maxPitch / 261.62);
    float grainSpeedInFloat = grainSpeed / 100.0f;
    grainPlaybackRate *= grainSpeedInFloat;

    grainLengthInSamples = (int)ceil((grainSampleRate * (grainLengthInMs / 1000.0f)) * grainPlaybackRate);
    grainStartPositionInSamples = (int)ceil(grainStartPosition * numSamples);
    grainStartPositionInSamples = jmax(grainStartPositionInSamples, 1);
    grainEndPositionInSamples = grainStartPositionInSamples + grainLengthInSamples;
    grainEndPositionInSamples = jmin(grainEndPositionInSamples, numSamples);
    grainLengthInSamples = grainEndPositionInSamples - grainStartPositionInSamples;
    generateEnvelope(0.1, 0.2, 0.5, 0.2, grainLengthInSamples, grainVolume);
}

void Grain::updateGrain(AudioSampleBuffer &audioBlock, AudioSampleBuffer *sampleBuffer)
{
    const int blockSamples = audioBlock.getNumSamples();
    const int samplesTotalSamples = sampleBuffer->getNumSamples();
    float grainVolumeInFloat = grainVolume / 100.0f;

    // Get the sample data from the file

    int playbackAmount = grainLengthInSamples - grainPlaybackPositionInSamples;
    playbackAmount = jmin(playbackAmount, blockSamples);

    dsp::WindowingFunction<float> window{(size_t)playbackAmount, dsp::WindowingFunction<float>::hann, true};
    // Mix the grain's waveform with the output buffer
    for (int channel = 0; channel < audioBlock.getNumChannels(); ++channel)
    {
        const float *sampleData = sampleBuffer->getReadPointer(channel);
        float *outputData = audioBlock.getWritePointer(channel);

        for (int i = 0; i < playbackAmount; ++i)
        {
            if (grainPlaybackPositionInSamples + i >= grainLengthInSamples)
                continue;

            float positionInFloat = ((float)(grainStartPositionInSamples + grainPlaybackPositionInSamples + i) * grainPlaybackRate);
            int positionInInt = (int)ceil(positionInFloat);
            float remainder = positionInFloat - positionInInt;

            if (positionInInt - 1 < 0 || positionInInt + 2 >= samplesTotalSamples)
                continue;

            // Cubic interpolation
            float y0 = sampleData[positionInInt];
            float y1 = sampleData[positionInInt + 1];
            float y2 = sampleData[positionInInt + 2];
            float y3 = sampleData[positionInInt + 3];

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
            outputData[i] += (nextSample * grainEnvelope[grainPlaybackPositionInSamples + i] * grainVolumeInFloat);
        }
    }
    grainPlaybackPositionInSamples += playbackAmount;
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

void Grain::generateEnvelope(float a, float d, float s, float r, int lengthInSamples, float peakVolume)
{
    int attackSamples = (int)ceil(a * lengthInSamples);
    int decaySamples = (int)ceil(d * lengthInSamples);
    int releaseSamples = (int)ceil(r * lengthInSamples);
    grainEnvelope.resize(lengthInSamples + 1);
    for (int i = 0; i < lengthInSamples; i++)
    {
        if (i < attackSamples)
        {
            grainEnvelope[i] = ((float)i / (float)attackSamples);
        }
        else if (i < attackSamples + decaySamples)
        {
            grainEnvelope[i] = (1.0f - (1.0f - s) * (i - attackSamples) / decaySamples);
        }
        else if (i < lengthInSamples - releaseSamples)
        {
            grainEnvelope[i] = (s);
        }
        else
        {
            grainEnvelope[i] = (s * (1.0f - (float)(i - (lengthInSamples - releaseSamples)) / (float)releaseSamples));
        }
    }
    for (int j = 0; j < 10; j++)
    {
        DBG("Grain envelope: " << j << " " << String(grainEnvelope[lengthInSamples - 10 + j]));
    }
}
//----------------------------------------------------------------------------------------------

// Setters

void Grain::setGrainSampleRate(float sampleRate)
{
    grainSampleRate = sampleRate;
}

void Grain::setGrainLengthInMs(int length)
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