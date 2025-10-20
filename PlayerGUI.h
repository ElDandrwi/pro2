#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
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
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
	juce::TextButton muteButton{ "Mute" };
    juce::Slider volumeSlider;

    PlayerAudio* audioPlayer = nullptr;

    float lastVolumeBeforeMute = 0.5f;

	bool isMuted = false;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};

