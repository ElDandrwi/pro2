
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    formatManager.registerBasicFormats();


    muteButton.setImages(false, true, true,
        unmuteImage, 1.0f, juce::Colours::transparentBlack,
        unmuteImage, 1.0f, juce::Colours::lightgrey,
        unmuteImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(muteButton);
    muteButton.addListener(this);

    loadButton.setImages(false, true, true,
        loadImage, 1.0f, juce::Colours::transparentBlack,
        loadImage, 1.0f, juce::Colours::lightgrey,
        loadImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);

    playButton.setImages(false, true, true,
        playImage, 1.0f, juce::Colours::transparentBlack,
        playImage, 1.0f, juce::Colours::lightgrey,
        playImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(playButton);
    playButton.addListener(this);

    pauseButton.setImages(false, true, true,
        pauseImage, 1.0f, juce::Colours::transparentBlack,
        pauseImage, 1.0f, juce::Colours::lightgrey,
        pauseImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(pauseButton);
    pauseButton.addListener(this);

    gotoStartButton.setImages(false, true, true,
        StartImage, 1.0f, juce::Colours::transparentBlack,
        StartImage, 1.0f, juce::Colours::lightgrey,
        StartImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(gotoStartButton);
    gotoStartButton.addListener(this);

    gotoEndButton.setImages(false, true, true,
        EndImage, 1.0f, juce::Colours::transparentBlack,
        EndImage, 1.0f, juce::Colours::lightgrey,
        EndImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(gotoEndButton);
    gotoEndButton.addListener(this);

    loopButton.setImages(false, true, true,
        loopImage, 1.0f, juce::Colours::transparentBlack,
        loopImage, 1.0f, juce::Colours::lightgrey,
        loopImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(loopButton);
    loopButton.addListener(this);

    clearABButton.setImages(false, true, true,
        clearImage, 1.0f, juce::Colours::transparentBlack,
        clearImage, 1.0f, juce::Colours::lightgrey,
        clearImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(clearABButton);
    clearABButton.addListener(this);

    abLoopButton.setImages(false, true, true,
        loopABImage, 1.0f, juce::Colours::transparentBlack,
        loopABImage, 1.0f, juce::Colours::lightgrey,
        loopABImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(abLoopButton);
    abLoopButton.addListener(this);

    jumpBackButton.setImages(false, true, true,
        jumpBackImage, 1.0f, juce::Colours::transparentBlack,
        jumpBackImage, 1.0f, juce::Colours::lightgrey,
        jumpBackImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(jumpBackButton);
    jumpBackButton.addListener(this);

    jumpForwardButton.setImages(false, true, true,
        jumpForwardImage, 1.0f, juce::Colours::transparentBlack,
        jumpForwardImage, 1.0f, juce::Colours::lightgrey,
        jumpForwardImage, 1.0f, juce::Colours::grey);
    addAndMakeVisible(jumpForwardButton);
    jumpForwardButton.addListener(this);


    addAndMakeVisible(setAButton);
    addAndMakeVisible(setBButton);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(timeLabel);
    addAndMakeVisible(abLoopLabel);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(fileInfoLabel);
  
    addAndMakeVisible(addToPlaylistButton);
    addToPlaylistButton.addListener(this);

    addAndMakeVisible(playlistBox);
    playlistBox.setRowHeight(25);
    addAndMakeVisible(clearPlaylistButton);
    clearPlaylistButton.addListener(this);

    setAButton.addListener(this);
    setBButton.addListener(this);

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

    fileInfoLabel.setJustificationType(juce::Justification::centredLeft);
    fileInfoLabel.setFont(juce::Font(13.0f));
    fileInfoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    playlistBox.setModel(&playlistModel);



    startTimer(50);
    playlistModel.onItemClicked = [this](int index)
        {
            if (onTrackSelected)
                onTrackSelected(index);
        };

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
    auto area = getLocalBounds().reduced(10);
    auto topRow = area.removeFromTop(50);
    loadButton.setBounds(topRow.removeFromLeft(100));
    topRow.removeFromLeft(10);
    muteButton.setBounds(topRow.removeFromLeft(100));
    topRow.removeFromLeft(10);
    loopButton.setBounds(topRow.removeFromLeft(100));

    area.removeFromTop(10);
    auto progressArea = area.removeFromTop(70);
    positionSlider.setBounds(progressArea.removeFromTop(20));
    timeLabel.setBounds(progressArea.removeFromTop(25));
    abLoopLabel.setBounds(progressArea.removeFromTop(25));

    area.removeFromTop(10);
    auto playControls = area.removeFromTop(40);
    int buttonWidth = 70;

    gotoStartButton.setBounds(playControls.removeFromLeft(buttonWidth));
    playControls.removeFromLeft(5);
    jumpBackButton.setBounds(playControls.removeFromLeft(buttonWidth));
    playControls.removeFromLeft(5);
    playButton.setBounds(playControls.removeFromLeft(buttonWidth));
    playControls.removeFromLeft(5);
    pauseButton.setBounds(playControls.removeFromLeft(buttonWidth));
    playControls.removeFromLeft(5);
    jumpForwardButton.setBounds(playControls.removeFromLeft(buttonWidth));
    playControls.removeFromLeft(5);
    gotoEndButton.setBounds(playControls.removeFromLeft(buttonWidth));

    area.removeFromTop(10);
    auto abControls = area.removeFromTop(40);
    setAButton.setBounds(abControls.removeFromLeft(buttonWidth));
    abControls.removeFromLeft(5);
    setBButton.setBounds(abControls.removeFromLeft(buttonWidth));
    abControls.removeFromLeft(5);
    clearABButton.setBounds(abControls.removeFromLeft(buttonWidth));
    abControls.removeFromLeft(5);
    abLoopButton.setBounds(abControls.removeFromLeft(buttonWidth));

    area.removeFromTop(10);
    volumeSlider.setBounds(area.removeFromTop(30));
    area.removeFromTop(5);
    speedSlider.setBounds(area.removeFromTop(25));

    area.removeFromTop(10);
    waveformDisplay.setBounds(area.removeFromTop(100));
    area.removeFromTop(10);
    fileInfoLabel.setBounds(area);


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
                muteButton.setImages(false, true, true,
                    unmuteImage, 1.0f, juce::Colours::transparentBlack,
                    unmuteImage, 1.0f, juce::Colours::lightgrey,
                    unmuteImage, 1.0f, juce::Colours::grey);
            }
            else {
                BeforeMute = (float)volumeSlider.getValue();
                volumeSlider.setValue(0.0);
                isMuted = true;
                muteButton.setImages(false, true, true,
                    muteImage, 1.0f, juce::Colours::transparentBlack,
                    muteImage, 1.0f, juce::Colours::lightgrey,
                    muteImage, 1.0f, juce::Colours::grey);
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
                abLoopButton.setButtonText("Custom Loop: On");
            else
                abLoopButton.setButtonText("Custom Loop: Off");
        }
    }
    else if (button == &jumpBackButton)
    {
        if (audioPlayer)
            audioPlayer->jumpBackward(10.0);
    }
    else if (button == &jumpForwardButton)
    {
        if (audioPlayer)
            audioPlayer->jumpForward(10.0);
    }
    else if (button == &addToPlaylistButton)
    {
        if (onAddToPlaylistRequest)
            onAddToPlaylistRequest();
    }
    else if (button == &clearPlaylistButton)
    {
        if (onClearPlaylistRequest)
            onClearPlaylistRequest();
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

        fileInfoLabel.setText(audioPlayer->getFileInfo(), juce::dontSendNotification);
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
void PlayerGUI::updatePlaylistDisplay(const std::vector<juce::File>& files)
{
    playlistModel.items.clear();

    for (auto& f : files)
        playlistModel.items.push_back(f.getFileName());

    playlistBox.updateContent();
    repaint();
}




