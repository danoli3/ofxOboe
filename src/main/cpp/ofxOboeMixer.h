//
// Created by Dan Rosser on 2/11/21.
//

#ifndef SUPERHEXAGON_ANDROID_OFXOBOEMIXER_H
#define SUPERHEXAGON_ANDROID_OFXOBOEMIXER_H

#include <array>

constexpr int32_t kBufferSize = 192*10;  // Temporary buffer is used for mixing
constexpr uint8_t kMaxTracks = 100;

/**
 * A Mixer object which sums the output from multiple tracks into a single output. The number of
 * input channels on each track must match the number of output channels (default 1=mono). This can
 * be changed by calling `setChannelCount`.
 * The inputs to the mixer are not owned by the mixer, they should not be deleted while rendering.
 */

class IRenderableAudio {

public:
    virtual ~IRenderableAudio() = default;
    virtual void renderAudio(float *audioData, int32_t numFrames) = 0;
};

class ofxOboeMixer : public IRenderableAudio {

public:
    void renderAudio(float *audioData, int32_t numFrames);

    void addTrack(IRenderableAudio *renderer);
    void setChannelCount(int32_t channelCount);

    void removeAllTracks();

private:
    float mixingBuffer[kBufferSize];
    std::array<IRenderableAudio*, kMaxTracks> tracks;
    uint8_t nextFreeTrackIndex = 0;
    int32_t channelCount = 2; // Default to mono 1
};


#endif //SUPERHEXAGON_ANDROID_OFXOBOEMIXER_H
