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

    void loadSampleFromUrl(juce::URL &url);

    // Setters for sample parameters
    void setRelativeSampleStart(float sampleStart);
    void setRelativeSampleEnd(float sampleEnd);

    void setGrainsPerSecond(float grainsPerSecond);

    // Setters for grain parameters
    void setGrainVolume(float _grainVolume);
    void setGrainLengthInMs(int _grainLengthInMs);
    void setGrainSpeed(float _grainSpeed);
    void setGrainPan(float _grainPan);

    // Setters for randomization
    void setRandomGrainVolume(float _randomGrainVolume);
    void setRandomGrainLengthInMs(int _randomGrainLengthInMs);
    void setRandomGrainSpeed(float _randomGrainSpeed);
    void setRandomGrainPan(float _randomGrainPan);

    // Setters for envelope
    void setEnvelopeParameters(int type, float attack, float peak, float decay, float sustain, float release);

    // Setters for envelope
    void setEnvelope(struct envelope);

    // Getter for grain pool
    std::vector<std::tuple<float, float, float>> getGrainParameters();

    // Grain processing
    void generateGrain(int midiNoteNumber, float velocity, int offsetInSamples);
    void processActiveGrains(int numSamples, AudioSampleBuffer &buffer, AudioSampleBuffer *sampleBuffer);

private:
    //==============================================================================
    std::vector<Grain *> grainPool;
    juce::AudioFormatManager &formatManager;
    AudioSampleBuffer *sampleBuffer;
    float storedSampleRate;
    int processedSamples;

    Random random;

    std::vector<MidiMessage> heldNotes;

    // Sample parameters
    float sampleStart;
    float sampleEnd;

    // Grains per second
    float grainsPerSecond;
    float lastGrainTime;
    int grainTimerInSamples;

    // Grain parameters
    float grainVolume;
    int grainLengthInMs;
    float grainSpeed;
    float grainPan;

    // Randomization parameters
    float randomGrainVolume;
    int randomGrainLengthInMs;
    float randomGrainSpeed;
    float randomGrainPan;

    // Envelope
    struct Envelope
    {
        int type;
        float attack;
        float peak;
        float decay;
        float sustain;
        float release;
    };

    Envelope envelope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GranularEngine)
};