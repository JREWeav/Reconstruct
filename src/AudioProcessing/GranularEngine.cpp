#include "GranularEngine.h"

//==============================================================================

GranularEngine::GranularEngine(AudioFormatManager &formatManager, AudioProcessorValueTreeState &vts)
    : AudioProcessor(BusesProperties()), formatManager(formatManager), vts(vts)
{
    processedSamples = 0;

    grainDensity = 0;
    grainTimerInSamples = 0;

    // Random Parameters
    randomGrainVolume = 0;
    randomGrainLengthInMs = 0;
    randomGrainSpeed = 0;
    randomGrainPan = 0;

    fileLoaded = false;
}

GranularEngine::~GranularEngine()
{
    if (sampleBuffer != nullptr)
    {
        sampleBuffer->clear();
    }
}

//==============================================================================

void GranularEngine::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    storedSampleRate = sampleRate;
    juce::ignoreUnused(samplesPerBlock);
}

void GranularEngine::releaseResources()
{
    grainPool.clear();
}

void GranularEngine::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages)
{
    if (!fileLoaded)
        return;

    // Get the number of samples in the buffer
    storedBufferSize = buffer.getNumSamples();
    // Clear the buffer
    buffer.clear();
    // Get the number of samples in the buffer
    const int numSamples = buffer.getNumSamples();
    int samplesToProcess = numSamples;

    int grainIntervalInSamples = storedSampleRate / ((grainDensity * 1000.0f) / grainLengthInMs);

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
            generateGrain(midiNoteNumber, velocity, 0);
        }
        else if (metadata.getMessage().isNoteOff())
        {
            heldNotes.erase(std::remove_if(heldNotes.begin(), heldNotes.end(), [&](const MidiMessage &m)
                                           { return m.getNoteNumber() == message.getNoteNumber(); }),
                            heldNotes.end());
        }
    }

    for (int i = 0; i < numSamples; i++)
    {
        if (grainIntervalInSamples > grainTimerInSamples)
        {
            grainTimerInSamples++;
        }
        else
        {
            for (int j = 0; j < heldNotes.size(); j++)
            {
                int midiNoteNumber = heldNotes[j].getNoteNumber();
                float velocity = heldNotes[j].getVelocity() / 127.0f;
                generateGrain(midiNoteNumber, velocity, i);
            }
            grainTimerInSamples -= grainIntervalInSamples;
        }
    }
    if (grainPool.size() > 0)
    {
        processActiveGrains(numSamples, buffer);
    }
    sendChangeMessage();
}

void GranularEngine::processActiveGrains(int numSamples, AudioSampleBuffer &buffer)
{
    // Loop through each Grain object
    for (int i = 0; i < grainPool.size(); ++i)
    {
        Grain *grain = grainPool[i];

        // Generate the grain's waveform
        if (grain->getGrainPlaybackPositionInSamples() == 0)
        {
            grain->setGrainSampleRate(storedSampleRate);
            grain->initGrain(sampleBuffer.get(), envelope.type, envelope.attack, envelope.peak, envelope.decay, envelope.sustain, envelope.release);
        }

        grain->updateGrain(numSamples, buffer, sampleBuffer.get());

        // Check if the grain has finished playing
        if (grain->getGrainPlaybackPositionInSamples() >= grain->getGrainLengthInSamples())
        {
            grainPool.erase(std::remove(grainPool.begin(), grainPool.end(), grain), grainPool.end());
            --i;
        }
    }
}

void GranularEngine::generateGrain(int midiNoteNumber, float velocity, int offsetInSamples)
{
    Grain *grain = new Grain();
    grain->setGrainPitch((float)juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));

    // Set the grain's start position based off of waveform
    if (sampleStart == sampleEnd)
    {
        grain->setGrainStartPosition(sampleStart);
    }
    else
    {
        float grainStartPosition = (sampleEnd - sampleStart) * random.nextFloat() + sampleStart;
        grain->setGrainStartPosition(grainStartPosition);
    }

    // Implement randomness
    // Randomness for volume
    float newGrainVolume = grainVolume + (random.nextFloat() * randomGrainVolume);
    if (randomGrainVolumeState == 2)
    {
        newGrainVolume = grainVolume + -1.0f * ((random.nextFloat() * randomGrainVolume));
    }
    else if (randomGrainVolumeState == 1)
    {
        newGrainVolume = grainVolume + ((random.nextFloat() * randomGrainVolume * 2.0f) - randomGrainVolume);
    }
    newGrainVolume = (float)jmax(0, (int)(jmin(150, (int)newGrainVolume)));

    // Randomness for Length
    int newGrainLengthInMs = grainLengthInMs + (int)ceil(random.nextFloat() * (float)randomGrainLengthInMs);
    if (randomGrainLengthState == 2)
    {
        newGrainLengthInMs = grainLengthInMs + (-1.0f * (int)ceil(random.nextFloat() * (float)randomGrainLengthInMs));
    }
    else if (randomGrainLengthState == 1)
    {
        newGrainLengthInMs = grainLengthInMs + ((int)ceil(((random.nextFloat() * (float)randomGrainLengthInMs * 2.0f))) - randomGrainLengthInMs);
    }
    newGrainLengthInMs = jmax(0, newGrainLengthInMs);

    // Randomness for Speed
    float newGrainSpeed = grainSpeed + ((random.nextFloat() * randomGrainSpeed));
    if (randomGrainSpeedState == 2)
    {
        newGrainSpeed = grainSpeed + (-1.0f * ((random.nextFloat() * randomGrainSpeed)));
    }
    else if (randomGrainSpeedState == 1)
    {
        newGrainSpeed = grainSpeed + ((random.nextFloat() * randomGrainSpeed * 2.0f) - randomGrainSpeed);
    }
    newGrainSpeed = jmax(0.0f, newGrainSpeed);

    // Randomness for Pan
    float newGrainPan = grainPan + ((random.nextFloat() * randomGrainPan));
    if (randomGrainPanState == 2)
    {
        newGrainPan = grainPan + (-1 * ((random.nextFloat() * randomGrainPan)));
    }
    else if (randomGrainPanState == 1)
    {
        newGrainPan = grainPan + ((random.nextFloat() * randomGrainPan * 2.0f) - randomGrainPan);
    }
    newGrainPan = jmax(0.0f, jmin(100.0f, newGrainPan));

    if (newGrainLengthInMs == 0 || newGrainSpeed == 0 || newGrainVolume == 0)
        return;

    grain->setGrainOffsetInSamples(offsetInSamples);
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
void GranularEngine::setGrainDensity(float _grainDensity)
{
    grainDensity = _grainDensity;
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

void GranularEngine::setRandomGrainVolume(float _randomGrainVolume, int dir)
{
    randomGrainVolume = _randomGrainVolume;
    randomGrainVolumeState = dir;
}
void GranularEngine::setRandomGrainLengthInMs(int _randomGrainLengthInMs, int dir)
{
    randomGrainLengthInMs = _randomGrainLengthInMs;
    randomGrainLengthState = dir;
}
void GranularEngine::setRandomGrainPan(float _randomGrainPan, int dir)
{
    randomGrainPan = _randomGrainPan;
    randomGrainPanState = dir;
}
void GranularEngine::setRandomGrainSpeed(float _randomGrainSpeed, int dir)
{
    randomGrainSpeed = _randomGrainSpeed;
    randomGrainSpeedState = dir;
}

// Envelope
void GranularEngine::setEnvelopeParameters(int type, float attack, float peak, float decay, float sustain, float release)
{
    envelope.type = type;
    envelope.attack = attack;
    envelope.peak = peak;
    envelope.decay = decay;
    envelope.sustain = sustain;
    envelope.release = release;
}

// Getter for grain pool
std::vector<std::tuple<float, float, float>> GranularEngine::getGrainParameters()
{
    std::vector<std::tuple<float, float, float>> grainParameters;
    for (int i = 0; i < grainPool.size(); i++)
    {
        Grain *grain = grainPool[i];
        if (grain->getGrainLengthInSamples() > storedBufferSize * 2)
        {
            grainParameters.push_back(std::make_tuple(grain->getGrainCurrentRelativePosition(), grain->getGrainCurrentVolume(), grain->getGrainPan()));
        }
        else
        {
            grainParameters.push_back(std::make_tuple(grain->getGrainCurrentRelativePosition(), grain->getGrainMaxVolume(), grain->getGrainPan()));
        }
    }
    return grainParameters;
}

// Getter for SampleRate
int GranularEngine::getStoredSampleRate()
{
    return storedSampleRate;
}
//==============================================================================

void GranularEngine::loadSampleFromUrl(juce::URL &url)
{
    juce::AudioFormatReader *reader = formatManager.createReaderFor(url.createInputStream(false));
    if (reader != nullptr)
    {
        sampleBuffer.reset(new juce::AudioBuffer<float>(reader->numChannels, reader->lengthInSamples));
        reader->read(sampleBuffer.get(), 0, reader->lengthInSamples, 0, true, true);
        delete reader;
        fileLoaded = true;
    }
}

AudioSampleBuffer *GranularEngine::getSampleBuffer()
{
    return sampleBuffer.get();
}

bool GranularEngine::isFileLoaded()
{
    return fileLoaded;
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
