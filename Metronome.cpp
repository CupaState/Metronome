/*
  ==============================================================================

    Metronome.cpp
    Created: 19 Jun 2020 5:55:25pm
    Author:  olegi

  ==============================================================================
*/

#include "Metronome.h"

Metronome::Metronome()
{
    mFormatManager.registerBasicFormats();
    File myFile{ File::getSpecialLocation(File::SpecialLocationType::userDesktopDirectory) };
    auto mySamples(myFile.findChildFiles(File::TypesOfFileToFind::findFiles, true, "Kick.wav"));
    jassert(mySamples[0].exists());

    auto formatReader = mFormatManager.createReaderFor(mySamples[0]);

    pClickSample.reset(new AudioFormatReaderSource(formatReader, true));

    mUpdateInterval = 60 / mBPM * mSampleRate;
    
    pClickSample->setNextReadPosition(0);
}

Metronome::~Metronome()
{
}

void Metronome::reset()
{
    mTotalSamples = 0;
    pClickSample->setNextReadPosition(0);
}

void Metronome::prepareToPlay(int samplePerBlock, double sampleRate)
{
    mSampleRate = sampleRate;
    mUpdateInterval = 60 / mBPM * mSampleRate;

    if (pClickSample != nullptr)
    {
        pClickSample->prepareToPlay(samplePerBlock, sampleRate);
    }
    pClickSample->setNextReadPosition(0);
}

void Metronome::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    auto bufferSize = bufferToFill.numSamples;

    mTotalSamples += bufferSize;

    mSampleRemaining = mTotalSamples % mUpdateInterval;

    if ((mSampleRemaining + bufferSize) >= mUpdateInterval)
    {
        const auto timeToStart = mUpdateInterval - mSampleRemaining;
        pClickSample->setNextReadPosition(0);

        for (auto sample = 0; sample < bufferSize; sample++)
        {
            if (sample == timeToStart)
            {
                pClickSample->getNextAudioBlock(bufferToFill);
            }
        }
    }

    if (pClickSample->getNextReadPosition() != 0)
    {
        pClickSample->getNextAudioBlock(bufferToFill);
    }
}