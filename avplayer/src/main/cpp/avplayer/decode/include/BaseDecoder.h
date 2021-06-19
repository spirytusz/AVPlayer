
#ifndef AVPLAYER_BASEDECODER_H
#define AVPLAYER_BASEDECODER_H

#include "IDecoder.h"
#include "jni.h"
#include <string>
#include <pthread.h>
#include "log.h"
#include "jni_util.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/time.h>
};

class BaseDecoder : public IDecoder {
private:

    const char* TAG = "BaseDecoder";

    JavaVM *jvm = nullptr;

    jobject g_surface = nullptr;

    AVFormatContext *av_format_ctx = nullptr;

    AVCodec *av_codec = nullptr;

    AVCodecContext *av_codec_ctx = nullptr;

    AVPacket *av_packet = nullptr;

    AVFrame *av_frame = nullptr;

    long current_t_s = -1;

    long duration = -1;

    int stream_index = -1;

    std::string url = nullptr;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    void Init(JNIEnv *env, jstring url);

public:
    BaseDecoder(JNIEnv *env, jstring url);

    void Start() override;

    void Pause() override;

    void Reset() override;

    bool IsDecoding() override;

    long GetCurrentPosition() override;

    long GetDuration() override;

    void setSurface(jobject surface) {
        JNIEnv *env = nullptr;
        int result = GetEnv(jvm, &env);
        if (result == JNI_OK) {
            this->g_surface = env->NewGlobalRef(surface);
        } else {
            LOGE(TAG, "set surface error");
        }
    }

    int Width() {
        return av_codec_ctx->width;
    }

    int Height() {
        return av_codec_ctx->height;
    }
};

#endif //AVPLAYER_BASEDECODER_H
