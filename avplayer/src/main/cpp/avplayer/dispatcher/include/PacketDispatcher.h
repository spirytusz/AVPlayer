
#ifndef AVPLAYER_PACKETDISPATCHER_H
#define AVPLAYER_PACKETDISPATCHER_H

#include <vector>
#include <map>
#include <BaseDecoder.h>
#include <log.h>
#include <BaseStreamAware.h>
#include "PacketDispatcherStatus.h"

extern "C" {
#include <libavformat/avformat.h>
};

class PacketDispatcher {

public:
    PacketDispatcher(AVFormatContext *context, const std::vector<BaseDecoder *> &decoders);

    ~PacketDispatcher();

    void Start();

    void Pause();

    void Stop();

    bool IsDispatching();

    void Release();

private:
    const char *TAG = "PacketDispatcher";
    AVFormatContext *av_format_context;
    std::map<int, BaseDecoder *> map;

    PacketDispatcherStatus packet_dispatcher_status;

    pthread_t dispatcher_thread_tid = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    BaseStreamAware* video_stream_aware = nullptr;
    BaseStreamAware* audio_stream_aware = nullptr;

    static void *DispatchRoutine(void *pVoid);

    void RealDispatch();

    PacketDispatcherStatus GetStatus();

    void SetStatus(PacketDispatcherStatus status);
};


#endif //AVPLAYER_PACKETDISPATCHER_H
