
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

    AVFormatContext *av_format_ctx = nullptr;

    std::queue<AVPacket*> av_packet_queue;

    int stream_index = -1;

    DecodeStatus decoder_status = IDLE;

    pthread_t decode_thread_tid = 0;

    pthread_mutex_t av_packet_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t av_packet_queue_cond = PTHREAD_COND_INITIALIZER;

    pthread_mutex_t decoder_status_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t decoder_status_cond = PTHREAD_COND_INITIALIZER;

    IRender* m_render;

    void FindTargetStream();

    void FindDecoder();

    void InitDecodeThread();

    static void* Decode(void* pVoid);

    void RealDecode();

    virtual void* DecodeFrame(AVFrame* av_frame) = 0;

    void SetDecoderState(DecodeStatus status);

    DecodeStatus GetDecoderStatus();

    void Free();

protected:
    AVCodec *av_codec = nullptr;

    AVCodecContext *av_codec_ctx = nullptr;

    long current_t_ms = -1;

    long duration = -1;

    AVRational GetTimeBase() {
        return av_format_ctx->streams[stream_index]->time_base;
    }

public:
    BaseDecoder(AVFormatContext *context);
    ~BaseDecoder();

    void Init();

    virtual void InitInternal() = 0;

    void Start() override;

    void Pause() override;

    void Push(AVPacket* av_packet) override;

    void SetRender(IRender* render) override;

    bool IsDecoding() override;

    void Release() override;

    long GetCurrentPosition() override;

    long GetDuration() override;

    int GetStreamIndex();

    virtual const char* LogSpec() = 0;
    virtual AVMediaType GetMediaType() = 0;
    virtual const char * GetPrintMediaType() = 0;
};

#endif //AVPLAYER_BASEDECODER_H
