//
// Created by 3000 on 15/10/2021.
//

#include "ofxOboeAssetDataSource.h"
#include <oboe/Oboe.h>

#if !defined(USE_FFMPEG)
//#error USE_FFMPEG should be defined in app.gradle
#endif

#if USE_FFMPEG==1
#include "FFMpegDecoder.h"
#else
#include "ofxOboeNDKDecoder.h"
#endif

#include "ofMain.h"
#include "ofLog.h"

constexpr int kMaxCompressionRatio { 12 };

ofxOboeAssetDataSource* ofxOboeAssetDataSource::newFromCompressedAsset(
        AAssetManager &assetManager,
        const char *filename,
        const AudioProperties targetProperties, int stream) {

    AAsset *asset = AAssetManager_open(&assetManager, filename, stream);
    if (!asset) {
        LOGE("Failed to open asset %s", filename);
        return nullptr;
    }

    off_t assetSize = AAsset_getLength(asset);
    LOGD("Opened %s, size %ld", filename, assetSize);

    // Allocate memory to store the decompressed audio. We don't know the exact
    // size of the decoded data until after decoding so we make an assumption about the
    // maximum compression ratio and the decoded sample format (float for FFmpeg, int16 for NDK).
#if USE_FFMPEG==true
    const long maximumDataSizeInBytes = kMaxCompressionRatio * assetSize * sizeof(float);
    auto decodedData = new uint8_t[maximumDataSizeInBytes];

    int64_t bytesDecoded = FFMpegExtractor::decode(asset, decodedData, targetProperties);
    auto numSamples = bytesDecoded / sizeof(float);
#else
    const long maximumDataSizeInBytes = kMaxCompressionRatio * assetSize * sizeof(int16_t);
    auto decodedData = new uint8_t[maximumDataSizeInBytes];

    int64_t bytesDecoded = ofxOboeNDKDecoder::decode(asset, decodedData, targetProperties);
    auto numSamples = bytesDecoded / sizeof(int16_t);
#endif

    // Now we know the exact number of samples we can create a float array to hold the audio data
    auto outputBuffer = std::make_unique<float[]>(numSamples);

#if USE_FFMPEG==1
    memcpy(outputBuffer.get(), decodedData, (size_t)bytesDecoded);
#else
    // The NDK decoder can only decode to int16, we need to convert to floats
    oboe::convertPcm16ToFloat(
            reinterpret_cast<int16_t*>(decodedData),
            outputBuffer.get(),
            bytesDecoded / sizeof(int16_t));
#endif

    delete[] decodedData;
    AAsset_close(asset);

    return new ofxOboeAssetDataSource(std::move(outputBuffer),
                                numSamples,
                                targetProperties);
}