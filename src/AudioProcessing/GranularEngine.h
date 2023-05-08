#pragma once

#include <JuceHeader.h>
#include "Grain.h"

class GranularEngine : public juce::AudioProcessor,
                       public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    GranularEngine(AudioFormatManager &formatManager, AudioProcessorValueTreeState &vts);
    ~GranularEngine() override;
    //==============================================================================

    //==============================================================================
    // AudioProcessor
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    AudioProcessorValueTreeState &vts;

    //==============================================================================

    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    // Setup
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

    // Load sample from file

    void loadSampleFromUrl(juce::URL &url);

    // Setters for sample parameters
    void setRelativeSampleStart(float sampleStart);
    void setRelativeSampleEnd(float sampleEnd);

    void setGrainDensity(float _grainDensity);

    // Setters for grain parameters
    void setGrainVolume(float _grainVolume);
    void setGrainLengthInMs(int _grainLengthInMs);
    void setGrainSpeed(float _grainSpeed);
    void setGrainPan(float _grainPan);

    // Setters for randomization
    void setRandomGrainVolume(float _randomGrainVolume, int dir);
    void setRandomGrainLengthInMs(int _randomGrainLengthInMs, int dir);
    void setRandomGrainSpeed(float _randomGrainSpeed, int dir);
    void setRandomGrainPan(float _randomGrainPan, int dir);

    // Setters for envelope
    void setEnvelopeParameters(int type, float attack, float peak, float decay, float sustain, float release);

    // Setters for envelope
    void setEnvelope(struct envelope);

    // Getter for grain pool
    std::vector<std::tuple<float, float, float>> getGrainParameters();

    int getStoredSampleRate();

    // Grain processing
    void generateGrain(int midiNoteNumber, float velocity, int offsetInSamples);
    void processActiveGrains(int numSamples, AudioSampleBuffer &buffer);

    // Check if sample is loaded
    bool isFileLoaded();
    AudioSampleBuffer *getSampleBuffer();

private:
    //==============================================================================
    std::vector<Grain *> grainPool;
    juce::AudioFormatManager &formatManager;
    std::unique_ptr<AudioSampleBuffer> sampleBuffer;
    float storedSampleRate;
    int processedSamples;
    int storedBufferSize;

    bool fileLoaded;

    Random random;

    std::vector<MidiMessage> heldNotes;

    // Sample parameters
    float sampleStart;
    float sampleEnd;

    // Grains per second
    float grainDensity;
    float lastGrainTime;
    int grainTimerInSamples;

    // Grain parameters
    float grainVolume;
    int grainLengthInMs;
    float grainSpeed;
    float grainPan;

    // Randomization parameters
    float randomGrainVolume;
    int randomGrainVolumeState;
    int randomGrainLengthInMs;
    int randomGrainLengthState;
    float randomGrainSpeed;
    int randomGrainSpeedState;
    float randomGrainPan;
    int randomGrainPanState;

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