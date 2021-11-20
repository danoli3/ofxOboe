//
// Created by Dan Rosser on 15/10/2021.
//

#ifndef ANDROID_OFXOBOEPLAYER_H
#define ANDROID_OFXOBOEPLAYER_H

#include <cstdint>
#include <array>

#include <chrono>
#include <memory>
#include <atomic>
#include "ofMain.h"
#include <android/asset_manager.h>

#include "ofxOboeMixer.h"
#include "ofxOboeAssetDataSource.h"

#include "ofxOboe.h"

class ofxOboePlayer : public IRenderableAudio {


    public:
        /**
         * Construct a new Player from the given DataSource. Players can share the same data source.
         * For example, you could play two identical sounds concurrently by creating 2 Players with the
         * same data source.
         *
         * @param source
         */
        ofxOboePlayer(std::shared_ptr<ofxOboeAssetDataSource> source)
                : source(source)
        {};

        void renderAudio(float *targetData, int32_t numFrames);

        void resetPlayHead() { mReadFrameIndex = 0; };
        void setPlayHead(int ms) { mReadFrameIndex = ms; };

        void setPlaying(bool isPlaying) { this->isPlaying = isPlaying; resetPlayHead(); };
        void setLooping(bool isLooping) { this->isLooping = isLooping; };


        bool getPlaying() { return this->isPlaying; };
        bool getLooping() { return this->isLooping; };


    void setVolume(float volume);



    private:
        int32_t mReadFrameIndex = 0;
        std::atomic<bool> isPlaying { false };
        std::atomic<bool> isLooping { false };
        std::atomic<float> volume { 1.0f };


        std::shared_ptr<ofxOboeAssetDataSource> source;

        int16_t hardLimiter(int16_t sample);

        void renderSilence(float*, int32_t);
 };


#endif //ANDROID_OFXOBOEPLAYER_H
