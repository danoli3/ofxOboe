//
// Created by 3000 on 15/10/2021.
//

#ifndef OFXOBOEASSETDATASOURCE_H
#define OFXOBOEASSETDATASOURCE_H

#include <android/asset_manager.h>
#include <memory>
#include "ofMain.h"

struct AudioProperties {
    int32_t channelCount;
    int32_t sampleRate;
};

class DataSource {
public:
    virtual ~DataSource(){};
    virtual int64_t getSize() const = 0;
    virtual AudioProperties getProperties() const  = 0;
    virtual const float* getData() const = 0;
};


class ofxOboeAssetDataSource : public DataSource {

    public:
        int64_t getSize() const override { return bufferSize; }
        AudioProperties getProperties() const override { return bufferProperties; }
        const float* getData() const override { return buffer.get(); }

        static ofxOboeAssetDataSource* newFromCompressedAsset(
                AAssetManager &assetManager,
                const char *filename,
                AudioProperties targetProperties, int stream);

    private:

    ofxOboeAssetDataSource(std::unique_ptr<float[]> data, size_t size,
                         const AudioProperties properties)
                : buffer(std::move(data))
                , bufferSize(size)
                , bufferProperties(properties) {
        }

        const std::unique_ptr<float[]> buffer;
        const int64_t bufferSize;
        const AudioProperties bufferProperties;

};



#endif // OFXOBOEASSETDATASOURCE_H
