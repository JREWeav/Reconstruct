#pragma once
#include <JuceHeader.h>

class PitchShifter
{
public:
    PitchShifter(int fftOrder, float pitchShiftRatio);
    ~PitchShifter();

    void process(AudioSampleBuffer &buffer);
    void createWindow();

private:
    dsp::FFT fft;
    float pitchShiftRatio;
    int fftSize;
    int hopSize;
    int windowSize;

    std::vector<float> prevPhases;
    std::vector<float> currentPhases;

    std::unique_ptr<std::complex<float>[]> fftData;
    std::unique_ptr<float[]> window;
    std::vector<float> outputBuffer;
};