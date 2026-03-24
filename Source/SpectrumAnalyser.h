#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>

class SpectrumAnalyser : public juce::Component,
                         private juce::Timer
{
public:
    static constexpr int fftOrder = 10;
    static constexpr int fftSize  = 1 << fftOrder; // 1024

    SpectrumAnalyser() { startTimerHz(30); }
    ~SpectrumAnalyser() override { stopTimer(); }

    void setSampleRate(double sr) { sampleRate = sr; }

    // Audio thread — no allocations, no locks
    void pushSamples(const float* data, int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            fifo[fifoWritePos++] = data[i];
            if (fifoWritePos >= fftSize)
            {
                fifoWritePos = 0;
                fftReady.store(true, std::memory_order_release);
            }
        }
    }

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        float w = bounds.getWidth();
        float h = bounds.getHeight();

        g.fillAll(juce::Colour(0xff0d0d0f));
        g.setColour(juce::Colour(0xff1e1e28));
        g.fillRect(0, 0, (int)w, 1); // separator

        if (w < 2.0f) return;

        const int    numPoints = (int)w;
        const double nyquist   = sampleRate * 0.5;
        const double logMin    = std::log10(30.0);
        const double logMax    = std::log10(juce::jmin(nyquist, 20000.0));

        juce::Path fillPath, strokePath;

        for (int px = 0; px < numPoints; ++px)
        {
            double t   = (double)px / (double)(numPoints - 1);
            double freq = std::pow(10.0, logMin + t * (logMax - logMin));
            int    bin  = juce::jlimit(0, fftSize / 2 - 1,
                              juce::roundToInt((float)(freq / nyquist * (fftSize * 0.5))));

            float y = h - displayBins[bin] * h;

            if (px == 0)
            {
                fillPath.startNewSubPath(0.0f, h);
                fillPath.lineTo(0.0f, y);
                strokePath.startNewSubPath(0.0f, y);
            }
            else
            {
                fillPath.lineTo((float)px, y);
                strokePath.lineTo((float)px, y);
            }
        }

        fillPath.lineTo((float)(numPoints - 1), h);
        fillPath.closeSubPath();

        g.setColour(juce::Colour(0xff39ff6e).withAlpha(0.10f));
        g.fillPath(fillPath);
        g.setColour(juce::Colour(0xff39ff6e).withAlpha(0.85f));
        g.strokePath(strokePath, juce::PathStrokeType(1.0f));
    }

private:
    float fifo[fftSize]           = {};
    float fftData[fftSize * 2]    = {};
    float displayBins[fftSize / 2] = {};
    int   fifoWritePos = 0;

    std::atomic<bool> fftReady { false };

    juce::dsp::FFT fft { fftOrder };
    juce::dsp::WindowingFunction<float> window {
        (size_t)fftSize, juce::dsp::WindowingFunction<float>::hann };

    double sampleRate = 44100.0;

    void timerCallback() override
    {
        if (!fftReady.load(std::memory_order_acquire))
            return;

        std::copy(fifo, fifo + fftSize, fftData);
        std::fill(fftData + fftSize, fftData + fftSize * 2, 0.0f);
        fftReady.store(false, std::memory_order_release);

        window.multiplyWithWindowingTable(fftData, fftSize);
        fft.performFrequencyOnlyForwardTransform(fftData);

        const float scale = 1.0f / (float)fftSize;
        for (int i = 0; i < fftSize / 2; ++i)
        {
            float db    = juce::Decibels::gainToDecibels(fftData[i] * scale, -100.0f);
            float level = juce::jlimit(0.0f, 1.0f, juce::jmap(db, -80.0f, -10.0f, 0.0f, 1.0f));
            displayBins[i] = displayBins[i] * 0.75f + level * 0.25f;
        }

        repaint();
    }
};
