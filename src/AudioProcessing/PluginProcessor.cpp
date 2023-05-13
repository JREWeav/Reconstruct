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
      globalVTS(*this, nullptr, "GLOBAL_PARAMS", createGlobalParameterLayout()),
      granularVTS{AudioProcessorValueTreeState(*this, nullptr, "GRAIN_PARAMS1", createGranularParameterLayout(1)), AudioProcessorValueTreeState(*this, nullptr, "GRAIN_PARAMS2", createGranularParameterLayout(2))},
      granularEngines{GranularEngine(formatManager, globalVTS, granularVTS[0]), GranularEngine(formatManager, globalVTS, granularVTS[1])}
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
    granularEngines[0].prepareToPlay(sampleRate, samplesPerBlock);
    granularEngines[1].prepareToPlay(sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    granularEngines[0].releaseResources();
    granularEngines[1].releaseResources();
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

    granularEngines[0].processBlock(buffer, midiMessages);
    granularEngines[1].processBlock(buffer, midiMessages);
}

// Create AudioProcessorValueTreeState

AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createGlobalParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    // Samples Begin and End
    params.push_back(std::make_unique<AudioParameterFloat>("SAMPLE_START", "Sample Start", NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("SAMPLE_END", "Sample End", NormalisableRange<float>(0.0f, 1.0f), 0.0f));

    return {params.begin(), params.end()};
}

AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createGranularParameterLayout(int index)
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    String indexString = std::to_string(index);
    DBG(indexString);

    // On Off
    params.push_back(std::make_unique<AudioParameterBool>("ON_OFF" + indexString, "On/Off " + indexString, true));

    // Grain Density
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_DENSITY" + indexString, "Grain Density " + indexString, NormalisableRange<float>(0.1f, 5.0f), 0.1f));

    // Voices
    // params.push_back(std::make_unique<AudioParameterInt>("NUM_VOICES", "Number of Voices", 1, 1000, 1));

    // Grain Parameters
    params.push_back(std::make_unique<AudioParameterFloat>(
        "GRAIN_VOLUME" + indexString, "Grain Volume " + indexString, 0, 100, 50));

    params.push_back(std::make_unique<AudioParameterFloat>(
        "GRAIN_LENGTH" + indexString, "Grain Length " + indexString, 1, 500, 100));

    params.push_back(std::make_unique<AudioParameterFloat>(
        "GRAIN_SPEED" + indexString, "Grain Speed " + indexString, 1, 500, 100));

    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_PAN" + indexString, "Grain Pan " + indexString, 0, 100, 50));

    // Grain Randomization
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_VOLUME_RANDOMNESS" + indexString, "Grain Volume Randomization " + indexString, 0, 100, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_VOLUME_DIRECTION" + indexString, "Grain Volume Direction " + indexString, 0, 2, 1));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_LENGTH_RANDOMNESS" + indexString, "Grain Length Randomization " + indexString, 0, 500, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_LENGTH_DIRECTION" + indexString, "Grain Length Direction " + indexString, 0, 2, 1));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_SPEED_RANDOMNESS" + indexString, "Grain Speed Randomization " + indexString, 0, 300, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_SPEED_DIRECTION" + indexString, "Grain Speed Direction " + indexString, 0, 2, 1));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_PAN_RANDOMNESS" + indexString, "Grain Pan Randomization " + indexString, 0, 100, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_PAN_DIRECTION" + indexString, "Grain Pan Direction " + indexString, 0, 2, 1));

    // Grain Envelope
    StringArray envelopeTypes = {"ADSR", "ASR", "Hamming", "Hann", "Blackman", "White Noise"};
    params.push_back(std::make_unique<AudioParameterChoice>("ENVELOPE_TYPE" + indexString, "Envelope Type " + indexString, envelopeTypes, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_ATTACK" + indexString, "Grain Attack " + indexString, 0.0f, 1.0f, 0.2f));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_PEAK" + indexString, "Grain Peak " + indexString, 0.0f, 1.0f, 1.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_DECAY" + indexString, "Grain Decay " + indexString, 0.0f, 1.0f, 0.2f));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_SUSTAIN" + indexString, "Grain Sustain " + indexString, 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("GRAIN_RELEASE" + indexString, "Grain Release " + indexString, 0.0f, 1.0f, 0.2f));

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
