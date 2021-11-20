//
// Created by 3000 on 15/10/2021.
//

#include "ofxOboeNDKDecoder.h"
#include <sys/types.h>
#include "ofMain.h"
#include <cstring>
#include "ofLog.h"
#include <media/NdkMediaExtractor.h>

#include <cinttypes>


int32_t ofxOboeNDKDecoder::decode(AAsset *asset, uint8_t *targetData, AudioProperties targetProperties) {

    //ofLogDebug("ofxOboe") << "ofxOboeNDKDecoder::Using NDK decoder";

    // open asset as file descriptor
    off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);

    // Extract the audio frames
    AMediaExtractor *extractor = AMediaExtractor_new();
    media_status_t amresult = AMediaExtractor_setDataSourceFd(extractor, fd,
                                                              static_cast<off64_t>(start),
                                                              static_cast<off64_t>(length));
    if (amresult != AMEDIA_OK){
        LOGE("Error setting extractor data source, err %d", amresult);
        return 0;
    }

    // Specify our desired output format by creating it from our source
    AMediaFormat *format = AMediaExtractor_getTrackFormat(extractor, 0);

    int32_t sampleRate;
    if (AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_SAMPLE_RATE, &sampleRate)){
        LOGD("Source sample rate %d", sampleRate);
        if (sampleRate != targetProperties.sampleRate){
            LOGE("Input (%d) and output (%d) sample rates do not match. "
                 "NDK decoder does not support resampling.",
                 sampleRate,
                 targetProperties.sampleRate);
            return 0;
        }
    } else {
        ofLogError("ofxOboe") << "ofxOboeNDKDecoder::Failed to get sample rate";
        return 0;
    };

    int32_t channelCount;
    if (AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_CHANNEL_COUNT, &channelCount)){
        ofLogNotice("ofxOboe") << "ofxOboeNDKDecoder::Got channel count" << channelCount;
        if (channelCount != targetProperties.channelCount){
            LOGE("NDK decoder does not support different "
                 "input (%d) and output (%d) channel counts",
                 channelCount,
                 targetProperties.channelCount);

        }
    } else {
        LOGE("Failed to get channel count");
        return 0;
    }

    const char *formatStr = AMediaFormat_toString(format);
    LOGD("Output format %s", formatStr);

    const char *mimeType;
    if (AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &mimeType)) {
        LOGD("Got mime type %s", mimeType);
    } else {
        LOGE("Failed to get mime type");
        return 0;
    }

    // Obtain the correct decoder
    AMediaCodec *codec = nullptr;
    AMediaExtractor_selectTrack(extractor, 0);
    codec = AMediaCodec_createDecoderByType(mimeType);
    AMediaCodec_configure(codec, format, nullptr, nullptr, 0);
    AMediaCodec_start(codec);

    // DECODE

    bool isExtracting = true;
    bool isDecoding = true;
    int32_t bytesWritten = 0;

    while(isExtracting || isDecoding){

        if (isExtracting){

            // Obtain the index of the next available input buffer
            ssize_t inputIndex = AMediaCodec_dequeueInputBuffer(codec, 2000);
            //LOGI("Got input buffer %d", inputIndex);

            // The input index acts as a status if its negative
            if (inputIndex < 0){
                if (inputIndex == AMEDIACODEC_INFO_TRY_AGAIN_LATER){
                    LOGE("Codec.dequeueInputBuffer try again later");
                } else {
                    ofLogError("ofxOboe") << "ofxOboeNDKDecoder::Codec.dequeueInputBuffer unknown error status";
                }
            } else {

                // Obtain the actual buffer and read the encoded data into it
                size_t inputSize;
                uint8_t *inputBuffer = AMediaCodec_getInputBuffer(codec, inputIndex, &inputSize);
                //LOGI("Sample size is: %d", inputSize);

                ssize_t sampleSize = AMediaExtractor_readSampleData(extractor, inputBuffer, inputSize);
                auto presentationTimeUs = AMediaExtractor_getSampleTime(extractor);

                if (sampleSize > 0){

                    // Enqueue the encoded data
                    AMediaCodec_queueInputBuffer(codec, inputIndex, 0, sampleSize,
                                                 presentationTimeUs,
                                                 0);
                    AMediaExtractor_advance(extractor);

                } else {
                    ofLogNotice("ofxOboe") << "ofxOboeNDKDecoder::End of extractor data stream";
                    isExtracting = false;

                    // We need to tell the codec that we've reached the end of the stream
                    AMediaCodec_queueInputBuffer(codec, inputIndex, 0, 0,
                                                 presentationTimeUs,
                                                 AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
                }
            }
        }

        if (isDecoding){
            // Dequeue the decoded data
            AMediaCodecBufferInfo info;
            ssize_t outputIndex = AMediaCodec_dequeueOutputBuffer(codec, &info, 0);

            if (outputIndex >= 0){

                // Check whether this is set earlier
                if (info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM){
                    ofLogNotice("ofxOboe") << "ofxOboeNDKDecoder::Reached end of decoding stream";
                    isDecoding = false;
                }

                // Valid index, acquire buffer
                size_t outputSize;
                uint8_t *outputBuffer = AMediaCodec_getOutputBuffer(codec, outputIndex, &outputSize);

                /*LOGV("Got output buffer index %d, buffer size: %d, info size: %d writing to pcm index %d",
                     outputIndex,
                     outputSize,
                     info.size,
                     m_writeIndex);*/

                // copy the data out of the buffer
                memcpy(targetData + bytesWritten, outputBuffer, info.size);
                bytesWritten+=info.size;
                AMediaCodec_releaseOutputBuffer(codec, outputIndex, false);
            } else {

                // The outputIndex doubles as a status return if its value is < 0
                switch(outputIndex){
                    case AMEDIACODEC_INFO_TRY_AGAIN_LATER:
                        ofLogError("ofxOboe") << "ofxOboeNDKDecoder::dequeueOutputBuffer: try again later";
                        break;
                    case AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED:
                        ofLogError("ofxOboe") << "ofxOboeNDKDecoder::dequeueOutputBuffer: output buffers changed";
                        break;
                    case AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED:
                        ofLogError("ofxOboe") << "ofxOboeNDKDecoder::dequeueOutputBuffer: output outputFormat changed";
                        format = AMediaCodec_getOutputFormat(codec);
                        //ofLogDebug("ofxOboe") << "ofxOboeNDKDecoder::outputFormat changed to: %s" << AMediaFormat_toString(format);
                        break;
                }
            }
        }
    }

    // Clean up
    AMediaFormat_delete(format);
    AMediaCodec_delete(codec);
    AMediaExtractor_delete(extractor);

    return bytesWritten;
}