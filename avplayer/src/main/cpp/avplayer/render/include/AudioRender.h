
#ifndef AVPLAYER_AUDIORENDER_H
#define AVPLAYER_AUDIORENDER_H

#include "IRender.h"
#include "log.h"
#include "BaseRender.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <queue>
#include <pthread.h>
#include <PCMData.h>

class AudioRender : public BaseRender {

private:
    const char *TAG = "AudioRender";

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

    void InitChild() override;

    void InitSLEngine();

    bool CheckResult(SLresult result, const char *tag);

    void RealRender() override;

    static void BlockingEnqueue(SLAndroidSimpleBufferQueueItf bq, void *pVoid);

};


#endif //AVPLAYER_AUDIORENDER_H
