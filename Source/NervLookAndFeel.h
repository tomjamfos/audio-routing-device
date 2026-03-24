#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

class NervLookAndFeel : public juce::LookAndFeel_V4
{
public:
    NervLookAndFeel()
    {
        setColour(juce::ComboBox::backgroundColourId,          juce::Colour(0xff1a1a1f));
        setColour(juce::ComboBox::textColourId,                juce::Colour(0xffe8e8f0));
        setColour(juce::ComboBox::outlineColourId,             juce::Colour(0xff2a2a35));
        setColour(juce::ComboBox::arrowColourId,               juce::Colour(0xff555566));
        setColour(juce::ComboBox::focusedOutlineColourId,      juce::Colour(0xff39ff6e));

        setColour(juce::PopupMenu::backgroundColourId,              juce::Colour(0xff1a1a1f));
        setColour(juce::PopupMenu::textColourId,                    juce::Colour(0xffe8e8f0));
        setColour(juce::PopupMenu::highlightedBackgroundColourId,   juce::Colour(0xff1e1e28));
        setColour(juce::PopupMenu::highlightedTextColourId,         juce::Colour(0xff39ff6e));

        setColour(juce::Slider::textBoxTextColourId,       juce::Colour(0xff00e5ff));
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff0d0d0f));
        setColour(juce::Slider::textBoxOutlineColourId,    juce::Colour(0xff2a2a35));
    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float, float,
                          juce::Slider::SliderStyle, juce::Slider&) override
    {
        const int trackH = 2;
        int trackY  = y + (height - trackH) / 2;
        int thumbX  = juce::roundToInt(sliderPos);

        // Full track
        g.setColour(juce::Colour(0xff2a2a35));
        g.fillRect(x, trackY, width, trackH);

        // Filled portion with glow
        if (thumbX > x)
        {
            g.setColour(juce::Colour(0x1039ff6e));
            g.fillRect(x, trackY - 3, thumbX - x, trackH + 6);
            g.setColour(juce::Colour(0x2539ff6e));
            g.fillRect(x, trackY - 1, thumbX - x, trackH + 2);
            g.setColour(juce::Colour(0xff39ff6e));
            g.fillRect(x, trackY, thumbX - x, trackH);
        }

        // Thumb — vertical bar
        const int thumbW = 2;
        const int thumbH = 14;
        int thumbY = y + (height - thumbH) / 2;

        g.setColour(juce::Colour(0x2039ff6e));
        g.fillRect(thumbX - 4, thumbY - 2, thumbW + 8, thumbH + 4);
        g.setColour(juce::Colour(0x5039ff6e));
        g.fillRect(thumbX - 2, thumbY - 1, thumbW + 4, thumbH + 2);
        g.setColour(juce::Colour(0xff39ff6e));
        g.fillRect(thumbX, thumbY, thumbW, thumbH);
    }

    void drawComboBox(juce::Graphics& g, int width, int height, bool,
                      int, int, int, int, juce::ComboBox&) override
    {
        g.setColour(juce::Colour(0xff1a1a1f));
        g.fillRect(0, 0, width, height);
        g.setColour(juce::Colour(0xff2a2a35));
        g.drawRect(0, 0, width, height, 1);

        // Chevron arrow
        const float aw = 6.0f;
        float ax = width - 14.0f;
        float ay = (height - aw * 0.6f) / 2.0f;
        juce::Path arrow;
        arrow.addTriangle(ax, ay, ax + aw, ay, ax + aw * 0.5f, ay + aw * 0.6f);
        g.setColour(juce::Colour(0xff555566));
        g.fillPath(arrow);
    }

    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override
    {
        g.setColour(juce::Colour(0xff1a1a1f));
        g.fillRect(0, 0, width, height);
        g.setColour(juce::Colour(0xff2a2a35));
        g.drawRect(0, 0, width, height, 1);
    }

    void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                           bool isSeparator, bool, bool isHighlighted,
                           bool, bool, const juce::String& text, const juce::String&,
                           const juce::Drawable*, const juce::Colour*) override
    {
        if (isSeparator)
        {
            g.setColour(juce::Colour(0xff2a2a35));
            g.fillRect(area.getX() + 6, area.getCentreY(), area.getWidth() - 12, 1);
            return;
        }

        g.setColour(isHighlighted ? juce::Colour(0xff1e1e28) : juce::Colour(0xff1a1a1f));
        g.fillRect(area);

        g.setColour(isHighlighted ? juce::Colour(0xff39ff6e) : juce::Colour(0xffe8e8f0));
        g.setFont(juce::Font(juce::FontOptions(11.0f)));
        g.drawText(text, area.reduced(8, 0), juce::Justification::centredLeft);
    }
};
