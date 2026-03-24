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
    volumeSlider.setRange(-60.0, 12.0);
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

    sampleRateBox.onChange = [this] {
        auto setup = deviceManager.getAudioDeviceSetup();
        setup.sampleRate = sampleRateBox.getText().getDoubleValue();
        deviceManager.setAudioDeviceSetup(setup, true);
    };

    bufferSizeBox.onChange = [this] {
        auto setup = deviceManager.getAudioDeviceSetup();
        setup.bufferSize = bufferSizeBox.getText().getIntValue();
        deviceManager.setAudioDeviceSetup(setup, true);
    };

    for (auto* label : { &outputLabel, &sampleRateLabel, &bufferSizeLabel })
        addAndMakeVisible(label);
    for (auto* box : { &outputDeviceBox, &sampleRateBox, &bufferSizeBox })
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
    auto* device     = deviceManager.getCurrentAudioDevice();

    // Output devices
    outputDeviceBox.clear(juce::dontSendNotification);
    if (deviceType != nullptr)
    {
        auto names = deviceType->getDeviceNames(false); // false = outputs
        for (int i = 0; i < names.size(); ++i)
            outputDeviceBox.addItem(names[i], i + 1);
        outputDeviceBox.setText(setup.outputDeviceName, juce::dontSendNotification);
    }

    // Sample rates
    sampleRateBox.clear(juce::dontSendNotification);
    bufferSizeBox.clear(juce::dontSendNotification);
    if (device != nullptr)
    {
        auto rates = device->getAvailableSampleRates();
        for (int i = 0; i < rates.size(); ++i)
            sampleRateBox.addItem(juce::String(juce::roundToInt(rates[i])) + " Hz", i + 1);
        sampleRateBox.setText(juce::String(juce::roundToInt(setup.sampleRate)) + " Hz",
                              juce::dontSendNotification);

        auto buffers = device->getAvailableBufferSizes();
        for (int i = 0; i < buffers.size(); ++i)
            bufferSizeBox.addItem(juce::String(buffers[i]), i + 1);
        bufferSizeBox.setText(juce::String(setup.bufferSize), juce::dontSendNotification);
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
    const int labelW  = 90;
    const int spacing = 6;

    volumeSlider.setBounds(area.removeFromTop(rowH));
    area.removeFromTop(spacing);

    auto layoutRow = [&](juce::Label& label, juce::ComboBox& box)
    {
        auto row = area.removeFromTop(rowH);
        label.setBounds(row.removeFromLeft(labelW));
        box.setBounds(row);
        area.removeFromTop(spacing);
    };

    layoutRow(outputLabel,     outputDeviceBox);
    layoutRow(sampleRateLabel, sampleRateBox);
    layoutRow(bufferSizeLabel, bufferSizeBox);

    levelMeter.setBounds(meterArea);
}
