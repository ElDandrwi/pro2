
#pragma once

#include <JuceHeader.h>
#include <map>

class PlayerAudio : public juce::AudioSource
{
public:
    PlayerAudio();
    ~PlayerAudio() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadFile(const juce::File& file);
    void setGain(double gain);
    void play();
    void pause();
    void setPosition(double seconds);
    double getLengthInSeconds() const;
    void goToStart();
    void goToEnd();
    void save(const juce::File& file);

    void setLooping(bool shouldLoop);
    bool isLooping() const { return isLoopEnabled; }

    void setSpeed(double speed);

    void setABLoopPointA(double seconds);
    void setABLoopPointB(double seconds);
    void clearABLoopPoints();
    void setABLoopEnabled(bool enabled);
    bool getABLoopEnabled() const { return isABLoopEnabled; }
    double getABLoopPointA() const { return abLoopPointA; }
    double getABLoopPointB() const { return abLoopPointB; }
    bool hasABLoopPoints() const { return abLoopPointA >= 0 && abLoopPointB > abLoopPointA; }

    double getCurrentPosition() const;
    double getPositionRatio() const;

    void jumpForward(double seconds);
    void jumpBackward(double seconds);
    void addFileToPlaylist(const juce::File& file);
    void playTrack(int index);
    void playNextTrack();
    void playPreviousTrack();
    bool isPlaying() const;
    std::vector<juce::File> getPlaylist() const { return playlist; }

    juce::String getFileInfo() const;
    void clearPlaylist();

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    bool isLoopEnabled = false;
    bool isABLoopEnabled = false;
    double abLoopPointA = -1.0;
    double abLoopPointB = -1.0;
    double totalLengthInSeconds = 0.0;
    double currentPos = 0.0;
    juce::File currentFile;
    std::map<juce::File, double> SaveList;
    std::vector<juce::File> playlist;
    int currentTrackIndex = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};


