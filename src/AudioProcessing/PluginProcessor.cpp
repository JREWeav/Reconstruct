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
                         ),
      vts(*this, nullptr, "PARAMETERS", createParameterLayout()), granularEngines{GranularEngine(formatManager, vts)}
{
    formatManager.registerBasicFormats();
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
    formatManager.clearFormats();
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
    granularEngines->prepareToPlay(sampleRate, samplesPerBlock);
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

    granularEngines->processBlock(buffer, midiMessages);
}

// Create AudioProcessorValueTreeState

AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    // Samples Begin and End
    params.push_back(std::make_unique<AudioParameterFloat>("SAMPLE_START", "Sample Start", NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("SAMPLE_END", "Sample End", NormalisableRange<float>(0.0f, 1.0f), 0.0f));

    // Grain Density
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_DENSITY", "Grain Density", NormalisableRange<float>(0.1f, 5.0f), 0.1f));

    // Voices
    // params.push_back(std::make_unique<AudioParameterInt>("NUM_VOICES", "Number of Voices", 1, 1000, 1));

    // Grain Parameters
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_VOLUME", "Grain Volume", 0, 100, 50));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_LENGTH", "Grain Length", 20, 500, 100));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_SPEED", "Grain Speed", 1, 500, 100));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_PAN", "Grain Pan", 0, 100, 50));

    // Grain Randomization
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_VOLUME_RANDOMNESS", "Grain Volume Randomization", 0, 100, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_LENGTH_RANDOMNESS", "Grain Length Randomization", 0, 500, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_SPEED_RANDOMNESS", "Grain Speed Randomization", 0, 300, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_PAN_RANDOMNESS", "Grain Pan Randomization", 0, 100, 0));

    // Grain Envelope
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_ATTACK", "Grain Attack", 0, 100, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_DECAY", "Grain Decay", 0, 100, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_SUSTAIN", "Grain Sustain", 0, 100, 100));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_RELEASE", "Grain Release", 0, 100, 0));

    return {params.begin(), params.end()};
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this, granularEngines, formatManager);
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
