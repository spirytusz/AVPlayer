
#ifndef AVPLAYER_AVPLAYER_H
#define AVPLAYER_AVPLAYER_H

#include <string>
#include "jni.h"
#include "log.h"
#include <thread>
#include "BaseDecoder.h"
#include "AudioDecoder.h"
#include "VideoDecoder.h"

extern "C" {
#include <libavformat/avformat.h>
};

class AVPlayer {
private:

    const char *TAG = "NativeAVPlayer";

    AVFormatContext *av_format_context = nullptr;

    JavaVM *javaVm = nullptr;
    jobject j_object = nullptr;
    const char* datasource_url;

    VideoDecoder* video_decoder = nullptr;
    AudioDecoder* audio_decoder = nullptr;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    static void PrepareBackground(AVPlayer* that);

public:
    AVPlayer(JNIEnv *env, jobject object);

    ~AVPlayer();

    void Prepare(const char* url);

    void Play();

    void Pause();

    bool IsPlaying();

    void Reset();

    void Release();
};


#endif //AVPLAYER_AVPLAYER_H
