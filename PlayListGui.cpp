#include "PlaylistGUI.h"

PlaylistGUI::PlaylistGUI()
{
    addAndMakeVisible(addButton);
    addAndMakeVisible(clearButton);
    addAndMakeVisible(playlistBox);

    playlistBox.setModel(this);

    addButton.onClick = [this]()
        {
            if (onAddToPlaylistRequest)
                onAddToPlaylistRequest();
        };

    clearButton.onClick = [this]()
        {
            if (onClearPlaylistRequest)
                onClearPlaylistRequest();
        };
}

void PlaylistGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void PlaylistGUI::resized()
{
    auto area = getLocalBounds().reduced(10);
    auto topArea = area.removeFromTop(40);

    addButton.setBounds(topArea.removeFromLeft(150));
    clearButton.setBounds(topArea.removeFromLeft(150).reduced(10, 0));

    playlistBox.setBounds(area);
}

void PlaylistGUI::updatePlaylistDisplay(const std::vector<juce::File>& newPlaylist)
{
    playlist = newPlaylist;
    playlistBox.updateContent();
}

int PlaylistGUI::getNumRows()
{
    return (int)playlist.size();
}

void PlaylistGUI::paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::darkgrey);

    g.setColour(juce::Colours::white);
    g.drawText(playlist[row].getFileName(), 10, 0, width - 20, height, juce::Justification::centredLeft);
}

void PlaylistGUI::listBoxItemClicked(int row, const juce::MouseEvent&)
{
    if (onTrackSelected)
        onTrackSelected(row);
}
