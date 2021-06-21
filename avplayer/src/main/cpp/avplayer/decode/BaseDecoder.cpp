
#include "include/BaseDecoder.h"

BaseDecoder::BaseDecoder(AVFormatContext *context) {
    this->av_format_ctx = context;
}

BaseDecoder::~BaseDecoder() {
    Free();
}

void BaseDecoder::InitDecodeThread() {
    std::thread t(Decode, this);
    t.detach();
}

void BaseDecoder::Decode(BaseDecoder *that) {
    that->SetDecoderState(START);
    LOGI(that->TAG, "start decode");

    while (true) {
        pthread_mutex_lock(&that->mutex);
        if (that->GetDecoderStatus() == STOP) {
            pthread_mutex_unlock(&that->mutex);
            break;
        }

        while (that->av_packet_queue.empty()) {
            pthread_cond_wait(&that->cond, &that->mutex);
        }

        pthread_mutex_unlock(&that->mutex);
    }
}

void BaseDecoder::SetDecoderState(DecodeStatus status) {
    pthread_mutex_lock(&decoder_status_lock);
    decoder_status = status;
    pthread_mutex_unlock(&decoder_status_lock);
}

DecodeStatus BaseDecoder::GetDecoderStatus() {
    DecodeStatus current_decoder_status = IDLE;
    pthread_mutex_lock(&decoder_status_lock);
    current_decoder_status = decoder_status;
    pthread_mutex_unlock(&decoder_status_lock);
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
        LOGE(TAG, "can not find stream for media type %s", GetPrintMediaType());
        SetDecoderState(ERROR);
        return;
    }
    stream_index = index;
    LOGI(TAG, "media_type=%s stream_index=%d", GetPrintMediaType(), stream_index);
}

void BaseDecoder::FindDecoder() {
    if (GetDecoderStatus() == ERROR) {
        return;
    }

    // 为解码器上下文分配空间
    av_codec_ctx = avcodec_alloc_context3(av_codec);
    if (!av_codec_ctx) {
        LOGE(TAG, "can not alloc codec context");
        SetDecoderState(ERROR);
        return;
    }

    // 寻找解码器
    AVCodecParameters *codec_param = av_format_ctx->streams[stream_index]->codecpar;
    av_codec = avcodec_find_decoder(codec_param->codec_id);
    if (!av_codec_ctx) {
        LOGE(TAG, "can not find decoder for codec_id %d", codec_param->codec_id);
        SetDecoderState(ERROR);
        return;
    }

    // 初始化解码器上下文
    int ret = avcodec_parameters_to_context(av_codec_ctx, codec_param);
    if (ret) {
        LOGE(TAG, "can not obtain codec context");
        SetDecoderState(ERROR);
        return;
    }

    // 打开解码器
    ret = avcodec_open2(av_codec_ctx, av_codec, nullptr);
    if (ret < 0) {
        LOGE(TAG, "can not open codec");
        SetDecoderState(ERROR);
        return;
    }

    // 获取时长
    duration = (long) ((float) av_format_ctx->duration / AV_TIME_BASE * 1000);
    LOGI(TAG, "duration=%ld", duration);
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
    LOGD(TAG, "Push Packet %s", GetPrintMediaType());
    pthread_mutex_lock(&mutex);
    av_packet_queue.push(av_packet);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void BaseDecoder::Release() {
    pthread_mutex_lock(&mutex);
    SetDecoderState(STOP);
    pthread_mutex_unlock(&mutex);
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