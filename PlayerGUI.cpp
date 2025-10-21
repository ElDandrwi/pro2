#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    addAndMakeVisible(loadButton);
    addAndMakeVisible(muteButton);
 addAndMakeVisible(playButton);
 addAndMakeVisible(pauseButton);
 addAndMakeVisible(gotoStartButton);
 addAndMakeVisible(gotoEndButton);
    addAndMakeVisible(volumeSlider);

    loadButton.addListener(this);
    muteButton.addListener(this);
 playButton.addListener(this);
 pauseButton.addListener(this);
 gotoStartButton.addListener(this);
 gotoEndButton.addListener(this);

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
     loadButton.setBounds(80, y, 100, 40);
 muteButton.setBounds(200 ,y, 100, 40);
 gotoStartButton.setBounds(20, 60, 80, 30);
 playButton.setBounds(110, 60, 80 ,  30  );
 pauseButton.setBounds(200, 60, 80,  30  );
 gotoEndButton.setBounds(290, 60, 80, 30 );

    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        if (onLoadFileRequest)
            onLoadFileRequest();
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
	else if (button == &playButton)
{
	if (audioPlayer)
		audioPlayer->play();
}
else if (button == &pauseButton)
{
	if (audioPlayer)
		audioPlayer->pause();   
}
else if (button == &gotoStartButton)
{
	if (audioPlayer)
		audioPlayer->goToStart();
}
else if (button == &gotoEndButton)
{
    if (audioPlayer)
		audioPlayer->goToEnd();

}
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider && audioPlayer)
        audioPlayer->setGain((float)slider->getValue());
}




