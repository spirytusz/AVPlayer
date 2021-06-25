
#include "include/VideoDecoder.h"

void *VideoDecoder::DecodeFrame(AVFrame *av_frame) {
    if (!m_sws_ctx) {
        return nullptr;
    }
    sws_scale(m_sws_ctx, av_frame->data, av_frame->linesize, 0,
              VideoHeight(), dst_data, dst_linesize);
    auto *rgba_data = new RGBAData();
    rgba_data->data = dst_data[0];
    rgba_data->line_size = dst_linesize[0];
    rgba_data->pts = av_frame->best_effort_timestamp * av_q2d(GetTimeBase());
    rgba_data->extra_delay = av_frame->repeat_pict * 1.0 / fps * 2.0;
    rgba_data->time_base = GetTimeBase();
    rgba_data->stream_index = stream_index;
    return rgba_data;
}

void VideoDecoder::InitInternal() {
    InitFps();
}

void VideoDecoder::InitSws() {
    // 初始化格式转换工具
    m_sws_ctx = sws_getContext(VideoWidth(), VideoHeight(), av_codec_ctx->pix_fmt,
                               m_dst_width, m_dst_height, AV_PIX_FMT_RGBA,
                               SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
}

void VideoDecoder::InitBuffer() {
    int ret = av_image_alloc(
            dst_data,
            dst_linesize,
            m_dst_width,
            m_dst_height,
            AV_PIX_FMT_RGBA,
            1
    );
    if (ret < 0) {
        LOGE(LogSpec(), "Could not allocate source image %d", ret);
    }
}

void VideoDecoder::InitFps() {
    fps = (int) av_q2d(av_format_ctx->streams[stream_index]->avg_frame_rate);
    LOGD(LogSpec(), "fps=%d", fps);
}

void VideoDecoder::AfterSetRender(IRender *render) {
    auto video_render = dynamic_cast<VideoRender *>(render);
    SetDstSize(video_render->GetRenderWidth(), video_render->GetRenderHeight());
    LOGD(LogSpec(), "SetDstSize(width=%d, height=%d)", video_render->GetRenderWidth(),
         video_render->GetRenderHeight());
    InitSws();
    InitBuffer();
}