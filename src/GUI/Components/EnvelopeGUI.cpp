#include "EnvelopeGUI.h"

EnvelopeGUI::EnvelopeGUI(AudioProcessorValueTreeState &vts) : vts(vts)
{
    addAndMakeVisible(envelopeType);
    envelopeType.addListener(this);
    envelopeTypeAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(vts, "ENVELOPE_TYPE", envelopeType);
    envelopeType.addItemList({"ADSR", "ASR", "Hamming", "Hann", "Blackman", "White Noise"}, 1);
    envelopeType.setSelectedId(vts.getRawParameterValue("ENVELOPE_TYPE")->load() + 1);
    envelope.type = envelopeType.getSelectedId();

    firstCall = true;
    selectedPoint = nullptr;
    recallVTS();
}

EnvelopeGUI::~EnvelopeGUI()
{
}

void EnvelopeGUI::paint(juce::Graphics &g)
{
    if (envelope.type == 1 || envelope.type == 2)
    {

        g.fillAll(juce::Colours::aliceblue);
        height = getHeight();
        heightOffset = 21;
        if (!isCollapsed)
        {
            g.setColour(juce::Colours::firebrick);
            g.fillRect(0, 0, (int)getWidth(), (int)heightOffset - 4);
            g.setColour(juce::Colours::aliceblue);
            g.drawLine(getWidth() - 17, 2, getWidth() - 2, 15, 2);
            g.drawLine(getWidth() - 2, 2, getWidth() - 17, 15, 2);
        }

        g.setColour(juce::Colours::red);
        if (isCollapsed)
        {
            envelopeType.setBounds(0, 0, getWidth(), getHeight() / 5);
            heightOffset = (getHeight() / 5) + 4;
            height -= heightOffset;
        }
        calculatePoints();
        if (envelopeType.getSelectedId() == 1)
        {

            g.fillEllipse(attackPoint.x, attackPoint.y, 8, 8);

            g.fillEllipse(decayPoint.x, decayPoint.y, 8, 8);

            g.fillEllipse(releasePoint.x, releasePoint.y, 8, 8);

            g.drawLine(0, height + heightOffset, envelope.attack * getWidth(), ((1.0f - envelope.peak) * height) + heightOffset, 2);
            g.drawLine(envelope.attack * getWidth(), ((1.0f - envelope.peak) * height) + heightOffset, (envelope.attack + envelope.decay) * getWidth(), ((1.0f - envelope.sustain) * height) + heightOffset, 2);
            g.drawLine((envelope.attack + envelope.decay) * getWidth(), ((1.0f - envelope.sustain) * height) + heightOffset, (1.0f - envelope.release) * getWidth(), ((1.0f - envelope.sustain) * height) + heightOffset, 2);
            g.drawLine((1.0f - envelope.release) * getWidth(), ((1.0f - envelope.sustain) * height) + heightOffset, getWidth(), height + heightOffset, 2);
        }
        else if (envelopeType.getSelectedId() == 2)
        {

            g.fillEllipse(attackPoint.x, attackPoint.y, 8, 8);

            g.fillEllipse(releasePoint.x, releasePoint.y, 8, 8);

            g.drawLine(0, height + heightOffset, envelope.attack * getWidth(), ((1.0f - envelope.sustain) * height) + heightOffset, 2);
            g.drawLine(envelope.attack * getWidth(), ((1.0f - envelope.sustain) * height) + heightOffset, (1.0f - envelope.release) * getWidth(), ((1.0f - envelope.sustain) * height) + heightOffset, 2);
            g.drawLine((1.0f - envelope.release) * getWidth(), ((1.0f - envelope.sustain) * height) + heightOffset, getWidth(), height + heightOffset, 2);
        }
    }
    else
    {
        g.fillAll(juce::Colours::transparentWhite);
        envelopeType.setBounds(0, ((getHeight() / 2) - (getHeight() / 5)), getWidth(), getHeight() / 5);
    }
}

void EnvelopeGUI::resized()
{
    if (isCollapsed)
    {
        if (envelope.type == 1 || envelope.type == 2)
        {
            envelopeType.setBounds(0, 0, getWidth(), getHeight() / 5);
        }
        else
        {
            envelopeType.setBounds(0, ((getHeight() / 2) - (getHeight() / 5)), getWidth(), getHeight() / 5);
        }
    }
    else
    {
        envelopeType.setBounds(0, 0, 0, 0);
    }
}

void EnvelopeGUI::calculatePoints()
{
    if (envelopeType.getSelectedId() == 1)
    {
        attackPoint.x = envelope.attack * getWidth() - 4;
        attackPoint.y = ((1.0f - envelope.peak) * height - 4) + heightOffset;

        decayPoint.x = (envelope.attack + envelope.decay) * getWidth() - 4;
        decayPoint.y = ((1.0f - envelope.sustain) * height - 4) + heightOffset;

        releasePoint.x = (1.0f - envelope.release) * getWidth() - 4;
        releasePoint.y = ((1.0f - envelope.sustain) * height - 4) + heightOffset;
    }
    if (envelopeType.getSelectedId() == 2)
    {
        attackPoint.x = envelope.attack * getWidth() - 4;
        attackPoint.y = ((1.0f - envelope.sustain) * height - 4) + heightOffset;

        releasePoint.x = (1.0f - envelope.release) * getWidth() - 4;
        releasePoint.y = ((1.0f - envelope.sustain) * height - 4) + heightOffset;
    }
}

void EnvelopeGUI::comboBoxChanged(juce::ComboBox *comboBox)
{
    if (firstCall)
    {
        firstCall = false;
        return;
    }
    if (comboBox == &envelopeType)
    {
        if (envelopeType.getSelectedId() == 1)
        {
            envelope.attack = 0.2f;
            envelope.peak = 1.0f;
            envelope.decay = 0.2f;
            envelope.sustain = 0.5f;
            envelope.release = 0.2f;
        }
        else if (envelopeType.getSelectedId() == 2)
        {
            envelope.attack = 0.2f;
            envelope.sustain = 0.5f;
            envelope.release = 0.2f;
        }
        envelope.type = envelopeType.getSelectedId();
        repaint();
        sendChangeMessage();
    }
}

void EnvelopeGUI::mouseDown(const juce::MouseEvent &event)
{
    if (envelope.type != 1 && envelope.type != 2)
        return;

    if (isCollapsed)
    {
        if (event.getMouseDownY() > getHeight() / 5)
        {
            isCollapsed = false;
            sendChangeMessage();
        }
    }
    else
    {
        if (event.getMouseDownY() < 20 && event.getMouseDownX() > getWidth() - 20)
        {
            isCollapsed = true;
            sendChangeMessage();
        }
    }
}
void EnvelopeGUI::mouseDrag(const juce::MouseEvent &event)
{
    if (!isCollapsed)
    {
        if (envelopeType.getSelectedId() == 1)
        {
            if ((getMouseXYRelative().getX() > attackPoint.x - 15 && getMouseXYRelative().getX() < attackPoint.x + 15 && getMouseXYRelative().getY() > attackPoint.y - 15 && getMouseXYRelative().getY() < attackPoint.y + 15 && selectedPoint == nullptr) || selectedPoint == &attackPoint)
            {
                selectedPoint = &attackPoint;
                envelope.attack = (float)getMouseXYRelative().getX() / (float)getWidth();
                envelope.attack = jmax(0.0f, envelope.attack);
                envelope.attack = jmin(1.0f - envelope.release - envelope.decay, envelope.attack);
                envelope.peak = 1.0f - ((float)getMouseXYRelative().getY() - heightOffset) / height;
                envelope.peak = jmax(envelope.sustain, envelope.peak);
                envelope.peak = jmin(1.0f, envelope.peak);
            }
            else if ((getMouseXYRelative().getX() > decayPoint.x - 15 && getMouseXYRelative().getX() < decayPoint.x + 15 && getMouseXYRelative().getY() > decayPoint.y - 15 && getMouseXYRelative().getY() < decayPoint.y + 15 && selectedPoint == nullptr) || selectedPoint == &decayPoint)
            {
                selectedPoint = &decayPoint;
                envelope.decay = ((float)getMouseXYRelative().getX() / (float)getWidth()) - envelope.attack;
                envelope.decay = jmax(0.0f, envelope.decay);
                envelope.decay = jmin(1.0f - envelope.attack - envelope.release, envelope.decay);
                envelope.sustain = 1.0f - ((float)getMouseXYRelative().getY() - heightOffset) / height;
                envelope.sustain = jmax(0.0f, envelope.sustain);
                envelope.sustain = jmin(envelope.peak, envelope.sustain);
            }
            else if ((getMouseXYRelative().getX() > releasePoint.x - 15 && getMouseXYRelative().getX() < releasePoint.x + 15 && getMouseXYRelative().getY() > releasePoint.y - 15 && getMouseXYRelative().getY() < releasePoint.y + 15 && selectedPoint == nullptr) || selectedPoint == &releasePoint)
            {
                selectedPoint = &releasePoint;
                envelope.release = 1.0f - (float)getMouseXYRelative().getX() / (float)getWidth();
                envelope.release = jmax(0.0f, envelope.release);
                envelope.release = jmin(1.0f - envelope.attack - envelope.decay, envelope.release);
                envelope.sustain = 1.0f - ((float)getMouseXYRelative().getY() - heightOffset) / height;
                envelope.sustain = jmax(0.0f, envelope.sustain);
                envelope.sustain = jmin(envelope.peak, envelope.sustain);
            }
        }
        else if (envelopeType.getSelectedId() == 2)
        {
            if ((getMouseXYRelative().getX() > attackPoint.x - 15 && getMouseXYRelative().getX() < attackPoint.x + 15 && getMouseXYRelative().getY() > attackPoint.y - 15 && getMouseXYRelative().getY() < attackPoint.y + 15 && selectedPoint == nullptr) || selectedPoint == &attackPoint)
            {
                selectedPoint = &attackPoint;
                envelope.attack = (float)getMouseXYRelative().getX() / (float)getWidth();
                envelope.attack = jmax(0.0f, envelope.attack);
                envelope.attack = jmin(1.0f - envelope.release, envelope.attack);
                envelope.sustain = 1.0f - ((float)getMouseXYRelative().getY() - heightOffset) / height;
                envelope.sustain = jmax(0.0f, envelope.sustain);
                envelope.sustain = jmin(1.0f, envelope.sustain);
            }
            else if ((getMouseXYRelative().getX() > releasePoint.x - 15 && getMouseXYRelative().getX() < releasePoint.x + 15 && getMouseXYRelative().getY() > releasePoint.y - 15 && getMouseXYRelative().getY() < releasePoint.y + 5 && selectedPoint == nullptr) || selectedPoint == &releasePoint)
            {
                selectedPoint = &releasePoint;
                envelope.release = 1.0f - (float)getMouseXYRelative().getX() / (float)getWidth();
                envelope.release = jmax(0.0f, envelope.release);
                envelope.release = jmin(1.0f - envelope.attack, envelope.release);
                envelope.sustain = 1.0f - ((float)getMouseXYRelative().getY() - heightOffset) / height;
                envelope.sustain = jmax(0.0f, envelope.sustain);
                envelope.sustain = jmin(1.0f, envelope.sustain);
            }
        }
        repaint();
        sendChangeMessage();
    }
}

void EnvelopeGUI::mouseUp(const juce::MouseEvent &event)
{
    selectedPoint = nullptr;
    updateVTS();
}

// Update VTS
void EnvelopeGUI::updateVTS()
{
    // Update VTS with new values
    vts.getParameter("GRAIN_ATTACK")->beginChangeGesture();
    vts.getParameter("GRAIN_ATTACK")->setValueNotifyingHost(envelope.attack);
    vts.getParameter("GRAIN_ATTACK")->endChangeGesture();

    vts.getParameter("GRAIN_PEAK")->beginChangeGesture();
    vts.getParameter("GRAIN_PEAK")->setValueNotifyingHost(envelope.peak);
    vts.getParameter("GRAIN_PEAK")->endChangeGesture();

    vts.getParameter("GRAIN_DECAY")->beginChangeGesture();
    vts.getParameter("GRAIN_DECAY")->setValueNotifyingHost(envelope.decay);
    vts.getParameter("GRAIN_DECAY")->endChangeGesture();

    vts.getParameter("GRAIN_SUSTAIN")->beginChangeGesture();
    vts.getParameter("GRAIN_SUSTAIN")->setValueNotifyingHost(envelope.sustain);
    vts.getParameter("GRAIN_SUSTAIN")->endChangeGesture();

    vts.getParameter("GRAIN_RELEASE")->beginChangeGesture();
    vts.getParameter("GRAIN_RELEASE")->setValueNotifyingHost(envelope.release);
    vts.getParameter("GRAIN_RELEASE")->endChangeGesture();
}

void EnvelopeGUI::recallVTS()
{
    envelope.attack = vts.getParameter("GRAIN_ATTACK")->getValue();
    envelope.peak = vts.getParameter("GRAIN_PEAK")->getValue();
    envelope.decay = vts.getParameter("GRAIN_DECAY")->getValue();
    envelope.sustain = vts.getParameter("GRAIN_SUSTAIN")->getValue();
    envelope.release = vts.getParameter("GRAIN_RELEASE")->getValue();
}
// Getters

bool EnvelopeGUI::getCollapseState()
{
    return isCollapsed;
}

int EnvelopeGUI::getEnvelopeType()
{
    return envelope.type;
}

float EnvelopeGUI::getAttack()
{
    return envelope.attack;
}

float EnvelopeGUI::getPeak()
{
    return envelope.peak;
}

float EnvelopeGUI::getDecay()
{
    return envelope.decay;
}

float EnvelopeGUI::getSustain()
{
    return envelope.sustain;
}

float EnvelopeGUI::getRelease()
{
    return envelope.release;
}
