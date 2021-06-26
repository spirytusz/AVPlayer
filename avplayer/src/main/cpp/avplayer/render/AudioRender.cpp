
#include "include/AudioRender.h"

void AudioRender::Render(void *frame_data) {
    pthread_mutex_lock(&mutex);
    auto pcm_data = static_cast<PCMData *>(frame_data);
    frame_queue.push(pcm_data);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void AudioRender::Start() {
    if (render_thread_tid) {
        return;
    }
    pthread_create(&render_thread_tid, nullptr, RenderRoutine, this);
}

void AudioRender::Stop() {}

void AudioRender::InitSLEngine() {
    SLresult result = slCreateEngine(
            &engineObject,
            0,
            nullptr,
            0,
            nullptr,
            nullptr
    );
    if (!CheckResult(result, "slCreateEngine engineObject")) {
        return;
    }

    // 初始化引擎
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (!CheckResult(result, "Realize engineObject")) {
        return;
    }

    // 获取引擎接口
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineInterface);
    if (!CheckResult(result, "GetInterface engineInterface")) {
        return;
    }

    // 创建混音器
    result = (*engineInterface)->CreateOutputMix(engineInterface, &outputMixObject, 0, nullptr,
                                                 nullptr);
    if (!CheckResult(result, "CreateOutputMix")) {
        return;
    }

    // 初始化混音器
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if (!CheckResult(result, "Realize outputMixObject")) {
        return;
    }

    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
            2
    };

    // pcm数据格式
    // SL_DATAFORMAT_PCM：数据格式为pcm格式
    // 2：双声道
    // SL_SAMPLINGRATE_44_1：采样率为44100（44.1赫兹 应用最广的，兼容性最好的）
    // SL_PCMSAMPLEFORMAT_FIXED_16：采样格式为16bit （16位）(2个字节)
    // SL_PCMSAMPLEFORMAT_FIXED_16：数据大小为16bit （16位）（2个字节）
    // SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT：左右声道（双声道）  （双声道 立体声的效果）
    // SL_BYTEORDER_LITTLEENDIAN：小端模式
    SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };

    // 数据源 将上述配置信息放到这个数据源中
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // 设置混音器
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, nullptr};

    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};

    // 创建播放器
    result = (*engineInterface)->CreateAudioPlayer(
            engineInterface,
            &bqPlayerObject,
            &audioSrc,
            &audioSnk,
            1,
            ids,
            req
    );
    if (!CheckResult(result, "CreateAudioPlayer")) {
        return;
    }

    // 初始化播放器
    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
    if (!CheckResult(result, "Realize bqPlayerObject")) {
        return;
    }

    // 获取播放器接口
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    if (!CheckResult(result, "GetInterface bqPlayerObject")) {
        return;
    }

    (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue);

    // 注册一个回调: 当SL的缓冲播放完成时，会回调该函数
    (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, BlockingEnqueue, this);

    (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
}

bool AudioRender::CheckResult(SLresult result, const char *tag) {
    if (SL_RESULT_SUCCESS != result) {
        LOGE(TAG, "%s failed %ud", tag, result);
        return false;
    }
    return true;
}

void *AudioRender::RenderRoutine(void *pVoid) {
    auto audio_render = static_cast<AudioRender *>(pVoid);
    audio_render->InitSLEngine();
    audio_render->RealRender();
    return nullptr;
}

void AudioRender::RealRender() {
    BlockingEnqueue(bqPlayerBufferQueue, this);
}

void AudioRender::BlockingEnqueue(SLAndroidSimpleBufferQueueItf bq, void *pVoid) {
    auto audio_render = static_cast<AudioRender *>(pVoid);

    while (!audio_render->frame_queue.empty() && audio_render->frame_queue.front()->used) {
        auto pcm_data = audio_render->frame_queue.front();
        delete pcm_data;
        audio_render->frame_queue.pop();
    }

    while (audio_render->frame_queue.empty()) {
        pthread_mutex_lock(&audio_render->mutex);
        pthread_cond_wait(&audio_render->cond, &audio_render->mutex);
        pthread_mutex_unlock(&audio_render->mutex);
    }

    PCMData *pcm_data = audio_render->frame_queue.front();
    audio_render->frame_queue.pop();
    if (pcm_data->pcm && pcm_data->size) {
        SLresult result = (*bq)->Enqueue(bq, pcm_data->pcm, pcm_data->size);
        if (result == SL_RESULT_SUCCESS) {
            pcm_data->used = true;
        }
    }
}