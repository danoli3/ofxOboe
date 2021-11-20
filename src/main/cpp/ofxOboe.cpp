//
// Created by Dan Rosser on 2/11/21.
//

#include "ofxOboe.h"
#include <thread>

int ofxOboe::sampleRate = 44100;
int ofxOboe::channelCount  = 2;
int ofxOboe::volume = 1;

ofxOboeMixer ofxOboe::mixer;
std::shared_ptr<oboe::AudioStream> ofxOboe::audioStream = nullptr;
AAssetManager* ofxOboe::assetManager;

bool ofxOboe::openStream( int setSampleRate,
                                            int setChannelCount,
                                            oboe::AudioFormat format,
                                            oboe::PerformanceMode performanceMode,
                                            oboe::SharingMode sharingMode,
                                            oboe::SampleRateConversionQuality sampleRateConversion,
                                            bool allowFormatConversion) {

    ofxOboe::sampleRate = setSampleRate;
    ofxOboe::channelCount = setChannelCount;

    AudioStreamBuilder builder;
    builder.setFormat(format);
    builder.setFormatConversionAllowed(allowFormatConversion);
    builder.setPerformanceMode(performanceMode);
    builder.setSharingMode(sharingMode);
    builder.setSampleRate(sampleRate);
    builder.setSampleRateConversionQuality(
            sampleRateConversion);
    builder.setChannelCount(channelCount);
    builder.setDataCallback(this);
    builder.setErrorCallback(this);



    Result result = builder.openStream(audioStream);


    if (audioStream->getChannelCount() != channelCount) {
        // LOG HERE TO SHOW WE MESSED UP REALLY BAD ON CHANNEL COUNT
        LOGE("Requested %d channels BAD received %d", audioStream->getChannelCount(),
             channelCount);
    } else {
        LOGW("Requested %d channels OKAY : received %d", audioStream->getChannelCount(),
             channelCount);
    }

    ofxOboe::mixer.setChannelCount(audioStream->getChannelCount());

    if (result != Result::OK){
        LOGE("Failed to open stream. Error: %s", convertToText(result));
        return false;
    }
    return true;
}

void ofxOboe::start() {
    if(audioStream != nullptr) {
        Result result = audioStream->requestStart();
        if (result != Result::OK) {
            LOGE("Failed to start stream. Error: %s", convertToText(result));
            state = ofxOboeSoundState::FAILED_TO_LOAD;
            return;
        }
    } else {
        LOGE("Failed to start stream. Error: audioStream nullptr");
        state = ofxOboeSoundState::FAILED_TO_LOAD;
        return;
    }

    oboe::AudioFormat format = audioStream->getFormat();
    LOGI("AudioStream format is %s", oboe::convertToText(format));
    state = ofxOboeSoundState::READY;
}

oboe::DataCallbackResult ofxOboe::onAudioReady(AudioStream *oboeStream, void *audioData,
                                        int32_t numFrames) {
    auto *outputBuffer = static_cast<float *>(audioData);

    for (int i = 0; i < numFrames; ++i) {
        ofxOboe::mixer.renderAudio(outputBuffer+(oboeStream->getChannelCount()*i), 1);
    }
    return DataCallbackResult::Continue;
}

void ofxOboe::load() {
    LOGI("ofxOboe load");
    if (!openStream()) {
        state = ofxOboeSoundState::FAILED_TO_LOAD;
        return;
    }

//    if (!setupAudioSources()) {
//        state = ofxOboeSoundState::FAILED_TO_LOAD;
//        return;
//    }

    start();
}

void ofxOboe::restart() {
    LOGI("ofxOboe restart");
    load();
    //loadingResult = std::async(&ofxOboe::load, this);
}

void ofxOboe::onErrorAfterClose(AudioStream *oboeStream, Result error) {
    if (error == Result::ErrorDisconnected){
        LOGI("ofxOboe onErrorAfterClose ErrorDisconnected");
        state = ofxOboeSoundState::FAILED_TO_LOAD;
        if(audioStream)
            audioStream.reset();
        //ofxOboe::mixer.removeAllTracks();
        restart();
    } else {
        //LOGE("Stream error: %s", convertToText(error));
    }
}

void ofxOboe::setupAssets(AAssetManager *assets) {
    assetManager = assets;
}

std::shared_ptr<oboe::AudioStream> ofxOboe::getAudioStream() {
    return ofxOboe::audioStream;
}

int ofxOboe::getSampleRate() {
    return ofxOboe::sampleRate;
}

int ofxOboe::getChannelCount() {
    return ofxOboe::channelCount;
}

ofxOboe::~ofxOboe(){
    stop();
}

void ofxOboe::stop() {
    ofxOboe::mixer.removeAllTracks();
    if (ofxOboe::audioStream){
        ofxOboe::audioStream->stop();
        ofxOboe::audioStream->close();
        ofxOboe::audioStream.reset();
    }
}

AAssetManager& ofxOboe::getAssetManager() {
    return *ofxOboe::assetManager;
}

