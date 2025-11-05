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

    void loadFileForWaveform(const juce::File& file);

    void updateFileInfoLabel(const juce::String& text)
    {
        fileInfoLabel.setText(text, juce::dontSendNotification);
    }
    void updatePlaylistDisplay(const std::vector<juce::File>& files);
    std::function<void()> onAddToPlaylistRequest;
    std::function<void(int)> onTrackSelected;
    /*
        void updateSaveLabel(const juce::String& text)
        {
            SaveLabel.setText(text, juce::dontSendNotification);
        }
    */
    std::function<void()> onLoadFileRequest;
    std::function<void()> onClearPlaylistRequest;


private:
    struct WaveformDisplay : public juce::Component, public juce::ChangeListener
    {
        juce::AudioThumbnail& thumbnail;
        float position = 0.0f;

        WaveformDisplay(juce::AudioThumbnail& t) : thumbnail(t)
        {
            thumbnail.addChangeListener(this);
        }

        ~WaveformDisplay() override
        {
            thumbnail.removeChangeListener(this);
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::black);
            g.setColour(juce::Colours::darkgreen);

            const auto bounds = getLocalBounds().toFloat().reduced(2.0f);

            if (thumbnail.getTotalLength() > 0.0)
            {
                thumbnail.drawChannels(g, bounds.toNearestInt(), 0.0, (double)thumbnail.getTotalLength(), 1.0f);

                const float x = bounds.getX() + bounds.getWidth() * juce::jlimit(0.0f, 1.0f, position);
                g.setColour(juce::Colours::orange);
                g.drawLine(x, bounds.getY(), x, bounds.getBottom(), 2.0f);
            }
            else
            {
                g.setColour(juce::Colours::white.withAlpha(0.6f));
                g.drawFittedText("No waveform", getLocalBounds(), juce::Justification::centred, 1);
            }
        }

        void changeListenerCallback(juce::ChangeBroadcaster*) override
        {
            repaint();
        }

        void setPositionRelative(double p)
        {
            position = (float)p;
            repaint();
        }
    };

    class PlaylistModel : public juce::ListBoxModel
    {
    public:
        std::vector<juce::String> items;
        std::function<void(int)> onItemClicked;

        int getNumRows() override { return (int)items.size(); }

        void paintListBoxItem(int rowNumber, juce::Graphics& g,
            int width, int height, bool rowIsSelected) override
        {
            if (rowIsSelected)
                g.fillAll(juce::Colours::lightblue);
            else
                g.fillAll(juce::Colours::darkgrey);

            g.setColour(juce::Colours::white);
            g.drawText(items[rowNumber], 5, 0, width - 10, height, juce::Justification::centredLeft);
        }

        void listBoxItemClicked(int row, const juce::MouseEvent&) override
        {
            if (onItemClicked)
                onItemClicked(row);
        }
    };
    PlaylistModel playlistModel;
    juce::ImageButton loadButton;
    juce::ImageButton muteButton;
    juce::ImageButton playButton;
    juce::ImageButton pauseButton;
    juce::ImageButton gotoStartButton;
    juce::ImageButton gotoEndButton;
    juce::ImageButton loopButton;
    juce::TextButton setAButton{ "A" };
    juce::TextButton setBButton{ "B" };
    juce::ImageButton clearABButton;
    juce::ImageButton abLoopButton;
    juce::ImageButton jumpBackButton;
    juce::ImageButton jumpForwardButton;
    juce::ImageButton clearPlaylistButton;


    juce::Image muteImage{ juce::ImageCache::getFromMemory(BinaryData::mute_png, BinaryData::mute_pngSize) };
    juce::Image unmuteImage{ juce::ImageCache::getFromMemory(BinaryData::unmute_png, BinaryData::unmute_pngSize) };
    juce::Image loadImage{ juce::ImageCache::getFromMemory(BinaryData::load_png, BinaryData::load_pngSize) };
    juce::Image playImage{ juce::ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize) };
    juce::Image pauseImage{ juce::ImageCache::getFromMemory(BinaryData::pause_png, BinaryData::pause_pngSize) };
    juce::Image StartImage{ juce::ImageCache::getFromMemory(BinaryData::start_png, BinaryData::start_pngSize) };
    juce::Image EndImage{ juce::ImageCache::getFromMemory(BinaryData::end_png, BinaryData::end_pngSize) };
    juce::Image loopImage{ juce::ImageCache::getFromMemory(BinaryData::loop_png, BinaryData::loop_pngSize) };
    juce::Image jumpBackImage{ juce::ImageCache::getFromMemory(BinaryData::replay_10_png, BinaryData::replay_10_pngSize) };
    juce::Image jumpForwardImage{ juce::ImageCache::getFromMemory(BinaryData::forward_10_png, BinaryData::forward_10_pngSize) };
    juce::Image clearImage{ juce::ImageCache::getFromMemory(BinaryData::clear_png, BinaryData::clear_pngSize) };
    juce::Image loopABImage{ juce::ImageCache::getFromMemory(BinaryData::loopAB_png, BinaryData::loopAB_pngSize) };

    juce::ListBox playlistBox;
    juce::TextButton addToPlaylistButton{ "Add to Playlist" };
    std::vector<juce::String> playlistNames;
    juce::Slider speedSlider;
    juce::Slider volumeSlider;
    juce::Slider positionSlider;

    juce::Label timeLabel;
    juce::Label abLoopLabel;
    juce::Label fileInfoLabel;
    //juce::Label SaveLabel;

    PlayerAudio* audioPlayer = nullptr;

    float BeforeMute = 0.5f;
    bool isMuted = false;

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{ 5 };
    juce::AudioThumbnail thumbnail{ 50,formatManager, thumbnailCache };
    WaveformDisplay waveformDisplay{ thumbnail };

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
    void updateTimeDisplay();
    void updateABLoopDisplay();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};





