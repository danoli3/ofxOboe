//
// Created by Dan Rosser on 15/10/2021.
//

#ifndef SUPERHEXAGON_ANDROID_OFXOBOENDKDECODER_H
#define SUPERHEXAGON_ANDROID_OFXOBOENDKDECODER_H

#include <cstdint>
#include <android/asset_manager.h>
#include "ofxOboeAssetDataSource.h"
#include "ofxOboe.h"

class ofxOboeNDKDecoder {

    public:
        static int32_t decode(AAsset *asset, uint8_t *targetData, AudioProperties targetProperties);

};


#endif //ANDROID_OFXOBOENDKDECODER_H
