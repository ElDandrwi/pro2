#pragma once
#include <JuceHeader.h>
#include "MainComponent.h"

class SimpleAudioPlayerApplication : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "Simple Audio Player"; }
    const juce::String getApplicationVersion() override { return "1.0"; }

    void initialise(const juce::String&) override;
    void shutdown() override;

private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name);
        void closeButtonPressed() override;
    };

    std::unique_ptr<MainWindow> mainWindow;
};
