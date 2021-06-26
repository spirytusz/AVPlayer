
#ifndef AVPLAYER_AUDIORENDER_H
#define AVPLAYER_AUDIORENDER_H

#include "IRender.h"
#include "log.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <queue>
#include <pthread.h>
#include <PCMData.h>

class AudioRender : public IRender {

public:
    void Start() override;

    void Render(void *frame_data) override;

    void Stop() override;

private:
    const char *TAG = "AudioRender";

    std::queue<PCMData *> frame_queue;

    pthread_t render_thread_tid = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    // OpenSLES 引擎
    SLObjectItf engineObject;
    // 引擎接口
    SLEngineItf engineInterface;
    // 混音器
    SLObjectItf outputMixObject;
    // 播放器的
    SLObjectItf bqPlayerObject;
    // 播放器接口
    SLPlayItf bqPlayerPlay;
    // 获取播放器队列接口
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;

    void InitSLEngine();

    bool CheckResult(SLresult result, const char *tag);

    static void *RenderRoutine(void *pVoid);

    void RealRender();

    static void BlockingEnqueue(SLAndroidSimpleBufferQueueItf bq, void *pVoid);

};


#endif //AVPLAYER_AUDIORENDER_H
