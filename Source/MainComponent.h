#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "LevelMeter.h"

class MainComponent : public juce::AudioAppComponent,
                      public juce::ChangeListener
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster*) override;

private:
    void saveSettings();
    void refreshDeviceControls();

    std::atomic<float> gain  { 1.0f };
    std::atomic<float> peakL { 0.0f };
    std::atomic<float> peakR { 0.0f };

    juce::Slider volumeSlider;

    juce::Label    outputLabel      { {}, "Output" };
    juce::Label    sampleRateLabel  { {}, "Sample Rate" };
    juce::Label    bufferSizeLabel  { {}, "Buffer Size" };
    juce::ComboBox outputDeviceBox;
    juce::ComboBox sampleRateBox;
    juce::ComboBox bufferSizeBox;

    LevelMeter levelMeter { peakL, peakR };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
