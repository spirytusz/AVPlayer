
#ifndef AVPLAYER_BASEDECODER_H
#define AVPLAYER_BASEDECODER_H

#include "IDecoder.h"
#include <string>
#include <pthread.h>
#include <thread>
#include <queue>
#include "log.h"
#include "DecoderStatus.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/time.h>
};

class BaseDecoder : public IDecoder {
private:

    const char *TAG = "BaseDecoder";

    AVFormatContext *av_format_ctx = nullptr;

    AVCodec *av_codec = nullptr;

    AVCodecContext *av_codec_ctx = nullptr;

    std::queue<AVPacket*> av_packet_queue;

    int stream_index = -1;

    DecodeState decoder_status = IDLE;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    void FindTargetStream();

    void FindDecoder();

    void InitDecodeThread();

    virtual AVMediaType GetMediaType() = 0;
    virtual char * GetPrintMediaType() = 0;

    static void Decode(BaseDecoder* that);

    void Free();

protected:
    long current_t_ms = -1;

    long duration = -1;

public:
    BaseDecoder(AVFormatContext *context);
    ~BaseDecoder();

    void Init();

    void Start() override;

    void Push(AVPacket* av_packet) override;

    void Release() override;

    long GetCurrentPosition() override;

    long GetDuration() override;
};

#endif //AVPLAYER_BASEDECODER_H