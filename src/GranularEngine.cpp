#include "GranularEngine.h"

//==============================================================================

GranularEngine::GranularEngine(juce::AudioFormatManager &formatManager)
    : AudioProcessor(BusesProperties()), formatManager(formatManager)
{
    timerForGrainGen = 0.0f;
    grainDensityInHz = 1.0f;
    processedSamples = 0;
}

GranularEngine::~GranularEngine()
{
    delete sampleBuffer;
}

void GranularEngine::generateGrain(int midiNoteNumber, float velocity)
{
    Grain *grain = new Grain();
    grain->setGrainPitch(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    grain->setGrainStartPosition(0.2);
    grain->setGrainLengthInMs(200);
    grain->setGrainPan(0.5);
    grain->setGrainVolume(0.5);
    grainPool.push_back(grain);
}

//==============================================================================

void GranularEngine::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (int i = 0; i < grainPool.size(); i++)
    {
        grainPool[i]->setGrainSampleRate(sampleRate);
    }
    storedSampleRate = sampleRate;
}

void GranularEngine::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void GranularEngine::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages)
{
    // Your audio-processing code goes here!
    // For more details, see the help for AudioProcessor::processBlock()
    const int numSamples = buffer.getNumSamples();
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

            DBG("ENGINE! MIDI Note: " + String(midiNoteNumber) + " Velocity: " + String(velocity));

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
        if (grain->getGrainPlaybackPosition() == 0)
        {
            grain->setGrainSampleRate(storedSampleRate);
            grain->initGrain(sampleBuffer);
        }

        int grainNumSamples = grain->getGrainBuffer()->getNumSamples();
        int grainPlaybackPosition = grain->getGrainPlaybackPosition();

        int playbackAmount = grainNumSamples - grainPlaybackPosition;
        playbackAmount = jmin(playbackAmount, numSamples);

        // Mix the grain's waveform with the output buffer
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            buffer.addFrom(channel, 0, *(grain->getGrainBuffer()), channel, grainPlaybackPosition, playbackAmount, 0.3f);
        }

        grain->setGrainPlaybackPosition(grainPlaybackPosition + numSamples);

        grainPlaybackPosition = grain->getGrainPlaybackPosition();
        if (grainPlaybackPosition >= grainNumSamples)
        {
            // Remove the grain from the pool
            grain->getGrainBuffer()->clear();
            grainPool.erase(std::remove(grainPool.begin(), grainPool.end(), grain), grainPool.end());
            delete grain;
            i--; // Decrement the loop counter to account for the removed grain
        }
    }

    processedSamples += numSamples;
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

void GranularEngine::setGrainDensityInHz(float density)
{
    grainDensityInHz = density;
    grainInterval = 1000 / grainDensityInHz;
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

void GranularEngine::deleteGrain(Grain *grain)
{
    grainPool.erase(std::remove(grainPool.begin(), grainPool.end(), grain), grainPool.end());
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
    return {};
    juce::ignoreUnused(index);
}

void GranularEngine::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}
