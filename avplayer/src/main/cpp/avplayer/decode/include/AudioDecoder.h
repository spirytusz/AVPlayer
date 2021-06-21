
#ifndef AVPLAYER_AUDIODECODER_H
#define AVPLAYER_AUDIODECODER_H

#include "BaseDecoder.h"
#include "log.h"

class AudioDecoder : public BaseDecoder {

public:
    AudioDecoder(AVFormatContext *context);

    const char* LogSpec() override {
        return "AudioDecoder";
    }

    AVMediaType GetMediaType() override {
        return AVMEDIA_TYPE_AUDIO;
    }

    const char* GetPrintMediaType() override {
        return "AVMEDIA_TYPE_AUDIO";
    }

};


#endif //AVPLAYER_AUDIODECODER_H
