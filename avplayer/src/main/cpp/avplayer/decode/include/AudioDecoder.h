
#ifndef AVPLAYER_AUDIODECODER_H
#define AVPLAYER_AUDIODECODER_H

#include "BaseDecoder.h"
#include "log.h"

class AudioDecoder : public BaseDecoder {

public:
    AudioDecoder(AVFormatContext *context);

private:
    AVMediaType GetMediaType() override {
        return AVMEDIA_TYPE_AUDIO;
    }

    char* GetPrintMediaType() override {
        return "AVMEDIA_TYPE_AUDIO";
    }
};


#endif //AVPLAYER_AUDIODECODER_H
