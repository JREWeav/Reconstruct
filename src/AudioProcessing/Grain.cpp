#include "Grain.h"

/*
TODO:
- Add randomisation to this class
*/

Grain::Grain()
{
    offsetComplete = false;
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

void Grain::initGrain(AudioSampleBuffer *sampleBuffer, int type, float attack, float peak, float decay, float sustain, float release)
{
    const int numSamples = sampleBuffer->getNumSamples();

    // Setting a max pitch of 20000Hz so that the grain doesn't get too high pitched or skipped entirely
    const float maxPitch = 20000.0f;
    grainPlaybackRate = jmin(grainPitch / 261.62, maxPitch / 261.62);
    float grainSpeedInFloat = grainSpeed / 100.0f;
    grainPlaybackRate *= grainSpeedInFloat;

    // Initialise Grain parameters
    grainLengthInSamples = (int)ceil((grainSampleRate * (grainLengthInMs / 1000.0f)));
    grainStartPositionInSamples = (int)ceil(grainStartPosition * numSamples);
    grainStartPositionInSamples = jmax(grainStartPositionInSamples, 1);
    grainEndPositionInSamples = grainStartPositionInSamples + grainLengthInSamples;
    grainEndPositionInSamples = jmin(grainEndPositionInSamples, numSamples);
    grainLengthInSamples = grainEndPositionInSamples - grainStartPositionInSamples;

    // Initialise envelope
    generateEnvelope(type, attack, peak, decay, sustain, release, grainLengthInSamples);
}

void Grain::updateGrain(int numSamples, AudioSampleBuffer &audioBlock, AudioSampleBuffer *sampleBuffer)
{
    const int samplesTotalSamples = sampleBuffer->getNumSamples();
    float grainVolumeInFloat = grainVolume / 100.0f;
    grainCurrentRelativePosition = (float)(grainStartPositionInSamples + grainPlaybackPositionInSamples) / (float)samplesTotalSamples;
    grainCurrentVolume = grainVolumeInFloat * grainEnvelope[grainPlaybackPositionInSamples];

    // Calculate the amount of samples to play back
    int playbackAmount = grainLengthInSamples - grainPlaybackPositionInSamples;
    playbackAmount = jmin(playbackAmount, numSamples);

    if (!offsetComplete)
    {
        if (numSamples > grainLengthInSamples - grainPlaybackPositionInSamples)
        {
            playbackAmount = grainLengthInSamples - grainPlaybackPositionInSamples;
            if (grainOffsetInSamples + playbackAmount > audioBlock.getNumSamples())
            {
                playbackAmount = audioBlock.getNumSamples() - grainOffsetInSamples;
            }
        }
        else
        {
            playbackAmount -= grainOffsetInSamples;
        }
    }

    // Mix the grain's waveform with the output buffer
    for (int channel = 0; channel < audioBlock.getNumChannels(); ++channel)
    {
        const float *sampleData = sampleBuffer->getReadPointer(channel);
        float *outputData = audioBlock.getWritePointer(channel);

        for (int i = 0; i < playbackAmount; ++i)
        {
            if (grainPlaybackPositionInSamples + i >= grainLengthInSamples)
                continue;

            float positionInFloat = ((float)(grainPlaybackPositionInSamples + i) * grainPlaybackRate + grainStartPositionInSamples);
            int positionInInt = (int)ceil(positionInFloat);
            float remainder = positionInFloat - positionInInt;

            if (positionInInt - 1 < 0 || positionInInt + 2 >= samplesTotalSamples)
                continue;

            // Cubic interpolation to get the next sample
            float y0 = sampleData[positionInInt];
            float y1 = sampleData[positionInInt + 1];
            float y2 = sampleData[positionInInt + 2];
            float y3 = sampleData[positionInInt + 3];

            float nextSample = cubicInterpolation(remainder, y0, y1, y2, y3);

            // Apply Panning
            float grainPanInFloat = grainPan / 100.0f;
            float grainPanL = sqrt(1.0f - grainPanInFloat);
            float grainPanR = sqrt(grainPanInFloat);

            if (audioBlock.getNumChannels() == 2)
            {
                if (channel == 0)
                    nextSample *= grainPanL;
                else if (channel == 1)
                    nextSample *= grainPanR;
            }
            // Add the sample to the output buffer
            if (!offsetComplete)
            {
                outputData[i + grainOffsetInSamples] += (nextSample * grainEnvelope[grainPlaybackPositionInSamples + i] * grainVolumeInFloat);
            }
            else
            {
                outputData[i] += (nextSample * grainEnvelope[grainPlaybackPositionInSamples + i] * grainVolumeInFloat);
            }
        }
    }
    offsetComplete = true;
    grainPlaybackPositionInSamples += playbackAmount;
}

// Cubic interpolation - Equation from http://paulbourke.net/miscellaneous/interpolation/

float Grain::cubicInterpolation(float x, float y0, float y1, float y2, float y3)
{
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c2 = y0 - 2.5f * y1 + 2.f * y2 - 0.5f * y3;
    float c3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);

    return ((c3 * x + c2) * x + c1) * x + c0;
}

void Grain::generateEnvelope(int type, float attack, float peak, float decay, float sustain, float release, int lengthInSamples)
{
    if (type == 1)
    {
        envelopeGenerator->generateADSR(&grainEnvelope, attack, peak, decay, sustain, release, lengthInSamples);
    }
    if (type == 2)
    {
        envelopeGenerator->generateASR(&grainEnvelope, attack, sustain, release, lengthInSamples);
    }
    if (type == 3)
    {
        envelopeGenerator->generateHamming(&grainEnvelope, lengthInSamples);
    }
    if (type == 4)
    {
        envelopeGenerator->generateHann(&grainEnvelope, lengthInSamples);
    }
    if (type == 5)
    {
        envelopeGenerator->generateBlackman(&grainEnvelope, lengthInSamples);
    }
    if (type == 6)
    {
        envelopeGenerator->generateWhiteNoise(&grainEnvelope, lengthInSamples);
    }
}
//----------------------------------------------------------------------------------------------

// Setters

// Grain Playback params

void Grain::setGrainSampleRate(float sampleRate)
{
    grainSampleRate = sampleRate;
}

void Grain::setGrainStartPosition(float position)
{
    grainStartPosition = position;
}

void Grain::setGrainOffsetInSamples(int offset)
{
    grainOffsetInSamples = offset;
}

// Grain Pitch from MIDI note

void Grain::setGrainPitch(float _pitch)
{
    grainPitch = _pitch;
}

// Grain params

void Grain::setGrainLengthInMs(int length)
{
    grainLengthInMs = length;
}

void Grain::setGrainVolume(float volume)
{
    grainVolume = volume;
}

void Grain::setGrainSpeed(float speed)
{
    grainSpeed = speed;
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

float Grain::getGrainCurrentRelativePosition()
{
    return grainCurrentRelativePosition;
}

float Grain::getGrainPlaybackRate()
{
    return grainPlaybackRate;
}

float Grain::getGrainCurrentVolume()
{
    return grainCurrentVolume;
}

float Grain::getGrainMaxVolume()
{
    float maxVolume = 0.0f;
    for (int i = 0; i < grainEnvelope.size(); i++)
    {
        if (grainEnvelope[i] > maxVolume)
            maxVolume = grainEnvelope[i];
    }
    maxVolume *= (grainVolume / 100.0f);
    return maxVolume;
}

float Grain::getGrainPan()
{
    return grainPan;
}

//----------------------------------------------------------------------------------------------