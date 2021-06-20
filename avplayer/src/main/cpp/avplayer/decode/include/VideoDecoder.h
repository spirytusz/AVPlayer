
#ifndef AVPLAYER_VIDEODECODER_H
#define AVPLAYER_VIDEODECODER_H

#include "BaseDecoder.h"

class VideoDecoder : public BaseDecoder {
public:
    VideoDecoder(AVFormatContext *context);

private:
    AVMediaType GetMediaType() override {
        return AVMEDIA_TYPE_VIDEO;
    }

    char* GetPrintMediaType() override {
        return "AVMEDIA_TYPE_VIDEO";
    }
};


#endif //AVPLAYER_VIDEODECODER_H
