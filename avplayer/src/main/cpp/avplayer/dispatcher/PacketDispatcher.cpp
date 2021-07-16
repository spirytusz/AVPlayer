
#include "include/PacketDispatcher.h"

#include <utility>

PacketDispatcher::PacketDispatcher(AVFormatContext *context,
                                   const std::vector<BaseDecoder *> &decoders) {
    video_stream_aware = new BaseStreamAware("video_packet_dispatcher");
    audio_stream_aware = new BaseStreamAware("audio_packet_dispatcher");

    this->av_format_context = context;
    for (auto decoder: decoders) {
        if (decoder->GetMediaType() == AVMEDIA_TYPE_AUDIO) {
            decoder->stream_aware = audio_stream_aware;
        } else if (decoder->GetMediaType() == AVMEDIA_TYPE_VIDEO) {
            decoder->stream_aware = video_stream_aware;
        }
        map[decoder->GetStreamIndex()] = decoder;
    }
    SetStatus(DISPATCHER_IDLE);
}

PacketDispatcher::~PacketDispatcher() = default;

void PacketDispatcher::Start() {
    SetStatus(DISPATCHER_START);
    if (dispatcher_thread_tid) {
        return;
    }
    LOGI(TAG, "PacketDispatcher Start");
    pthread_create(&dispatcher_thread_tid, nullptr, DispatchRoutine, this);
}

void PacketDispatcher::Pause() {
    SetStatus(DISPATCHER_PAUSE);
}

void PacketDispatcher::Stop() {
    SetStatus(DISPATCHER_STOP);
}

bool PacketDispatcher::IsDispatching() {
    return GetStatus() == DISPATCHER_START;
}

void PacketDispatcher::Release() {}

void *PacketDispatcher::DispatchRoutine(void *pVoid) {
    auto dispatcher = static_cast<PacketDispatcher *>(pVoid);
    dispatcher->RealDispatch();
    return nullptr;
}

void PacketDispatcher::RealDispatch() {
    while (GetStatus() != DISPATCHER_STOP) {
        while (GetStatus() == DISPATCHER_PAUSE) {
            LOGD(TAG, "wait signal");
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
        }

        AVPacket *av_packet = av_packet_alloc();
        int ret = av_read_frame(av_format_context, av_packet);
        if (ret) {
            LOGE(TAG, "av_read_frame ret=%d", ret);
            break;
        }

        video_stream_aware->Quota();
        auto decoder = map[av_packet->stream_index];
        if (decoder->GetMediaType() == AVMEDIA_TYPE_AUDIO) {
            audio_stream_aware->Quota();
        } else if (decoder->GetMediaType() == AVMEDIA_TYPE_VIDEO) {
            video_stream_aware->Quota();
        }
        decoder->Push(av_packet);
    }
}

PacketDispatcherStatus PacketDispatcher::GetStatus() {
    PacketDispatcherStatus status;
    pthread_mutex_lock(&mutex);
    status = packet_dispatcher_status;
    pthread_mutex_unlock(&mutex);
    return status;
}

void PacketDispatcher::SetStatus(PacketDispatcherStatus status) {
    pthread_mutex_lock(&mutex);
    packet_dispatcher_status = status;
    if (packet_dispatcher_status == DISPATCHER_START) {
        LOGI(TAG, "send signal");
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);
}