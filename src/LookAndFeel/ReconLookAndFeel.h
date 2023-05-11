#pragma once
#include <JuceHeader.h>

class ReconLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ReconLookAndFeel();
    ~ReconLookAndFeel() override;

    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &) override;

    void drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox &box) override;
    Font getComboBoxFont(ComboBox &box) override;

    void positionComboBoxText(ComboBox &box, Label &label) override;
    void drawPopupMenuBackground(Graphics &,
                                 int width,
                                 int height) override;
    void drawPopupMenuItem(Graphics &, const Rectangle<int> &area,
                           bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                           const String &text, const String &shortcutKeyText,
                           const Drawable *icon, const Colour *textColour) override;

    void drawButtonBackground(Graphics &, Button &, const Colour &backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawToggleButton(Graphics &, ToggleButton &,
                          bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};