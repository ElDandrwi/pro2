#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "PlayerGUI.h"
#include "PlaylistGUI.h"

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    
    PlayerAudio audioPlayer1;
    PlayerAudio audioPlayer2;
    PlayerGUI   guiPlayer1;
    PlayerGUI   guiPlayer2;

   
    PlayerAudio audioPlayer;
    PlaylistGUI playlistGUI;

   
    juce::Slider masterVolumeSlider;
    juce::Label  masterVolumeLabel;
    juce::Slider balanceSlider;
    juce::Label  balanceLabel;

    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
