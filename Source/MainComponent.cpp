#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(400, 200);
    setAudioChannels(2, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int /*samplesPerBlockExpected*/, double /*sampleRate*/)
{
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* device = deviceManager.getCurrentAudioDevice();

    if (device == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    auto activeInputChannels  = device->getActiveInputChannels();
    auto activeOutputChannels = device->getActiveOutputChannels();
    auto maxInputChannels  = activeInputChannels.countNumberOfSetBits();
    auto maxOutputChannels = activeOutputChannels.countNumberOfSetBits();

    for (int ch = 0; ch < maxOutputChannels; ++ch)
    {
        if (!activeOutputChannels[ch] || maxInputChannels == 0)
        {
            bufferToFill.buffer->clear(ch, bufferToFill.startSample, bufferToFill.numSamples);
        }
        else
        {
            int srcCh = ch % maxInputChannels;
            bufferToFill.buffer->copyFrom(ch, bufferToFill.startSample,
                                          *bufferToFill.buffer, srcCh,
                                          bufferToFill.startSample,
                                          bufferToFill.numSamples);
        }
    }
}

void MainComponent::releaseResources()
{
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    g.drawText("Audio Routing Device", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
}
