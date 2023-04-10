#pragma once

#include <JuceHeader.h>
#include "Grain.h"

class GranularEngine : public juce::AudioProcessor
{
public:
    //==============================================================================
    GranularEngine(juce::AudioFormatManager &formatManager);
    ~GranularEngine() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    //==============================================================================

    void generateGrain(int midiNoteNumber, float velocity);

    void loadSampleFromUrl(juce::URL &url);

    void setRelativeSampleStart(float sampleStart);
    void setRelativeSampleEnd(float sampleEnd);

    void setGrainDensityInHz(float grainsPerSecondInHz);
    void setGrainLengthInMs(float grainLengthInMs);
    void setGrainSpeed(float grainSpeed);
    void setGrainPan(float grainPan);

private:
    //==============================================================================
    std::vector<Grain *> grainPool;
    juce::AudioFormatManager &formatManager;
    float timerForGrainGen;
    float grainInterval;
    AudioSampleBuffer *sampleBuffer;
    double storedSampleRate;
    int processedSamples;

    Random random;

    std::vector<MidiMessage> heldNotes;

    // Grain parameters
    float grainLengthInMs;
    float grainSpeed;
    float grainPan;
    float grainsPerSecondInHz;
    float grainVolume;

    // Sample parameters
    float sampleStart;
    float sampleEnd;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GranularEngine)
};