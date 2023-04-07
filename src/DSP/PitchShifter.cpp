#include <JuceHeader.h>
#include "PitchShifter.h"

//==============================================================================

PitchShifter::PitchShifter(int fftOrder, float pitchShiftRatio) : fft(fftOrder), pitchShiftRatio(pitchShiftRatio)
{
    fftSize = 1 << fftOrder;
    hopSize = fftSize / 4;
    windowSize = fftSize;
}

PitchShifter::~PitchShifter()
{
}