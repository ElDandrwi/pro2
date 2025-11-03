#pragma once
#include <JuceHeader.h>

class PlaylistGUI : public juce::Component,
    public juce::ListBoxModel
{
public:
    PlaylistGUI();
    ~PlaylistGUI() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

    // · ÕœÌÀ ⁄—÷ «·„·›« 
    void updatePlaylistDisplay(const std::vector<juce::File>& newPlaylist);

    // === Callbacks ===
    std::function<void()> onAddToPlaylistRequest;
    std::function<void()> onClearPlaylistRequest;
    std::function<void(int)> onTrackSelected;

private:
    juce::TextButton addButton{ "Add to Playlist" };
    juce::TextButton clearButton{ "Clear Playlist" };
    juce::ListBox playlistBox;

    std::vector<juce::File> playlist;

    // „‰ ListBoxModel
    int getNumRows() override;
    void paintListBoxItem(int row, juce::Graphics&, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent&) override;
};
