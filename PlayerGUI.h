#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setAudioPlayer(PlayerAudio* player) { audioPlayer = player; }

    std::function<void()> onLoadFileRequest;

private:
    juce::TextButton loadButton{ "Load Files" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton gotoStartButton{ "Start" };
    juce::TextButton gotoEndButton{ "End" };
    juce::TextButton loopButton{ "Loop: Off" };
    juce::TextButton setAButton{ "A" };
    juce::TextButton setBButton{ "B" };
    juce::TextButton clearABButton{ "Clear" };
    juce::TextButton abLoopButton{ "A-B Loop: Off" };
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Label timeLabel;
    juce::Label abLoopLabel;

    PlayerAudio* audioPlayer = nullptr;

    float lastVolumeBeforeMute = 0.5f;
    bool isMuted = false;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
    void updateTimeDisplay();
    void updateABLoopDisplay();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
