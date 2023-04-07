#pragma once
#include <JuceHeader.h>

class PitchShifter
{
public:
    PitchShifter(int fftOrder, float pitchShiftRatio);
    ~PitchShifter();

    void process(AudioSampleBuffer &buffer);

private:
    dsp::FFT fft;
    float pitchShiftRatio;
    int fftSize;
    int hopSize;
    int windowSize;
};