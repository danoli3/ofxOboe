//
// Created by Dan Rosser on 15/10/2021.
//

#include "ofxOboePlayer.h"


void ofxOboePlayer::renderAudio(float *targetData, int32_t numFrames){

    const AudioProperties properties = source->getProperties();

    if (isPlaying){

        int64_t framesToRenderFromData = numFrames;
        int64_t totalSourceFrames = source->getSize() / properties.channelCount;
        const float *data = source->getData();

        // Check whether we're about to reach the end of the recording
        if (!isLooping && mReadFrameIndex + numFrames >= totalSourceFrames){
            framesToRenderFromData = totalSourceFrames - mReadFrameIndex;
            isPlaying = false;
        }

        for (int i = 0; i < framesToRenderFromData; ++i) {
            for (int j = 0; j < properties.channelCount; ++j) {
                targetData[(i*properties.channelCount)+j] = data[(mReadFrameIndex*properties.channelCount)+j] * volume;
            }

            // Increment and handle wraparound
            if (++mReadFrameIndex >= totalSourceFrames) mReadFrameIndex = 0;
        }

        if (framesToRenderFromData < numFrames){
            // fill the rest of the buffer with silence
            renderSilence(&targetData[framesToRenderFromData], numFrames * properties.channelCount);
        }

    } else {
        renderSilence(targetData, numFrames * properties.channelCount);
    }
}

int16_t ofxOboePlayer::hardLimiter(int16_t sample) {
    int16_t audioData = sample * volume;
    if(audioData >= INT16_MAX){
        return INT16_MAX;
    }else if(audioData <= INT16_MIN){
        return  INT16_MIN;
    }
    return audioData;
};


void ofxOboePlayer::renderSilence(float *start, int32_t numSamples){
    for (int i = 0; i < numSamples; ++i) {
        start[i] = 0;
    }
}

void ofxOboePlayer::setVolume(float volume) {
    this->volume = volume * 1.37f;
}