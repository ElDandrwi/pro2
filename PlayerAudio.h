#pragma once

#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
{
public:
    PlayerAudio();
    ~PlayerAudio() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadFile(const juce::File& file);
    void setGain(float gain);
    void play();
    void pause();
    void setPosition(double seconds);
    double getLengthInSeconds() const;
    void goToStart();
    void goToEnd();

    void setLooping(bool shouldLoop);
    bool isLooping() const { return isLoopEnabled; }

    double getCurrentPosition() const;
    double getPositionRatio() const;

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    bool isLoopEnabled = false;
    double totalLengthInSeconds = 0.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
