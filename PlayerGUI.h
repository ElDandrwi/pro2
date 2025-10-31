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
    std::function<void()> onLoadFileRequest;

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

    juce::TextButton loadButton{ "Load Files" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton gotoStartButton{ "Start" };
    juce::TextButton gotoEndButton{ "End" };
    juce::TextButton loopButton{ "Loop: Off" };
    juce::TextButton setAButton{ "A" };
    juce::TextButton setBButton{ "B" };
    juce::TextButton clearABButton{ "Clear" };
    juce::TextButton abLoopButton{ "A-B Loop: Off" };
	juce::Slider speedSlider;
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Label timeLabel;
    juce::Label abLoopLabel;
  juce::Label fileInfoLabel;

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





