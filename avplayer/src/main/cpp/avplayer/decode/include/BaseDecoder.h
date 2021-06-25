
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

    std::queue<AVPacket*> av_packet_queue;

    DecodeStatus decoder_status = IDLE;

    pthread_t decode_thread_tid = 0;

    pthread_mutex_t av_packet_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t av_packet_queue_cond = PTHREAD_COND_INITIALIZER;

    pthread_mutex_t decoder_status_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t decoder_status_cond = PTHREAD_COND_INITIALIZER;

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

    int stream_index = -1;

    AVFormatContext *av_format_ctx = nullptr;

    AVCodec *av_codec = nullptr;

    AVCodecContext *av_codec_ctx = nullptr;

    IRender *m_render;

    long current_t_ms = -1;

    long duration = -1;

    AVRational GetTimeBase() {
        return av_format_ctx->streams[stream_index]->time_base;
    }

    virtual void AfterSetRender(IRender *render) = 0;

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
