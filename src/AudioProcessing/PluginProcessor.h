#pragma once

#include <JuceHeader.h>
#include "GranularEngine.h"
#include "Grain.h"

//==============================================================================
class AudioPluginAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

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
    void setGrainsPerSecond(float grainsPerSecond);
    void setGrainParameters(float randomGrainVolume, int grainLengthInMs, float grainSpeed, float grainPan);
    void setRandomParameters(float randomGrainVolume, int randomGrainLengthInMs, float randomGrainSpeed, float randomGrainPan);
    void setEnvelopeParameters(int type, float attack, float peak, float decay, float sustain, float release);
    void setSampleParameters(float sampleStart, float sampleEnd);

    // Get the parameters of the grains
    std::vector<std::tuple<float, float, float>> getGrainParameters();

private:
    //==============================================================================
    juce::AudioFormatManager formatManager;
    std::vector<Grain *> grains;
    GranularEngine granularEngine{formatManager};
    const int numVoices = 100;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
