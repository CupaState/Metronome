/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(btnPlay);
    addAndMakeVisible(btnStop);

    btnPlay.setToggleState(true, NotificationType::dontSendNotification);
    btnStop.setToggleState(false, NotificationType::dontSendNotification);

    btnPlay.onClick = [this]() { play(); };
    btnStop.onClick = [this]() { stop(); };

    btnPlay.setRadioGroupId(1);
    btnStop.setRadioGroupId(1);

    setSize (200, 200);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::play()
{
    playState = State::PAYING;
}

void MainComponent::stop()
{
    playState = State::STOPPED;
    metronome.reset();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    metronome.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
        bufferToFill.clearActiveBufferRegion();

        if (playState == State::PAYING)
        {
            metronome.getNextAudioBlock(bufferToFill);
        }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{    
    g.fillAll (Colours::black);
}

void MainComponent::resized()
{
    Rectangle<int>bounds = getLocalBounds();

    FlexBox flexbox;
    flexbox.items.add(FlexItem(100, 100, btnPlay));
    flexbox.items.add(FlexItem(100, 100, btnStop));
    flexbox.performLayout(bounds);
}
