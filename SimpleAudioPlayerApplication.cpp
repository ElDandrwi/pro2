#include "SimpleAudioPlayerApplication.h"

void SimpleAudioPlayerApplication::initialise(const juce::String&)
{
    mainWindow = std::make_unique<MainWindow>(getApplicationName());
}

void SimpleAudioPlayerApplication::shutdown()
{
    mainWindow = nullptr;
}

// ---------- MainWindow ----------
SimpleAudioPlayerApplication::MainWindow::MainWindow(juce::String name)
    : DocumentWindow(name, juce::Colours::lightgrey, DocumentWindow::allButtons)
{
    setUsingNativeTitleBar(true);
    setContentOwned(new MainComponent(), true);
    centreWithSize(800, 600); // ÎáíåÇ ÃßÈÑ ãä 400x200 ÚÔÇä ßá ÍÇÌÉ ÊÈÇä
    setVisible(true);
}

void SimpleAudioPlayerApplication::MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
