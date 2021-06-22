
#include "include/BaseDecoder.h"

BaseDecoder::BaseDecoder(AVFormatContext *context) {
    this->av_format_ctx = context;
}

BaseDecoder::~BaseDecoder() {
    Free();
}

void BaseDecoder::InitDecodeThread() {
    LOGD(LogSpec(), "send signal decoder_status_cond");
    pthread_cond_signal(&decoder_status_cond);
    SetDecoderState(START);
    if (decode_thread_tid) {
        LOGD("BaseDecoder", "%s already create thread, skip", LogSpec());
        return;
    }
    LOGD("BaseDecoder", "%s create thread", LogSpec());
    pthread_create(&decode_thread_tid, nullptr, Decode, this);
}

void* BaseDecoder::Decode(void* pVoid) {
    auto decoder = static_cast<BaseDecoder*>(pVoid);
    decoder->RealDecode();
    return nullptr;
}

void BaseDecoder::RealDecode() {
    SetDecoderState(START);
    LOGI(LogSpec(), "start decode");

    while (true) {
        while (GetDecoderStatus() == PAUSE) {
            pthread_mutex_lock(&decoder_status_mutex);
            LOGD(LogSpec(), "wait signal decoder_status_cond");
            pthread_cond_wait(&decoder_status_cond, &decoder_status_mutex);
            LOGD(LogSpec(), "receive signal decoder_status_cond");
            pthread_mutex_unlock(&decoder_status_mutex);
        }

        if (GetDecoderStatus() == STOP) {
            break;
        }

        while (av_packet_queue.empty()) {
            pthread_mutex_lock(&av_packet_queue_mutex);
            pthread_cond_wait(&av_packet_queue_cond, &av_packet_queue_mutex);
            pthread_mutex_unlock(&av_packet_queue_mutex);
        }

        AVPacket* av_packet = av_packet_queue.front();
        av_packet_queue.pop();
        if (!av_packet) {
            LOGW(LogSpec(), "invalid av_packet, skip");
            continue;
        }
        LOGD(LogSpec(), "read av_packet size=%d", av_packet->size);

        int ret = avcodec_send_packet(av_codec_ctx, av_packet);
        if (ret) {
            LOGW(LogSpec(), "avcodec_send_packet failed by %d", ret);
            continue;
        }

        av_packet_free(&av_packet);

        AVFrame* av_frame = av_frame_alloc();
        ret = avcodec_receive_frame(av_codec_ctx, av_frame);
        if (ret == AVERROR(EAGAIN)) {
            LOGI(LogSpec(), "avcodec_receive_frame %s", av_err2str(AVERROR(EAGAIN)));
            continue;
        } else if (ret != 0) {
            LOGD(LogSpec(), "avcodec_receive_frame %d", ret);
            av_frame_free(&av_frame);
            break;
        }

        if (m_render) {
            m_render->Render(av_frame);
        }
    }
}

void BaseDecoder::SetDecoderState(DecodeStatus status) {
    pthread_mutex_lock(&decoder_status_mutex);
    decoder_status = status;
    pthread_mutex_unlock(&decoder_status_mutex);
}

DecodeStatus BaseDecoder::GetDecoderStatus() {
    DecodeStatus current_decoder_status = IDLE;
    pthread_mutex_lock(&decoder_status_mutex);
    current_decoder_status = decoder_status;
    pthread_mutex_unlock(&decoder_status_mutex);
    return current_decoder_status;
}

void BaseDecoder::Init() {
    SetDecoderState(PREPARE);
    FindTargetStream();
    FindDecoder();
    if (GetDecoderStatus() == ERROR) {
        Free();
        return;
    }
    SetDecoderState(READY);
}

void BaseDecoder::FindTargetStream() {
    int index = -1;
    for (int i = 0; i < av_format_ctx->nb_streams; i++) {
        int media_type = av_format_ctx->streams[i]->codecpar->codec_type;
        if (media_type == GetMediaType()) {
            index = i;
            break;
        }
    }

    if (index < 0) {
        LOGE(LogSpec(), "can not find stream for media type %s", GetPrintMediaType());
        SetDecoderState(ERROR);
        return;
    }
    stream_index = index;
    LOGI(LogSpec(), "media_type=%s stream_index=%d", GetPrintMediaType(), stream_index);
}

void BaseDecoder::FindDecoder() {
    if (GetDecoderStatus() == ERROR) {
        return;
    }

    // 为解码器上下文分配空间
    av_codec_ctx = avcodec_alloc_context3(av_codec);
    if (!av_codec_ctx) {
        LOGE(LogSpec(), "can not alloc codec context");
        SetDecoderState(ERROR);
        return;
    }

    // 寻找解码器
    AVCodecParameters *codec_param = av_format_ctx->streams[stream_index]->codecpar;
    av_codec = avcodec_find_decoder(codec_param->codec_id);
    if (!av_codec_ctx) {
        LOGE(LogSpec(), "can not find decoder for codec_id %d", codec_param->codec_id);
        SetDecoderState(ERROR);
        return;
    }

    // 初始化解码器上下文
    int ret = avcodec_parameters_to_context(av_codec_ctx, codec_param);
    if (ret) {
        LOGE(LogSpec(), "can not obtain codec context");
        SetDecoderState(ERROR);
        return;
    }

    // 打开解码器
    ret = avcodec_open2(av_codec_ctx, av_codec, nullptr);
    if (ret < 0) {
        LOGE(LogSpec(), "can not open codec");
        SetDecoderState(ERROR);
        return;
    }

    // 获取时长
    duration = (long) ((float) av_format_ctx->duration / AV_TIME_BASE * 1000);
    LOGI(LogSpec(), "duration=%ld", duration);
}

void BaseDecoder::Free() {
    if (av_codec_ctx) {
        avcodec_close(av_codec_ctx);
        avcodec_free_context(&av_codec_ctx);
        delete av_codec_ctx;
    }
}

void BaseDecoder::Start() {
    InitDecodeThread();
}

void BaseDecoder::Pause() {
    SetDecoderState(PAUSE);
}

void BaseDecoder::Push(AVPacket *av_packet) {
    pthread_mutex_lock(&av_packet_queue_mutex);
    av_packet_queue.push(av_packet);
    pthread_cond_signal(&av_packet_queue_cond);
    pthread_mutex_unlock(&av_packet_queue_mutex);
}

void BaseDecoder::SetRender(IRender *render) {
    m_render = render;
}

void BaseDecoder::Release() {
    pthread_mutex_lock(&av_packet_queue_mutex);
    SetDecoderState(STOP);
    pthread_mutex_unlock(&av_packet_queue_mutex);
    Free();
}

long BaseDecoder::GetCurrentPosition() {
    return current_t_ms;
}

long BaseDecoder::GetDuration() {
    return duration;
}

int BaseDecoder::GetStreamIndex() {
    return stream_index;
}