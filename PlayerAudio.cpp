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

    if (isLoopEnabled && readerSource != nullptr)
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

        transportSource.start();
    }
}

void PlayerAudio::setGain(float gain)
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

void PlayerAudio::setLooping(bool shouldLoop)
{
    isLoopEnabled = shouldLoop;

    if (readerSource != nullptr)
    {
        readerSource->setLooping(shouldLoop);
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
