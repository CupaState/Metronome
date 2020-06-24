/*
  ==============================================================================

    Metronome.h
    Created: 19 Jun 2020 5:55:25pm
    Author:  olegi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Metronome
{
public:
    Metronome();

    ~Metronome();

    void prepareToPlay(int samplePerBlock, double sampleRate);

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);

    void reset();

private:
    int mTotalSamples{ 0 };
    double mSampleRate{ 0 };
    double mBPM{ 120 };

    int mUpdateInterval{ 0 };
    int mSampleRemaining{ 0 };

    AudioFormatManager mFormatManager;
    std::unique_ptr<AudioFormatReaderSource>pClickSample{ nullptr };
};