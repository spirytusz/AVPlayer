
#ifndef AVPLAYER_VIDEORENDER_H
#define AVPLAYER_VIDEORENDER_H

#include "log.h"
#include "BaseRender.h"
#include <jni.h>

#include <android/native_window.h>
#include <android/native_window_jni.h>

#include <RGBAData.h>
#include <string>
#include <queue>
#include <pthread.h>

class VideoRender : public BaseRender {
public:

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

    int window_width = -1;
    int window_height = -1;

    void RealRender() override;

    void InitChild() override;
};


#endif //AVPLAYER_VIDEORENDER_H
