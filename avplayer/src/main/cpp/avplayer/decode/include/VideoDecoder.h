
#ifndef AVPLAYER_VIDEODECODER_H
#define AVPLAYER_VIDEODECODER_H

#include "BaseDecoder.h"
#include <RGBAData.h>

extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
};

class VideoDecoder : public BaseDecoder {
public:
    VideoDecoder(AVFormatContext *context): BaseDecoder(context) {}

    const char* LogSpec() override {
        return "VideoDecoder";
    }

    AVMediaType GetMediaType() override {
        return AVMEDIA_TYPE_VIDEO;
    }

    const char* GetPrintMediaType() override {
        return "AVMEDIA_TYPE_VIDEO";
    }

private:
    uint8_t *m_buf_for_rgb_frame = NULL;

    //视频格式转换器
    SwsContext *m_sws_ctx = NULL;
    AVFrame* rgb_frame;

    // TODO set by surface
    //显示的目标宽
    int m_dst_w = 1080;
    //显示的目标高
    int m_dst_h = 1920;

    void InitInternal() override;

    void InitSws();

    void InitBuffer();

    void* DecodeFrame(AVFrame* av_frame) override;
};


#endif //AVPLAYER_VIDEODECODER_H
