#include "MainComponent.h"

MainComponent::MainComponent()
    : deviceSelector(deviceManager, 1, 2, 1, 2, false, false, true, false)
{
    setAudioChannels(2, 2);

    volumeSlider.setRange(-60.0, 12.0);
    volumeSlider.setValue(0.0);
    volumeSlider.setTextValueSuffix(" dB");
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 70, 20);
    volumeSlider.onValueChange = [this] {
        gain = juce::Decibels::decibelsToGain((float) volumeSlider.getValue());
    };
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(deviceSelector);

    setSize(400, 500);
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
    auto* inputDevice  = deviceManager.getCurrentAudioDevice();

    if (inputDevice == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    auto activeInputChannels  = inputDevice->getActiveInputChannels();
    auto activeOutputChannels = inputDevice->getActiveOutputChannels();
    int numInputChannels  = activeInputChannels.countNumberOfSetBits();
    int numOutputChannels = activeOutputChannels.countNumberOfSetBits();

    const float g = gain.load();

    for (int ch = 0; ch < numOutputChannels; ++ch)
    {
        if (numInputChannels == 0)
        {
            bufferToFill.buffer->clear(ch, bufferToFill.startSample, bufferToFill.numSamples);
        }
        else
        {
            int srcCh = ch % numInputChannels;
            auto* inData  = bufferToFill.buffer->getReadPointer(srcCh, bufferToFill.startSample);
            auto* outData = bufferToFill.buffer->getWritePointer(ch,    bufferToFill.startSample);

            for (int s = 0; s < bufferToFill.numSamples; ++s)
                outData[s] = inData[s] * g;
        }
    }
}

void MainComponent::releaseResources()
{
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(10);
    volumeSlider.setBounds(area.removeFromTop(40));
    deviceSelector.setBounds(area);
}
