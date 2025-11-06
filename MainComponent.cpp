
#include "MainComponent.h"

MainComponent::MainComponent()
{

    guiPlayer1.setAudioPlayer(&audioPlayer1);
    guiPlayer1.onLoadFileRequest = [this]()
        {
            fileChooser = std::make_unique<juce::FileChooser>(
                "Select an audio file for Track 1...",
                juce::File{},
                "*.wav;*.mp3;*.aiff;*.flac");

            fileChooser->launchAsync(
                juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                [this](const juce::FileChooser& fc)
                {
                    auto file = fc.getResult();
                    if (file.existsAsFile())
                    {
                        audioPlayer1.loadFile(file);
                        guiPlayer1.loadFileForWaveform(file);
                        guiPlayer1.updateFileInfoLabel(audioPlayer1.getFileInfo());
                    }
                });
        };
    addAndMakeVisible(guiPlayer1);

 
    guiPlayer2.setAudioPlayer(&audioPlayer2);
    guiPlayer2.onLoadFileRequest = [this]()
        {
            fileChooser = std::make_unique<juce::FileChooser>(
                "Select an audio file for Track 2...",
                juce::File{},
                "*.wav;*.mp3;*.aiff;*.flac");

            fileChooser->launchAsync(
                juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                [this](const juce::FileChooser& fc)
                {
                    auto file = fc.getResult();
                    if (file.existsAsFile())
                    {
                        audioPlayer2.loadFile(file);
                        guiPlayer2.loadFileForWaveform(file);
                        guiPlayer2.updateFileInfoLabel(audioPlayer2.getFileInfo());
                    }
                });
        };
    addAndMakeVisible(guiPlayer2);
    addAndMakeVisible(playlistGUI);
    playlistGUI.onAddToPlaylistRequest = [this]()
        {
            fileChooser = std::make_unique<juce::FileChooser>(
                "Select audio files...",
                juce::File{},
                "*.wav;*.mp3");

            fileChooser->launchAsync(
                juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectMultipleItems,
                [this](const juce::FileChooser& fc)
                {
                    juce::Array<juce::File> chosen = fc.getResults();
                    for (auto& file : chosen)
                        audioPlayer.addFileToPlaylist(file);

                    playlistGUI.updatePlaylistDisplay(audioPlayer.getPlaylist());
                });
        };


    playlistGUI.onTrackSelected = [this](int index)
        {
            auto playlist = audioPlayer.getPlaylist();
            if (index >= 0 && index < (int)playlist.size())
            {
                juce::File file = playlist[index];


                if (!audioPlayer1.isPlaying())
                {
                    audioPlayer1.loadFile(file);
                    guiPlayer1.loadFileForWaveform(file);
                    guiPlayer1.updateFileInfoLabel(audioPlayer1.getFileInfo());
                }
                else
                {
                    audioPlayer2.loadFile(file);
                    guiPlayer2.loadFileForWaveform(file);
                    guiPlayer2.updateFileInfoLabel(audioPlayer2.getFileInfo());
                }
            }
        };

    playlistGUI.onClearPlaylistRequest = [this]()
        {
            audioPlayer.clearPlaylist();
            playlistGUI.updatePlaylistDisplay(audioPlayer.getPlaylist());
        };


    addAndMakeVisible(masterVolumeSlider);
    masterVolumeSlider.setRange(0.0, 1.0, 0.01);
    masterVolumeSlider.setValue(0.7);
    masterVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);

    addAndMakeVisible(masterVolumeLabel);
    masterVolumeLabel.setText("Main Volume", juce::dontSendNotification);

    addAndMakeVisible(balanceSlider);
    balanceSlider.setRange(0.0, 1.0, 0.01);
    balanceSlider.setValue(0.5);
    balanceSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);

    addAndMakeVisible(balanceLabel);
    balanceLabel.setText("Balance", juce::dontSendNotification);


    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audioPlayer1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    audioPlayer2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    juce::AudioBuffer<float> tempBuffer1(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
    juce::AudioBuffer<float> tempBuffer2(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);

    juce::AudioSourceChannelInfo tempInfo1(&tempBuffer1, 0, bufferToFill.numSamples);
    juce::AudioSourceChannelInfo tempInfo2(&tempBuffer2, 0, bufferToFill.numSamples);

    audioPlayer1.getNextAudioBlock(tempInfo1);
    audioPlayer2.getNextAudioBlock(tempInfo2);

    float balanceValue = (float)balanceSlider.getValue();
    float gain1 = 1.0f - balanceValue;
    float gain2 = balanceValue;

    tempBuffer1.applyGain(gain1);
    tempBuffer2.applyGain(gain2);

    for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ++ch)
    {
        bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, tempBuffer1, ch, 0, bufferToFill.numSamples);
        bufferToFill.buffer->addFrom(ch, bufferToFill.startSample, tempBuffer2, ch, 0, bufferToFill.numSamples);
    }

    bufferToFill.buffer->applyGain((float)masterVolumeSlider.getValue());
}

void MainComponent::releaseResources()
{
    audioPlayer1.releaseResources();
    audioPlayer2.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
      g.fillAll(juce::Colour::fromRGB(40, 30, 60));
}

void MainComponent::resized()
{
    auto area = getLocalBounds();


    auto playlistArea = area.removeFromBottom(180);


    auto halfWidth = area.getWidth() / 2;
    guiPlayer1.setBounds(area.removeFromLeft(halfWidth).reduced(10));
    guiPlayer2.setBounds(area.reduced(10));


    playlistGUI.setBounds(playlistArea.reduced(10));


    auto controls = playlistArea.removeFromTop(40);
    masterVolumeLabel.setBounds(controls.removeFromLeft(100).reduced(5));
    masterVolumeSlider.setBounds(controls.removeFromLeft(150).reduced(5));
    balanceLabel.setBounds(controls.removeFromLeft(100).reduced(5));
    balanceSlider.setBounds(controls.removeFromLeft(150).reduced(5));
}

