//
// Created by Dan Rosser on 2/11/21.
//

#include "ofxOboeMixer.h"

void ofxOboeMixer::renderAudio(float *audioData, int32_t numFrames) {

    // Zero out the incoming container array
    memset(audioData, 0, sizeof(float) * numFrames * channelCount);

    for (int i = 0; i < nextFreeTrackIndex; ++i) {
        tracks[i]->renderAudio(mixingBuffer, numFrames);

        for (int j = 0; j < numFrames * channelCount; ++j) {
            audioData[j] += mixingBuffer[j];
        }
    }
}

void ofxOboeMixer::addTrack(IRenderableAudio *renderer){
    tracks[nextFreeTrackIndex++] = renderer;
}

void ofxOboeMixer::setChannelCount(int32_t channelCount){
    channelCount = channelCount;
}

void ofxOboeMixer::removeAllTracks(){
    for (int i = 0; i < nextFreeTrackIndex; i++){
        tracks[i] = nullptr;
    }
    nextFreeTrackIndex = 0;
}