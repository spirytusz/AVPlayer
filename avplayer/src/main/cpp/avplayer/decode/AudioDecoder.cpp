
#include "include/AudioDecoder.h"

void *AudioDecoder::DecodeFrame(AVFrame *av_frame) {
    auto *pcmData = new PCMData();
    int ret = swr_convert(m_swr, m_out_buffer, m_dest_data_size / 2,
                          (const uint8_t **) av_frame->data, av_frame->nb_samples);
    if (ret <= 0) {
        LOGE(LogSpec(), "swr_convert failed by %d", ret);
        return nullptr;
    } else {
        LOGE(LogSpec(), "swr_convert success by %d", ret);
    }
    pcmData->pcm = m_out_buffer[0];
    pcmData->size = m_dest_data_size;
    pcmData->stream_index = stream_index;
    pcmData->pts = av_frame->best_effort_timestamp * av_q2d(GetTimeBase());
    return pcmData;
}

void AudioDecoder::InitInternal() {
    InitSwr();
    CalculateSampleArgs();
    InitOutBuffer();
}

void AudioDecoder::InitSwr() {
    //初始化格式转换工具
    m_swr = swr_alloc();

    av_opt_set_int(m_swr, "in_channel_layout", av_codec_ctx->channel_layout, 0);
    av_opt_set_int(m_swr, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);

    av_opt_set_int(m_swr, "in_sample_rate", av_codec_ctx->sample_rate, 0);
    av_opt_set_int(m_swr, "out_sample_rate", av_codec_ctx->sample_rate, 0);

    av_opt_set_sample_fmt(m_swr, "in_sample_fmt", av_codec_ctx->sample_fmt, 0);
    av_opt_set_sample_fmt(m_swr, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

    swr_init(m_swr);

    LOGI(LogSpec(), "sample rate: %d, channel: %d, format: %d, frame_size: %d, layout: %lld",
         av_codec_ctx->sample_rate, av_codec_ctx->channels, av_codec_ctx->sample_fmt,
         av_codec_ctx->frame_size, av_codec_ctx->channel_layout);
}

void AudioDecoder::CalculateSampleArgs() {
    // 重采样后一个通道采样数
    m_dest_nb_sample = (int) av_rescale_rnd(1024, av_codec_ctx->sample_rate,
                                            av_codec_ctx->sample_rate, AV_ROUND_UP);

    // 重采样后一帧数据的大小
    m_dest_data_size = (size_t) av_samples_get_buffer_size(
            nullptr, 2,
            m_dest_nb_sample, AV_SAMPLE_FMT_S16, 1);
}

void AudioDecoder::InitOutBuffer() {
    m_out_buffer[0] = (uint8_t *) malloc(m_dest_data_size);
}

void AudioDecoder::AfterSetRender(IRender *render) {}