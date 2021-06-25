
#ifndef AVPLAYER_VIDEORENDER_H
#define AVPLAYER_VIDEORENDER_H

#include "IRender.h"
#include "log.h"
#include <jni.h>

#include <android/native_window.h>
#include <android/native_window_jni.h>

#include <RGBAData.h>
#include <string>
#include <queue>
#include <pthread.h>

class VideoRender : public IRender {
public:
    void Start() override;

    void Render(void *frame_data) override;

    void Stop() override;

    void SetSurface(JNIEnv *env, jobject g_surface);

    int GetRenderWidth() {
        return window_width;
    }

    int GetRenderHeight() {
        return window_height;
    }

private:
    const char *TAG = "VideoRender";

    ANativeWindow *native_window;

    bool rendering = false;

    pthread_t render_thread_tid = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    std::queue<RGBAData *> frame_queue;

    int window_width = -1;
    int window_height = -1;

    static void *RenderRoutine(void *pVoid);

    void RealRender();
};


#endif //AVPLAYER_VIDEORENDER_H
