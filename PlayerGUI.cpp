#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    addAndMakeVisible(loadButton);
    addAndMakeVisible(restartButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(muteButton);

    addAndMakeVisible(volumeSlider);

    loadButton.addListener(this);
    restartButton.addListener(this);
    stopButton.addListener(this);
    muteButton.addListener(this);


    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
}

PlayerGUI::~PlayerGUI()
{
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
    muteButton.setBounds(40, 150, 80, 40);

    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        if (onLoadFileRequest)
            onLoadFileRequest();
    }
    else if (button == &restartButton)
    {
        if (audioPlayer)
            audioPlayer->stop();
            audioPlayer->start();
    }
    else if (button == &stopButton)
    {
        if (audioPlayer)
            audioPlayer->stop();
    }
    else if (button == &muteButton)
    {
        if (audioPlayer) {
			if (isMuted) {
				volumeSlider.setValue(lastVolumeBeforeMute);
				isMuted = false;
                muteButton.setButtonText("Mute");
            }
			else {
				lastVolumeBeforeMute = (float)volumeSlider.getValue();
				volumeSlider.setValue(0.0);
				isMuted = true;
                muteButton.setButtonText("Unmute");
            }
        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider && audioPlayer)
        audioPlayer->setGain((float)slider->getValue());
}

