//
// Created by Dan Rosser on 10/9/21.
//

#include "ofxOboeAndroidSoundPlayer.h"

bool ofxOboeAndroidSoundPlayer::load(const std::filesystem::path& fileName, bool stream) {
    AudioProperties targetProperties {
            .channelCount = ofxOboe::getChannelCount(),
            .sampleRate = ofxOboe::getSampleRate()
    };


    filePath = fileName.c_str();
    std::shared_ptr<ofxOboeAssetDataSource> trackSource {
            ofxOboeAssetDataSource::newFromCompressedAsset(ofxOboe::getAssetManager(), fileName.c_str(), targetProperties, AASSET_MODE_UNKNOWN)
    };
    if (trackSource == nullptr){
        ofLogError("ofxOboe") <<  "Could not load source data for track:" << fileName;
        return false;
    }
    track = std::make_unique<ofxOboePlayer>(trackSource);
    if(track != nullptr) {
        track->setPlaying(false);
        track->setLooping(isLooping);
        ofxOboe::mixer.addTrack(track.get());
    } else
        return false;

    return true;
}

void ofxOboeAndroidSoundPlayer::unload() {
    if(track) {
        track->setPlaying(false);
        track.reset();
        track.release();
        track = nullptr;
        state = ofxOboeAndroidSoundState::UNLOADED;
    }
}
void ofxOboeAndroidSoundPlayer::play() {

    if(track)
        track->setPlaying(true);

}

void ofxOboeAndroidSoundPlayer::stop() {
    if(track) {
        track->setPlaying(false);
    }
}

void ofxOboeAndroidSoundPlayer::setVolume(float vol) {

    if(track) {
        track->setVolume(vol);
    }
}
void ofxOboeAndroidSoundPlayer::setPan(float vol) {

}
void ofxOboeAndroidSoundPlayer::setSpeed(float spd) {

}
void ofxOboeAndroidSoundPlayer::setPaused(bool bP) {
    if(track) {
        track->setPlaying(false);
    }

}
void ofxOboeAndroidSoundPlayer::setLoop(bool bLp) {
    if(track) {
        track->setLooping(bLp);
    }
}
void ofxOboeAndroidSoundPlayer::setMultiPlay(bool bMp) {

}
void ofxOboeAndroidSoundPlayer::setPosition(float pct) {
    if(track) {
        track->setPlayHead((int)pct);
    }
}
void ofxOboeAndroidSoundPlayer::setPositionMS(int ms) {
    if(track) {
         track->setPlayHead(ms);
    }
}

float ofxOboeAndroidSoundPlayer::getPosition() const {
    return 1.0f;
}
int ofxOboeAndroidSoundPlayer::getPositionMS() const {
    return 0;
}
bool ofxOboeAndroidSoundPlayer::isPlaying() const {
    if(track) {
        return track->getPlaying();
    }
    return false;
}
float ofxOboeAndroidSoundPlayer::getSpeed() const {
    return 1.0f;
}
float ofxOboeAndroidSoundPlayer::getPan() const {
    return 1.0f;
}
bool ofxOboeAndroidSoundPlayer::isPaused() const {
    return false;
}
float ofxOboeAndroidSoundPlayer::getVolume() const {
    return 1.0f;
}
bool ofxOboeAndroidSoundPlayer::isLoaded() const {
    if(track) {
        return true;
    }
    return false;
}

void ofxOboeAndroidSoundPlayer::audioIn(ofSoundBuffer&) const {

}
void ofxOboeAndroidSoundPlayer::audioOut(ofSoundBuffer&) const {

}

ofxOboeAndroidSoundPlayer::~ofxOboeAndroidSoundPlayer() {

}

oboe::DataCallbackResult
ofxOboeAndroidSoundPlayer::onAudioReady(AudioStream *oboeStream, void *audioData,
                                        int32_t numFrames) {
//    auto *outputBuffer = static_cast<float *>(audioData);
//
//    int64_t nextClapEventMs;
//
//    for (int i = 0; i < numFrames; ++i) {
//
//        songPositionMS = convertFramesToMillis(
//                currentFrame,
//                ofxOboe::getSampleRate());
//
//
//        ofxOboe::mixer.renderAudio(outputBuffer+(oboeStream->getChannelCount()*i), 1);
//        currentFrame++;
//    }

    return DataCallbackResult::Continue;
}

void ofxOboeAndroidSoundPlayer::onErrorAfterClose(AudioStream *oboeStream, Result error) {
    if (error == Result::ErrorDisconnected){
        state = ofxOboeAndroidSoundState::FAILED;
        currentFrame = 0;
        songPositionMS = 0;
        lastUpdateTime = 0;
        //start();
    } else {
        //LOGE("Stream error: %s", convertToText(error));
    }
}

