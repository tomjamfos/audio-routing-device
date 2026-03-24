#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <atomic>

class LevelMeter : public juce::Component,
                   private juce::Timer
{
public:
    LevelMeter(std::atomic<float>& inPeakL, std::atomic<float>& inPeakR)
        : peakL(inPeakL), peakR(inPeakR)
    {
        startTimerHz(30);
    }

    ~LevelMeter() override { stopTimer(); }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().reduced(2);
        int barWidth = (area.getWidth() - kGap) / 2;

        auto leftArea  = area.withWidth(barWidth);
        auto rightArea = area.withX(area.getX() + barWidth + kGap).withWidth(barWidth);

        g.fillAll(juce::Colour(0xff1a1a1a));

        drawBar(g, leftArea,  displayL);
        drawBar(g, rightArea, displayR);
    }

private:
    static constexpr int kGap = 3;

    static constexpr float kDbMin    = -60.0f;
    static constexpr float kDbOrange = -12.0f;
    static constexpr float kDbRed    =  -3.0f;
    static constexpr float kDbMax    =  12.0f;

    // Fast attack, slow decay
    static constexpr float kAttack = 0.8f;
    static constexpr float kDecay  = 0.05f;

    std::atomic<float>& peakL;
    std::atomic<float>& peakR;

    float displayL = kDbMin;
    float displayR = kDbMin;

    void timerCallback() override
    {
        auto toDb = [](float gain) {
            return juce::Decibels::gainToDecibels(gain, kDbMin);
        };

        float newL = toDb(peakL.load());
        float newR = toDb(peakR.load());

        displayL = smooth(displayL, newL);
        displayR = smooth(displayR, newR);

        repaint();
    }

    static float smooth(float current, float target)
    {
        float factor = (target > current) ? kAttack : kDecay;
        return current + (target - current) * factor;
    }

    static void drawBar(juce::Graphics& g, juce::Rectangle<int> area, float levelDb)
    {
        float clampedDb  = juce::jlimit(kDbMin, kDbMax, levelDb);
        float normalised = (clampedDb - kDbMin) / (kDbMax - kDbMin);

        int totalH  = area.getHeight();
        int filledH = juce::roundToInt(normalised * totalH);
        int filledY = area.getBottom() - filledH;

        // Y positions for colour thresholds
        auto thresholdY = [&](float db) {
            float t = (db - kDbMin) / (kDbMax - kDbMin);
            return area.getBottom() - juce::roundToInt(t * totalH);
        };

        int yOrange = thresholdY(kDbOrange);
        int yRed    = thresholdY(kDbRed);

        // Unfilled (dark background)
        g.setColour(juce::Colour(0xff2a2a2a));
        g.fillRect(area.getX(), area.getY(), area.getWidth(), filledY - area.getY());

        // Helper: draw one colour segment of the filled region
        auto drawSegment = [&](int segTop, int segBottom, juce::Colour colour)
        {
            int top    = juce::jmax(segTop,    filledY);
            int bottom = juce::jmin(segBottom, area.getBottom());
            if (top < bottom)
            {
                g.setColour(colour);
                g.fillRect(area.getX(), top, area.getWidth(), bottom - top);
            }
        };

        drawSegment(area.getY(), yRed,    juce::Colour(0xffff3333)); // red
        drawSegment(yRed,        yOrange, juce::Colour(0xffff9900)); // orange
        drawSegment(yOrange,     area.getBottom(), juce::Colour(0xff33cc44)); // green

        // Tick marks at -12 dB and 0 dB
        g.setColour(juce::Colour(0xff555555));
        g.fillRect(area.getX(), yOrange - 1,            area.getWidth(), 1);
        g.fillRect(area.getX(), thresholdY(0.0f) - 1,   area.getWidth(), 1);
    }
};
