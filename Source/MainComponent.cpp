#include "MainComponent.h"

static juce::File getSettingsFile()
{
    return juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
               .getChildFile("Audio Routing Device")
               .getChildFile("settings.xml");
}

MainComponent::MainComponent()
{
    setAudioChannels(2, 2);

    // Restore saved settings
    auto settingsFile = getSettingsFile();
    if (settingsFile.existsAsFile())
    {
        if (auto xml = juce::XmlDocument::parse(settingsFile))
        {
            deviceManager.initialise(2, 2, xml.get(), true);

            double savedVolume = xml->getDoubleAttribute("volumeDb", 0.0);
            volumeSlider.setValue(savedVolume, juce::dontSendNotification);
            gain = juce::Decibels::decibelsToGain((float) savedVolume);
        }
    }

    deviceManager.addChangeListener(this);

    // Volume slider
    volumeSlider.setRange(-60.0, 0.0, 0.1);
    volumeSlider.setValue(0.0);
    volumeSlider.setTextValueSuffix(" dB");
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 70, 20);
    volumeSlider.onValueChange = [this] {
        gain = juce::Decibels::decibelsToGain((float) volumeSlider.getValue());
        saveSettings();
    };

    // Device controls
    outputDeviceBox.onChange = [this] {
        auto setup = deviceManager.getAudioDeviceSetup();
        setup.outputDeviceName = outputDeviceBox.getText();
        setup.useDefaultOutputChannels = false;
        deviceManager.setAudioDeviceSetup(setup, true);
    };

    for (auto* box : { &outputDeviceBox })
        addAndMakeVisible(box);

    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(levelMeter);

    refreshDeviceControls();

    setSize(460, 220);
}

MainComponent::~MainComponent()
{
    deviceManager.removeChangeListener(this);
    shutdownAudio();
}

void MainComponent::refreshDeviceControls()
{
    auto* deviceType = deviceManager.getCurrentDeviceTypeObject();
    auto  setup      = deviceManager.getAudioDeviceSetup();

    // Output devices
    outputDeviceBox.clear(juce::dontSendNotification);
    if (deviceType != nullptr)
    {
        auto truncate = [](const juce::String& s) {
            return s.length() > 14 ? s.substring(0, 12) + "..." : s;
        };

        auto names = deviceType->getDeviceNames(false); // false = outputs
        for (int i = 0; i < names.size(); ++i)
            outputDeviceBox.addItem(truncate(names[i]), i + 1);
        outputDeviceBox.setText(truncate(setup.outputDeviceName), juce::dontSendNotification);
    }

}

void MainComponent::saveSettings()
{
    if (auto xml = deviceManager.createStateXml())
    {
        xml->setAttribute("volumeDb", volumeSlider.getValue());
        auto settingsFile = getSettingsFile();
        settingsFile.getParentDirectory().createDirectory();
        xml->writeTo(settingsFile);
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster*)
{
    refreshDeviceControls();
    saveSettings();
}

void MainComponent::prepareToPlay(int /*samplesPerBlockExpected*/, double /*sampleRate*/)
{
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* inputDevice = deviceManager.getCurrentAudioDevice();

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

    // Update peak levels for the VU meter (audio thread — atomics only)
    float maxL = 0.0f, maxR = 0.0f;
    if (bufferToFill.buffer->getNumChannels() > 0)
        maxL = bufferToFill.buffer->getMagnitude(0, bufferToFill.startSample, bufferToFill.numSamples);
    if (bufferToFill.buffer->getNumChannels() > 1)
        maxR = bufferToFill.buffer->getMagnitude(1, bufferToFill.startSample, bufferToFill.numSamples);

    peakL.store(maxL);
    peakR.store(maxR);
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
    auto area      = getLocalBounds().reduced(10);
    auto meterArea = area.removeFromRight(50);

    const int rowH    = 30;
    const int spacing = 6;

    volumeSlider.setBounds(area.removeFromTop(rowH));
    area.removeFromTop(spacing);

    outputDeviceBox.setBounds(area.removeFromTop(rowH).withWidth(120));

    levelMeter.setBounds(meterArea);
}
