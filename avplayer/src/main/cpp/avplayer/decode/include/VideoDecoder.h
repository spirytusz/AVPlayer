
#ifndef AVPLAYER_VIDEODECODER_H
#define AVPLAYER_VIDEODECODER_H

#include "BaseDecoder.h"
#include <RGBAData.h>
#include <VideoRender.h>
#include <ctime>

extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
};

class VideoDecoder : public BaseDecoder {
public:
    VideoDecoder(AVFormatContext *context) : BaseDecoder(context) {}

    const char *LogSpec() override {
        return "VideoDecoder";
    }

    AVMediaType GetMediaType() override {
        return AVMEDIA_TYPE_VIDEO;
    }

    const char *GetPrintMediaType() override {
        return "AVMEDIA_TYPE_VIDEO";
    }

private:

    uint8_t *dst_data[4];
    int dst_linesize[4];

    //视频格式转换器
    SwsContext *m_sws_ctx = NULL;
    AVFrame *rgb_frame;

    int m_dst_width = -1;
    int m_dst_height = -1;

    int fps = 0;

    void InitInternal() override;

    void InitSws();

    void InitBuffer();

    void InitFps();

    void *DecodeFrame(AVFrame *av_frame) override;

    int VideoWidth() {
        return av_codec_ctx->width;
    }

    int VideoHeight() {
        return av_codec_ctx->height;
    }

    void SetDstSize(int dst_width, int dst_height) {
        this->m_dst_width = dst_width;
        this->m_dst_height = dst_height;
    }

    void AfterSetRender(IRender *render) override;
};


#endif //AVPLAYER_VIDEODECODER_H
