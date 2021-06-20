
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

    const int CODE_PREPARED = 1;

    AVFormatContext *av_format_context = nullptr;

    JavaVM *javaVm = nullptr;
    jobject j_object = nullptr;
    const char *datasource_url;

    VideoDecoder *video_decoder = nullptr;
    AudioDecoder *audio_decoder = nullptr;

    pthread_t prepare_tid;

    static void* PrepareBackground(void* pVoid);

    void RealPrepareBackground();

    void sendNativeMessage(int code, const char *msg) {
        if (!javaVm) {
            return;
        }
        JNIEnv *env;
        int result = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6);
        if (result == JNI_EDETACHED) {
            LOGI(TAG, "JNI_EDETACHED try to AttachCurrentThread");
            result = javaVm->AttachCurrentThread(&env, nullptr);
        }
        if (result != JNI_OK) {
            LOGE(TAG, "sendNativeMessage failed");
            return;
        }
        LOGD(TAG, "call java: sendNativeMessage");
        jclass clazz = env->GetObjectClass(j_object);
        jmethodID method = env->GetMethodID(clazz, "messageBridge", "(ILjava/lang/String;)V");
        jstring j_msg = env->NewStringUTF(msg);
        env->CallVoidMethod(j_object, method, (jint) code, j_msg);
    }

public:
    AVPlayer(JNIEnv *env, jobject object);

    ~AVPlayer();

    void Prepare(const char *url);

    void Play();

    void Pause();

    bool IsPlaying();

    void Reset();

    void Release();
};


#endif //AVPLAYER_AVPLAYER_H
