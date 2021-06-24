
#include "include/VideoDecoder.h"

void *VideoDecoder::DecodeFrame(AVFrame *av_frame) {
    int ret = sws_scale(m_sws_ctx, av_frame->data, av_frame->linesize, 0,
                        av_codec_ctx->width, rgb_frame->data, rgb_frame->linesize);
    if (ret <= 0) {
        LOGD(LogSpec(), "sws_scale failed by %d", ret);
        return nullptr;
    } else {
        LOGD(LogSpec(), "sws_scale success by %d", ret);
    }
    auto *rgba_data = new RGBAData();
    rgba_data->data = rgb_frame->data[0];
    rgba_data->line_size = rgb_frame->linesize[0];
    rgba_data->pts = rgb_frame->pts;
    rgba_data->time_base = GetTimeBase();
    LOGD(LogSpec(), "before sws_scale return");
    return rgba_data;
}

void VideoDecoder::InitInternal() {
    InitSws();
    InitBuffer();
}

void VideoDecoder::InitSws() {
    // 初始化格式转换工具
    m_sws_ctx = sws_getContext(av_codec_ctx->width, av_codec_ctx->height, av_codec_ctx->pix_fmt,
                               m_dst_w, m_dst_h, AV_PIX_FMT_RGBA,
                               SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
}

void VideoDecoder::InitBuffer() {
    rgb_frame = av_frame_alloc();
    // 获取缓存大小
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_dst_w, m_dst_h, 1);
    // 分配内存
    m_buf_for_rgb_frame = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    // 将内存分配给RgbFrame，并将内存格式化为三个通道后，分别保存其地址
    av_image_fill_arrays(rgb_frame->data, rgb_frame->linesize,
                         m_buf_for_rgb_frame, AV_PIX_FMT_RGBA, m_dst_w, m_dst_h, 1);
}