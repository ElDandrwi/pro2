#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    addAndMakeVisible(loadButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(gotoStartButton);
    addAndMakeVisible(gotoEndButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(timeLabel);

    loadButton.addListener(this);
    muteButton.addListener(this);
    playButton.addListener(this);
    pauseButton.addListener(this);
    gotoStartButton.addListener(this);
    gotoEndButton.addListener(this);
    loopButton.addListener(this);

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);

    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.setValue(0.0);
    positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    positionSlider.addListener(this);

    timeLabel.setText("0:00 / 0:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centred);

    startTimer(50);
}

PlayerGUI::~PlayerGUI()
{
    stopTimer();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    muteButton.setBounds(140, y, 100, 40);
    loopButton.setBounds(250, y, 100, 40);

    int controlY = 70;
    gotoStartButton.setBounds(20, controlY, 80, 30);
    playButton.setBounds(110, controlY, 80, 30);
    pauseButton.setBounds(200, controlY, 80, 30);
    gotoEndButton.setBounds(290, controlY, 80, 30);

    int sliderY = 110;
    positionSlider.setBounds(20, sliderY, getWidth() - 40, 20);
    timeLabel.setBounds(20, sliderY + 25, getWidth() - 40, 20);

    volumeSlider.setBounds(20, 150, getWidth() - 40, 30);
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
    else if (button == &loopButton)
    {
        if (audioPlayer)
        {
            bool newLoopState = !audioPlayer->isLooping();
            audioPlayer->setLooping(newLoopState);

            if (newLoopState)
                loopButton.setButtonText("Loop: On");
            else
                loopButton.setButtonText("Loop: Off");
        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider && audioPlayer)
    {
        audioPlayer->setGain((float)slider->getValue());
    }
    else if (slider == &positionSlider && audioPlayer)
    {
        double positionRatio = slider->getValue();
        double totalLength = audioPlayer->getLengthInSeconds();
        double newPosition = positionRatio * totalLength;
        audioPlayer->setPosition(newPosition);
        updateTimeDisplay();
    }
}

void PlayerGUI::timerCallback()
{
    if (audioPlayer != nullptr)
    {
        positionSlider.setValue(audioPlayer->getPositionRatio(), juce::dontSendNotification);
        updateTimeDisplay();
    }
}

void PlayerGUI::updateTimeDisplay()
{
    if (audioPlayer != nullptr)
    {
        double currentTime = audioPlayer->getCurrentPosition();
        double totalTime = audioPlayer->getLengthInSeconds();

        auto formatTime = [](double seconds) -> juce::String
            {
                int totalSeconds = static_cast<int>(seconds);
                int mins = totalSeconds / 60;
                int secs = totalSeconds % 60;
                return juce::String(mins) + ":" + (secs < 10 ? "0" : "") + juce::String(secs);
            };

        juce::String timeText = formatTime(currentTime) + " / " + formatTime(totalTime);
        timeLabel.setText(timeText, juce::dontSendNotification);
    }
}
