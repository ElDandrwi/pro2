#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
	formatManager.registerBasicFormats();
    addAndMakeVisible(loadButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(gotoStartButton);
    addAndMakeVisible(gotoEndButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(setAButton);
    addAndMakeVisible(setBButton);
    addAndMakeVisible(clearABButton);
    addAndMakeVisible(abLoopButton);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(timeLabel);
    addAndMakeVisible(abLoopLabel);
	addAndMakeVisible(waveformDisplay);

    loadButton.addListener(this);
    muteButton.addListener(this);
    playButton.addListener(this);
    pauseButton.addListener(this);
    gotoStartButton.addListener(this);
    gotoEndButton.addListener(this);
    loopButton.addListener(this);
    setAButton.addListener(this);
    setBButton.addListener(this);
    clearABButton.addListener(this);
    abLoopButton.addListener(this);

	speedSlider.setRange(0.5, 2.0, 0.1);
	speedSlider.setValue(1.0);
	speedSlider.addListener(this);
	speedSlider.setTextValueSuffix("X");

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);

    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.setValue(0.0);
    positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    positionSlider.addListener(this);

    timeLabel.setText("0:00 / 0:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centred);

    abLoopLabel.setText("A-B: --:-- / --:--", juce::dontSendNotification);
    abLoopLabel.setJustificationType(juce::Justification::centred);
	
 	addAndMakeVisible(fileInfoLabel);
 	fileInfoLabel.setJustificationType(juce::Justification::centredLeft);
 	fileInfoLabel.setFont(juce::Font(13.0f));
 	fileInfoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
  
	addAndMakeVisible(SaveLabel);
	SaveLabel.setText(" --SAVE-- \n", juce::dontSendNotification);

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
    loopButton.setBounds(260, y, 100, 40);

    int sliderY = 70;
    positionSlider.setBounds(20, sliderY, 460, 20);
    timeLabel.setBounds(20, sliderY + 25, 460, 20);
    abLoopLabel.setBounds(20, sliderY + 45, 460, 20);
    speedSlider.setBounds(20, sliderY + 180, 460, 20);

    int controlY = 120;
    gotoStartButton.setBounds(20, controlY, 80, 30);
    playButton.setBounds(110, controlY, 80, 30);
    pauseButton.setBounds(200, controlY, 80, 30);
    gotoEndButton.setBounds(290, controlY, 80, 30);

    int abY = 160;
    setAButton.setBounds(20, abY, 80, 30);
    setBButton.setBounds(110, abY, 80, 30);
    clearABButton.setBounds(200, abY, 80, 30);
    abLoopButton.setBounds(290, abY, 80, 30);

    volumeSlider.setBounds(20, 200, 460, 30);
	
    waveformDisplay.setBounds(20, 300, 460, 80);
    fileInfoLabel.setBounds(20, 400, 460, 60);

	SaveLabel.setBounds(520, 20, 260, getHeight()-30);

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
                volumeSlider.setValue(BeforeMute);
                isMuted = false;
                muteButton.setButtonText("Mute");
            }
            else {
                BeforeMute = (float)volumeSlider.getValue();
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

            
            if (newLoopState && audioPlayer->getABLoopEnabled())
            {
                audioPlayer->setABLoopEnabled(false);
                abLoopButton.setButtonText("A-B Loop: Off");
                updateABLoopDisplay();
            }

            if (newLoopState)
                loopButton.setButtonText("Loop: On");
            else
                loopButton.setButtonText("Loop: Off");
        }
    }
    else if (button == &setAButton)
    {
        if (audioPlayer)
        {
            double currentPos = audioPlayer->getCurrentPosition();
            audioPlayer->setABLoopPointA(currentPos);
            updateABLoopDisplay();
        }
    }
    else if (button == &setBButton)
    {
        if (audioPlayer)
        {
            double currentPos = audioPlayer->getCurrentPosition();
            audioPlayer->setABLoopPointB(currentPos);
            updateABLoopDisplay();
        }
    }
    else if (button == &clearABButton)
    {
        if (audioPlayer)
        {
            audioPlayer->clearABLoopPoints();
            abLoopButton.setButtonText("A-B Loop: Off");
            updateABLoopDisplay();
        }
    }
    else if (button == &abLoopButton)
    {
        if (audioPlayer && audioPlayer->hasABLoopPoints())
        {
            bool newABLoopState = !audioPlayer->getABLoopEnabled();
            audioPlayer->setABLoopEnabled(newABLoopState);

            
            if (newABLoopState && audioPlayer->isLooping())
            {
                audioPlayer->setLooping(false);
                loopButton.setButtonText("Loop: Off");
            }

            if (newABLoopState)
                abLoopButton.setButtonText("A-B Loop: On");
            else
                abLoopButton.setButtonText("A-B Loop: Off");
        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider && audioPlayer)
    {
        audioPlayer->setGain((double)slider->getValue());
    }
    else if (slider == &positionSlider && audioPlayer)
    {
        double positionRatio = slider->getValue();
        double totalLength = audioPlayer->getLengthInSeconds();
        double newPosition = positionRatio * totalLength;
        audioPlayer->setPosition(newPosition);
        updateTimeDisplay();
    }
    else if (slider == &speedSlider && audioPlayer)
    {
		audioPlayer->setSpeed((double)slider->getValue());
    }
}

void PlayerGUI::timerCallback()
{
    if (audioPlayer != nullptr)
    {
        positionSlider.setValue(audioPlayer->getPositionRatio(), juce::dontSendNotification);
        updateTimeDisplay();
        updateABLoopDisplay();

		waveformDisplay.setPositionRelative(audioPlayer->getPositionRatio());

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

void PlayerGUI::updateABLoopDisplay()
{
    if (audioPlayer != nullptr)
    {
        auto formatTime = [](double seconds) -> juce::String
            {
                if (seconds < 0) return "--:--";
                int totalSeconds = static_cast<int>(seconds);
                int mins = totalSeconds / 60;
                int secs = totalSeconds % 60;
                return juce::String(mins) + ":" + (secs < 10 ? "0" : "") + juce::String(secs);
            };

        double pointA = audioPlayer->getABLoopPointA();
        double pointB = audioPlayer->getABLoopPointB();

        juce::String abText = "From " + formatTime(pointA) + " To " + formatTime(pointB);
        abLoopLabel.setText(abText, juce::dontSendNotification);
    }
}

void PlayerGUI::loadFileForWaveform(const juce::File& file)
{
    if (!file.existsAsFile())
        return;

    thumbnail.clear();
    thumbnail.setSource(new juce::FileInputSource(file));
    waveformDisplay.repaint();
}



