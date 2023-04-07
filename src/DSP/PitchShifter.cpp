#include <JuceHeader.h>
#include "PitchShifter.h"

//==============================================================================

PitchShifter::PitchShifter(int fftOrder, float pitchShiftRatio) : fft(fftOrder), pitchShiftRatio(pitchShiftRatio)
{
    fftSize = pow(2, fftOrder);
    hopSize = fftSize / 4;
    windowSize = fftSize;

    window = std::make_unique<float[]>(windowSize);
    fftData = std::make_unique<std::complex<float>[]>(fftSize);
    outputBuffer = std::vector<float>(fftSize, 0.0f);

    prevPhases.resize(fftSize / 2, 0.0f);
    currentPhases.resize(fftSize / 2, 0.0f);

    // Create a Hann window
    createWindow();
}

PitchShifter::~PitchShifter()
{
}

void PitchShifter::process(AudioSampleBuffer &buffer)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    float phaseIncrement = 2.0f * float_Pi * hopSize / fftSize;
    float phaseAdjustment = 2.0f * float_Pi * hopSize * (pitchShiftRatio - 1.0f) / fftSize;

    float inputSum = 0.0f;
    for (int channel = 0; channel < numChannels; ++channel)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            inputSum += std::abs(buffer.getSample(channel, sample));
        }
    }

    AudioSampleBuffer output(AudioSampleBuffer(numChannels, numSamples));
    output.clear();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float *channelData = buffer.getWritePointer(channel);
        float *outputChannelData = output.getWritePointer(channel);

        // Step 1: Overlap-add analysis
        for (int sample = 0; sample < numSamples; sample += hopSize)
        {
            int remainingSamples = numSamples - sample;

            // Step 2: Apply window and perform FFT
            for (int i = 0; i < fftSize; ++i)
            {
                fftData[i] = (i < remainingSamples) ? std::complex<float>(channelData[sample + i] * window[i], 0.0f) : 0.0f;
            }

            fft.performRealOnlyForwardTransform(reinterpret_cast<float *>(fftData.get()));

            // Step 3: Process the frequency-domain data

            std::vector<std::complex<float>> originalFftData;
            originalFftData.resize(fftSize / 2);

            for (int i = 0; i < originalFftData.size(); ++i)
            {
                originalFftData[i] = fftData[i];
            }

            for (int i = 0; i < fftSize / 2; ++i)
            {
                int bin = i;

                // Calculate phase difference
                float phaseDiff = std::arg(originalFftData[bin]) - prevPhases[bin] - phaseIncrement * bin;

                // Unwrap the phase difference
                phaseDiff -= 2.0f * float_Pi * std::round(phaseDiff / (2.0f * float_Pi));

                // Calculate true frequency
                float trueFreq = 2.0f * float_Pi * bin / fftSize + phaseDiff / hopSize;

                // Calculate the new frequency based on pitch shift ratio
                float newFreq = trueFreq * pitchShiftRatio;

                // Find the nearest integer multiple of the new frequency bin
                int newBin = std::round(static_cast<float>(bin) * pitchShiftRatio);

                // Update the phase
                currentPhases[bin] = prevPhases[bin] + hopSize * newFreq;

                // Update the magnitudes and phases of the complex frequency-domain data
                float magnitude = std::abs(originalFftData[bin]);
                std::complex<float> shiftedValue = std::polar(magnitude, currentPhases[bin]);

                // Assign the shifted value to the new bin
                if (newBin >= 0 && newBin < fftSize / 2)
                {
                    fftData[newBin] = shiftedValue;
                }
            }

            prevPhases = currentPhases;

            // Step 4: Perform inverse FFT and apply window
            fft.performRealOnlyInverseTransform(reinterpret_cast<float *>(fftData.get()));
            for (int i = 0; i < fftSize; ++i)
            {
                outputBuffer[i] += ((fftData[i].real() * window[i]) / fftSize);
            }
            // Step 5: Overlap-add synthesis
            int loopLimit = jmin(remainingSamples, fftSize);
            for (int i = 0; i < loopLimit; ++i)
            {
                outputChannelData[sample + i] += outputBuffer[i];
                outputBuffer[i] = 0.0f;
            }
        }
    }
    float outputSum = 0.0f;
    for (int channel = 0; channel < numChannels; ++channel)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            outputSum += std::abs(output.getSample(channel, sample));
        }
    }
    DBG("Output sum: " << outputSum);
    buffer = output;
}

void PitchShifter::createWindow()
{
    for (int i = 0; i < fftSize; ++i)
    {
        window[i] = 0.5f * (1.0f - std::cos(2.0f * float_Pi * static_cast<float>(i) / static_cast<float>(fftSize - 1)));
    }
}