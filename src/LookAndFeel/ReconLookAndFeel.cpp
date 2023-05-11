#include "ReconLookAndFeel.h"

//----------------------------------------------------------------------------------------------

ReconLookAndFeel::ReconLookAndFeel()
{
}

ReconLookAndFeel::~ReconLookAndFeel()
{
}

//----------------------------------------------------------------------------------------------

void ReconLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                                        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &)
{
    // Modified from original JUCE Code
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin(8.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;

    // fill
    Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                rotaryEndAngle,
                                true);

    g.setColour(juce::Colours::aliceblue);
    g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::square));

    juce::Path p;
    auto pointerLength = jmin(8.0f, radius * 0.5f);
    auto pointerThickness = 4.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    // pointer
    g.setColour(juce::Colours::red);
    g.fillPath(p);
}

void ReconLookAndFeel::drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox &box)
{
    auto cornerSize = box.findParentComponentOfClass<juce::GroupComponent>() != nullptr ? 0.0f : 3.0f;
    auto bounds = juce::Rectangle<int>(0, 0, width, height).toFloat().reduced(1);
    g.setColour(juce::Colours::firebrick);
    g.fillRect(bounds);

    auto arrowX = 0.3f;
    auto arrowH = 0.2f;

    Path p;
    p.addTriangle(bounds.getX() + bounds.getWidth() * (1.0f - arrowX),
                  bounds.getY() + bounds.getHeight() * 0.5f - bounds.getHeight() * arrowH * 0.5f,
                  bounds.getX() + bounds.getWidth() * (1.0f - arrowX),
                  bounds.getY() + bounds.getHeight() * 0.5f + bounds.getHeight() * arrowH * 0.5f,
                  bounds.getX() + bounds.getWidth() * (1.0f - arrowX * 0.6f),
                  bounds.getY() + bounds.getHeight() * 0.5f);

    g.setColour(juce::Colours::aliceblue);
    g.fillPath(p);
}

Font ReconLookAndFeel::getComboBoxFont(ComboBox &box)
{
    return {jmin(16.0f, (float)box.getHeight() * 0.85f)};
}

void ReconLookAndFeel::positionComboBoxText(ComboBox &box, Label &label)
{
    label.setColour(Label::textColourId, Colours::aliceblue);
    label.setBounds(1, 1,
                    box.getWidth() - 30,
                    box.getHeight() - 2);

    label.setFont(getComboBoxFont(box));
}

void ReconLookAndFeel::drawPopupMenuBackground(Graphics &g, int width, int height)
{
    g.fillAll(Colours::firebrick);
}

void ReconLookAndFeel::drawPopupMenuItem(Graphics &g, const Rectangle<int> &area,
                                         bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                                         const String &text, const String &shortcutKeyText,
                                         const Drawable *icon, const Colour *textColour)
{
    // Modified from original JUCE Code
    if (isSeparator)
    {
        auto r = area.reduced(5, 0);
        r.removeFromTop(roundToInt(((float)r.getHeight() * 0.5f) - 0.5f));

        g.setColour(juce::Colours::aliceblue);
        g.fillRect(r.removeFromTop(1));
    }
    else
    {
        auto textColour = juce::Colours::aliceblue;

        auto r = area.reduced(1);

        if (isHighlighted && isActive)
        {
            g.setColour(juce::Colours::red);
            g.fillRect(r);

            g.setColour(juce::Colours::aliceblue);
        }
        else
        {
            g.setColour(textColour.withMultipliedAlpha(isActive ? 1.0f : 0.5f));
        }

        r.reduce(jmin(5, area.getWidth() / 20), 0);

        auto font = getPopupMenuFont();

        auto maxFontHeight = (float)r.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight(maxFontHeight);

        g.setFont(font);

        auto iconArea = r.removeFromLeft(roundToInt(maxFontHeight)).toFloat();

        if (icon != nullptr)
        {
            icon->drawWithin(g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
            r.removeFromLeft(roundToInt(maxFontHeight * 0.5f));
        }
        else if (isTicked)
        {
            auto tick = getTickShape(1.0f);
            g.setColour(juce::Colours::aliceblue);
            g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
        }

        if (hasSubMenu)
        {
            auto arrowH = 0.6f * getPopupMenuFont().getAscent();

            auto x = static_cast<float>(r.removeFromRight((int)arrowH).getX());
            auto halfH = static_cast<float>(r.getCentreY());

            Path path;
            path.startNewSubPath(x, halfH - arrowH * 0.5f);
            path.lineTo(x + arrowH * 0.6f, halfH);
            path.lineTo(x, halfH + arrowH * 0.5f);

            g.strokePath(path, PathStrokeType(2.0f));
        }

        r.removeFromRight(3);
        g.drawFittedText(text, r, Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            auto f2 = font;
            f2.setHeight(f2.getHeight() * 0.75f);
            f2.setHorizontalScale(0.95f);
            g.setFont(f2);

            g.drawText(shortcutKeyText, r, Justification::centredRight, true);
        }
    }
}

void ReconLookAndFeel::drawButtonBackground(Graphics &g, Button &button, const Colour &backgroundColour,
                                            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    // Modified Juce Code

    auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
    auto colour = juce::Colours::firebrick;

    if (button.isMouseOver())
        colour = juce::Colours::red;

    g.setColour(colour);

    g.fillRect(bounds);
}

void ReconLookAndFeel::drawToggleButton(Graphics &g, ToggleButton &button,
                                        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    // Modified Juce Code
    auto fontSize = jmin(14.0f, (float)button.getHeight() * 0.75f);
    auto tickWidth = fontSize * 1.1f;

    float tickX = button.getWidth() / 2 - tickWidth / 2;
    drawTickBox(g, button, tickX, 4.0f,
                tickWidth, tickWidth,
                button.getToggleState(),
                button.isEnabled(),
                shouldDrawButtonAsHighlighted,
                shouldDrawButtonAsDown);

    g.setColour(button.findColour(ToggleButton::textColourId));
    g.setFont(fontSize);

    if (!button.isEnabled())
        g.setOpacity(0.5f);

    g.drawText(button.getButtonText(),
               button.getLocalBounds().getBottomLeft().getX(), button.getLocalBounds().getBottomLeft().getY() - button.getHeight() / 2 + 2, button.getWidth(), 15,
               Justification::centred);
}