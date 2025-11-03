#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);

    if (isABLoopEnabled && hasABLoopPoints())
    {
        double currentPos = getCurrentPosition();

        if (currentPos >= abLoopPointB)
        {
            transportSource.setPosition(abLoopPointA);
        }
    }
    else if (isLoopEnabled && readerSource != nullptr)
    {
        if (transportSource.hasStreamFinished())
        {
            transportSource.setPosition(0.0);
        }
    }
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

void PlayerAudio::loadFile(const juce::File& file)
{
    if (auto* reader = formatManager.createReaderFor(file))
    {
        transportSource.stop();
        save(currentFile);

        transportSource.setSource(nullptr);
        readerSource.reset();

        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

        transportSource.setSource(readerSource.get(),
            0,
            nullptr,
            reader->sampleRate);

        if (readerSource != nullptr)
        {
            totalLengthInSeconds = readerSource->getTotalLength() / reader->sampleRate;
            readerSource->setLooping(isLoopEnabled);
        }

        if (SaveList.find(file) != SaveList.end())
        {
            transportSource.setPosition(SaveList[file]);
        }
        else
        {
            transportSource.setPosition(0.0);
        }

        clearABLoopPoints();
        currentFile = file;
        transportSource.start();
    }
}

void PlayerAudio::setGain(double gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::play()
{
    transportSource.start();
}

void PlayerAudio::pause()
{
    transportSource.stop();
}

void PlayerAudio::setPosition(double seconds)
{
    transportSource.setPosition(seconds);
}

double PlayerAudio::getLengthInSeconds() const
{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::goToStart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::goToEnd()
{
    transportSource.setPosition(transportSource.getLengthInSeconds());
}

void PlayerAudio::save(const juce::File& file)
{
    if (readerSource == nullptr)
        return;
    auto* reader = readerSource->getAudioFormatReader();
    if (reader == nullptr)
        return;
    currentPos = transportSource.getCurrentPosition();
    if (SaveList.find(file) != SaveList.end())
        SaveList.erase(file);
    SaveList.insert(std::make_pair(file, currentPos));
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    isLoopEnabled = shouldLoop;

    if (readerSource != nullptr)
    {
        readerSource->setLooping(shouldLoop);
    }
}

void PlayerAudio::setABLoopPointA(double seconds)
{
    double length = getLengthInSeconds();
    if (length > 0.0)
    {
        abLoopPointA = juce::jlimit(0.0, length, seconds);

        if (abLoopPointB >= 0 && abLoopPointB <= abLoopPointA)
        {
            abLoopPointB = abLoopPointA + 1.0;
            if (abLoopPointB > length)
                abLoopPointB = length;
        }
    }
}

void PlayerAudio::setABLoopPointB(double seconds)
{
    double length = getLengthInSeconds();
    if (length > 0.0)
    {
        abLoopPointB = juce::jlimit(0.0, length, seconds);

        if (abLoopPointA >= 0 && abLoopPointA >= abLoopPointB)
        {
            abLoopPointA = abLoopPointB - 1.0;
            if (abLoopPointA < 0)
                abLoopPointA = 0;
        }
    }
}

void PlayerAudio::clearABLoopPoints()
{
    abLoopPointA = -1.0;
    abLoopPointB = -1.0;
    isABLoopEnabled = false;
}

void PlayerAudio::setABLoopEnabled(bool enabled)
{
    isABLoopEnabled = enabled && hasABLoopPoints();

    if (isABLoopEnabled)
    {
        isLoopEnabled = false;
        if (readerSource != nullptr)
        {
            readerSource->setLooping(false);
        }
    }
}

double PlayerAudio::getCurrentPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getPositionRatio() const
{
    double length = getLengthInSeconds();
    if (length > 0.0)
        return getCurrentPosition() / length;
    return 0.0;
}

void PlayerAudio::setSpeed(double speed)
{
    if (readerSource == nullptr)
        return;

    currentPos = transportSource.getCurrentPosition();
    const bool wasPlaying = transportSource.isPlaying();

    if (wasPlaying)
        transportSource.stop();

    transportSource.setSource(readerSource.get(),
        0,
        nullptr,
        readerSource->getAudioFormatReader()->sampleRate * speed);

    transportSource.setPosition(currentPos);

    if (wasPlaying)
        transportSource.start();
}

void PlayerAudio::jumpForward(double seconds)
{
    double currentPos = getCurrentPosition();
    double newPos = currentPos + seconds;
    double totalLength = getLengthInSeconds();

    if (newPos > totalLength)
        newPos = totalLength;

    setPosition(newPos);
}

void PlayerAudio::jumpBackward(double seconds)
{
    double currentPos = getCurrentPosition();
    double newPos = currentPos - seconds;

    if (newPos < 0.0)
        newPos = 0.0;

    setPosition(newPos);
}

juce::String PlayerAudio::getFileInfo() const
{
    if (currentFile.existsAsFile())
        return currentFile.getFileName() + " (" + juce::String(totalLengthInSeconds, 1) + "s)";
    return "No file loaded";
}
/*
juce::String PlayerAudio::getSave() const
{
    juce::String info;
    info += " --Saved-- \n";

    for (const auto& pair : SaveList)
    {
        juce::String filePath = pair.first.getFileName();
        double position = pair.second;
        info += "File: " + filePath + "\n | Position: " + juce::String(position, 2) + " sec\n\n";
    }
    return info;
}
*/
