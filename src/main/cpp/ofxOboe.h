//
// Created by Dan Rosser on 2/11/21.
//

#ifndef SUPERHEXAGON_ANDROID_OFXOBOE_H
#define SUPERHEXAGON_ANDROID_OFXOBOE_H

#include <oboe/Oboe.h>
#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofSoundBaseTypes.h"
#include <math.h>
#include "ofxOboeNDKDecoder.h"
#include "ofxOboeAssetDataSource.h"
#include "ofxOboePlayer.h"
#include "ofxOboeMixer.h"
#include <memory>
#include <stdint.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "ofxOboe", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "ofxOboe", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "ofxOboe", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "ofxOboe", __VA_ARGS__))


using namespace oboe;

enum class ofxOboeSoundState {
    LOADING,
    READY,
    FAILED_TO_LOAD
};



constexpr int64_t kMillisecondsInSecond = 1000;
constexpr int64_t kNanosecondsInMillisecond = 1000000;

constexpr int64_t convertFramesToMillis(const int64_t frames, const int sampleRate){
    return static_cast<int64_t>((static_cast<double>(frames)/ sampleRate) * kMillisecondsInSecond);
}


class ofxOboe: public AudioStreamDataCallback, public AudioStreamErrorCallback {
public:
    ~ofxOboe();
    bool openStream(int sampleRate = 44000, int channelCount = 2, oboe::AudioFormat format = oboe::AudioFormat::Float, oboe::PerformanceMode performanceMode = oboe::PerformanceMode::LowLatency, oboe::SharingMode sharingMode = oboe::SharingMode::Exclusive, oboe::SampleRateConversionQuality sampleRateConversion = oboe::SampleRateConversionQuality::Medium, bool allowFormatConversion = false);
    void setupAssets(AAssetManager* assets);

    DataCallbackResult
    onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) override;

    // Inherited from oboe::AudioStreamErrorCallback.
    void onErrorAfterClose(AudioStream *oboeStream, Result error) override;


    void load();
    void restart();
    void start();
    void stop();

    static ofxOboeMixer mixer;
    static std::shared_ptr<oboe::AudioStream> audioStream;
    static AAssetManager* assetManager;

    static std::shared_ptr<oboe::AudioStream> getAudioStream();
    static int getSampleRate();
    static int getChannelCount();
    static AAssetManager& getAssetManager();


    static int sampleRate;
    static int channelCount;
    static int volume;

private:
    ofxOboeSoundState state;

};


#endif //SUPERHEXAGON_ANDROID_OFXOBOE_H
