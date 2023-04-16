#include "PluginProcessor.h"
#include "../GUI/PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    granularEngine.prepareToPlay(sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (const MidiMessageMetadata metadata : midiMessages)
    {
        if (metadata.getMessage().isNoteOn())
        {
            // Get the MIDI note number and velocity
            int midiNoteNumber = metadata.getMessage().getNoteNumber();
            float velocity = metadata.getMessage().getVelocity() / 127.0f;
        }
    }

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    granularEngine.processBlock(buffer, midiMessages);
}

//================

void AudioPluginAudioProcessor::loadSampleFromUrl(juce::URL &url)
{
    granularEngine.loadSampleFromUrl(url);
}

void AudioPluginAudioProcessor::setGrainsPerSecond(float grainsPerSecond)
{
    granularEngine.setGrainsPerSecond(grainsPerSecond);
}

void AudioPluginAudioProcessor::setGrainParameters(float grainVolume, int grainLengthInMs, float grainPan, float grainSpeed)
{
    granularEngine.setGrainVolume(grainVolume);
    granularEngine.setGrainLengthInMs(grainLengthInMs);
    granularEngine.setGrainPan(grainPan);
    granularEngine.setGrainSpeed(grainSpeed);
}

void AudioPluginAudioProcessor::setRandomParameters(float randomGrainVolume, int randomGrainLengthInMs, float randomGrainSpeed, float randomGrainPan)
{
    granularEngine.setRandomGrainVolume(randomGrainVolume);
    granularEngine.setRandomGrainLengthInMs(randomGrainLengthInMs);
    granularEngine.setRandomGrainSpeed(randomGrainSpeed);
    granularEngine.setRandomGrainPan(randomGrainPan);
}

void AudioPluginAudioProcessor::setEnvelopeParameters(int type, float attack, float peak, float decay, float sustain, float release)
{
    granularEngine.setEnvelopeParameters(type, attack, peak, decay, sustain, release);
}

void AudioPluginAudioProcessor::setSampleParameters(float sampleStart, float sampleEnd)
{
    granularEngine.setRelativeSampleStart(sampleStart);
    granularEngine.setRelativeSampleEnd(sampleEnd);
}

// Get grain parameters

std::vector<std::tuple<float, float, float>> AudioPluginAudioProcessor::getGrainParameters()
{
    return granularEngine.getGrainParameters();
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this, formatManager);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
}

void AudioPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
