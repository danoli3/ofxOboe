//
// Created by Dan Rosser on 10/9/21.
//

#ifndef ANDROID_OFXOBOE_H
#define ANDROID_OFXOBOE_H

#include <oboe/Oboe.h>
#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofSoundBaseTypes.h"
#include "ofxOboe.h"
#include <math.h>
#include "ofxOboeNDKDecoder.h"
#include "ofxOboeAssetDataSource.h"
#include "ofxOboePlayer.h"
#include "ofxOboeMixer.h"
#include <memory>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

using namespace oboe;

enum class ofxOboeAndroidSoundState {
    UNLOADED,
    LOADING,
    PLAYING,
    FAILED
};

class ofxOboeAndroidSoundPlayer: public ofBaseSoundPlayer, public AudioStreamDataCallback, public AudioStreamErrorCallback {
    public:
        ofxOboeAndroidSoundPlayer() {
        };
        ~ofxOboeAndroidSoundPlayer();

        bool load(const std::filesystem::path& fileName, bool stream = false) override;
        void unload() override;
        void play() override;
        void stop() override;

        void setVolume(float vol) override;
        void setPan(float vol) override;
        void setSpeed(float spd) override;
        void setPaused(bool bP) override;
        void setLoop(bool bLp) override;
        void setMultiPlay(bool bMp) override;
        void setPosition(float pct) override; // 0 = start, 1 = end;
        void setPositionMS(int ms) override;

        float getPosition() const override;
        int getPositionMS() const override;
        bool isPlaying() const override;
        float getSpeed() const override;
        float getPan() const override;
        bool isPaused() const;
        float getVolume() const override;
        bool isLoaded() const override;

        void audioIn(ofSoundBuffer&) const;
        void audioOut(ofSoundBuffer&) const;

        DataCallbackResult
        onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) override;

        // Inherited from oboe::AudioStreamErrorCallback.
        void onErrorAfterClose(AudioStream *oboeStream, Result error) override;





private:

    ofxOboeAndroidSoundState state;

    bool isLooping = false;
    std::unique_ptr<ofxOboePlayer> track;
    AAssetManager* assetManager;

    string filePath;

    std::atomic<int64_t> currentFrame { 0 };
    std::atomic<int64_t> songPositionMS { 0 };
    std::atomic<int64_t> lastUpdateTime { 0 };




};


#endif //SUPERHEXAGON_ANDROID_OFXOBOE_H
