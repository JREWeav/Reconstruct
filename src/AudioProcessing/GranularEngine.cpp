#include "GranularEngine.h"

//==============================================================================

/*
TODO:
- Make all parameters controllable by the GUI
*/

GranularEngine::GranularEngine(juce::AudioFormatManager &formatManager)
    : AudioProcessor(BusesProperties()), formatManager(formatManager)
{
    timerForGrainGen = 0.0f;
    processedSamples = 0;

    // Random Parameters
    randomGrainVolume = 0;
    randomGrainLengthInMs = 0;
    randomGrainSpeed = 0;
    randomGrainPan = 0;
}

GranularEngine::~GranularEngine()
{
    delete sampleBuffer;
}

//==============================================================================

void GranularEngine::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    storedSampleRate = sampleRate;
}

void GranularEngine::releaseResources()
{
    grainPool.clear();
}

void GranularEngine::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages)
{
    // Your audio-processing code goes here!
    // For more details, see the help for AudioProcessor::processBlock()
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    float timePassed = round(((float)numSamples / storedSampleRate) * 1000);

    for (const MidiMessageMetadata metadata : midiMessages)
    {
        const MidiMessage message = metadata.getMessage();

        if (message.isNoteOn())
        {
            heldNotes.push_back(message);
            // Get the MIDI note number and velocity
            int midiNoteNumber = message.getNoteNumber();
            float velocity = message.getVelocity() / 127.0f;

            // Generate a new grain
            generateGrain(midiNoteNumber, velocity);
        }
        else if (metadata.getMessage().isNoteOff())
        {
            heldNotes.erase(std::remove_if(heldNotes.begin(), heldNotes.end(), [&](const MidiMessage &m)
                                           { return m.getNoteNumber() == message.getNoteNumber(); }),
                            heldNotes.end());
        }
    }

    for (int i = 0; i < heldNotes.size(); i++)
    {
        timerForGrainGen += timePassed;
        if (timerForGrainGen >= grainInterval)
        {
            int midiNoteNumber = heldNotes[i].getNoteNumber();
            float velocity = heldNotes[i].getVelocity() / 127.0f;
            generateGrain(midiNoteNumber, velocity);
            timerForGrainGen -= grainInterval;
        }
    }

    buffer.clear();
    // Loop through each Grain object

    for (int i = 0; i < grainPool.size(); ++i)
    {
        Grain *grain = grainPool[i];

        // Generate the grain's waveform
        if (grain->getGrainPlaybackPositionInSamples() == 0)
        {
            grain->setGrainSampleRate(storedSampleRate);
            grain->initGrain(sampleBuffer);
        }

        grain->updateGrain(buffer, sampleBuffer);

        if (grain->getGrainPlaybackPositionInSamples() >= grain->getGrainLengthInSamples())
        {
            grainPool.erase(std::remove(grainPool.begin(), grainPool.end(), grain), grainPool.end());
            --i;
        }
    }

    processedSamples += numSamples;
}

void GranularEngine::generateGrain(int midiNoteNumber, float velocity)
{
    Grain *grain = new Grain();
    grain->setGrainPitch(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));

    // Set the grain's start position based off of waveform
    if (sampleStart == sampleEnd)
    {
        grain->setGrainStartPosition(sampleStart);
    }
    else
    {
        float grainStartPosition = (sampleEnd - sampleStart) * random.nextFloat() / (1 + sampleStart);
        grain->setGrainStartPosition(grainStartPosition);
    }

    // Implement randomness
    float newGrainVolume = grainVolume + ((random.nextFloat() * randomGrainVolume * 2.0f) - randomGrainVolume);
    newGrainVolume = jmax(0, (int)(jmin(150, (int)newGrainVolume)));
    int newGrainLengthInMs = grainLengthInMs + ((int)ceil(((random.nextFloat() * (float)randomGrainLengthInMs * 2.0f))) - randomGrainLengthInMs);
    newGrainLengthInMs = jmax(0, newGrainLengthInMs);
    float newGrainSpeed = grainSpeed + ((random.nextFloat() * randomGrainSpeed * 2.0f) - randomGrainSpeed);
    newGrainSpeed = jmax(0.0f, newGrainSpeed);
    float newGrainPan = grainPan + ((random.nextFloat() * randomGrainPan * 2.0f) - randomGrainPan);
    newGrainPan = jmax(0.0f, jmin(100.0f, newGrainPan));

    grain->setGrainLengthInMs(newGrainLengthInMs);
    grain->setGrainPan(newGrainPan);
    grain->setGrainSpeed(newGrainSpeed);
    grain->setGrainVolume(newGrainVolume);
    grainPool.push_back(grain);
}

//==============================================================================

bool GranularEngine::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor *GranularEngine::createEditor()
{
    return nullptr;
}

//==============================================================================

// Sample parameters
void GranularEngine::setRelativeSampleStart(float start)
{
    sampleStart = start;
}
void GranularEngine::setRelativeSampleEnd(float end)
{
    sampleEnd = end;
}

// Grains per second
void GranularEngine::setGrainsPerSecond(float hz)
{
    grainsPerSecond = hz;
    grainInterval = 1000 / grainsPerSecond;
}

// Grain parameters
void GranularEngine::setGrainVolume(float _grainVolume)
{
    grainVolume = _grainVolume;
}
void GranularEngine::setGrainLengthInMs(int _grainLengthInMs)
{
    grainLengthInMs = _grainLengthInMs;
}
void GranularEngine::setGrainPan(float _grainPan)
{
    grainPan = _grainPan;
}
void GranularEngine::setGrainSpeed(float _grainSpeed)
{
    grainSpeed = _grainSpeed;
}

// Randomness parameters

void GranularEngine::setRandomGrainVolume(float _randomGrainVolume)
{
    randomGrainVolume = _randomGrainVolume;
}
void GranularEngine::setRandomGrainLengthInMs(int _randomGrainLengthInMs)
{
    randomGrainLengthInMs = _randomGrainLengthInMs;
}
void GranularEngine::setRandomGrainPan(float _randomGrainPan)
{
    randomGrainPan = _randomGrainPan;
}
void GranularEngine::setRandomGrainSpeed(float _randomGrainSpeed)
{
    randomGrainSpeed = _randomGrainSpeed;
}

//==============================================================================

void GranularEngine::loadSampleFromUrl(juce::URL &url)
{
    juce::AudioFormatReader *reader = formatManager.createReaderFor(url.createInputStream(false));
    if (reader != nullptr)
    {
        sampleBuffer = new juce::AudioSampleBuffer(reader->numChannels, reader->lengthInSamples);
        reader->read(sampleBuffer, 0, reader->lengthInSamples, 0, true, true);
        delete reader;
    }
}

//==============================================================================

void GranularEngine::getStateInformation(MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void GranularEngine::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================

bool GranularEngine::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    juce::ignoreUnused(layouts);
    return true;
}

const juce::String GranularEngine::getName() const
{
    return "GranularEngine";
}

bool GranularEngine::acceptsMidi() const
{
    return true;
}

bool GranularEngine::producesMidi() const
{
    return false;
}

bool GranularEngine::isMidiEffect() const
{
    return false;
}

double GranularEngine::getTailLengthSeconds() const
{
    return 0.0;
}

int GranularEngine::getNumPrograms()
{
    return 1;
}

int GranularEngine::getCurrentProgram()
{
    return 0;
}

void GranularEngine::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String GranularEngine::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void GranularEngine::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}
