
#include "include/AVPlayer.h"

AVPlayer::AVPlayer(JNIEnv *env, jobject object) {
    if (!env) {
        LOGE(TAG, "illegal JNIEnv");
        return;
    }
    env->GetJavaVM(&javaVm);
    this->j_object = env->NewGlobalRef(object);
}

AVPlayer::~AVPlayer() {
    JNIEnv *env = nullptr;
    javaVm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (env) {
        env->DeleteGlobalRef(this->j_object);
    }
}

void AVPlayer::Prepare(const char *url) {
    LOGE(TAG, "Prepare %s", url);
    this->datasource_url = url;
    pthread_create(&prepare_tid, nullptr, PrepareBackground, this);
}

void AVPlayer::SetSurfaceView(jobject g_surface) {
    video_render = new VideoRender();
    SetSurfaceToRender(g_surface);
    if (video_decoder && video_render) {
        video_decoder->SetRender(video_render);
    }
}

void *AVPlayer::PrepareBackground(void *pVoid) {
    auto av_player = static_cast<AVPlayer *>(pVoid);

    av_player->RealPrepareBackground();

    return nullptr;
}

void AVPlayer::RealPrepareBackground() {
    // 初始化上下文
    av_format_context = avformat_alloc_context();
    if (!av_format_context) {
        LOGE(TAG, "avformat_alloc_context failed");
        sendNativeMessage(PREPARED_ERROR, "avformat_alloc_context failed");
        return;
    }

    // 设置参数
    AVDictionary *dictionary = nullptr;
    av_dict_set(&dictionary, "timeout", "5000000", 0);

    // 打开媒体源
    int ret = avformat_open_input(
            &av_format_context,
            datasource_url,
            nullptr,
            &dictionary
    );
    av_dict_free(&dictionary);
    if (ret) {
        sendNativeMessage(PREPARED_ERROR, "avformat_alloc_context failed");
        LOGE(TAG, "avformat_open_input failed by ret=%d", ret);
        return;
    }

    // 寻找流信息
    ret = avformat_find_stream_info(av_format_context, nullptr);
    if (ret < 0) {
        sendNativeMessage(PREPARED_ERROR, "avformat_find_stream_info failed");
        LOGE(TAG, "avformat_find_stream_info failed by ret=%d", ret);
        return;
    }

    // 初始化音频解码器
    audio_decoder = new AudioDecoder(av_format_context);
    audio_render = new AudioRender();
    if (audio_render && audio_decoder) {
        audio_decoder->SetRender(audio_render);
    }
    audio_decoder->Init();

    // 初始化视频解码器
    video_decoder = new VideoDecoder(av_format_context);
    video_decoder->Init();

    // 初始化封装数据分发器
    std::vector<BaseDecoder *> decoders;
    decoders.push_back(audio_decoder);
    decoders.push_back(video_decoder);
    packet_dispatcher = new PacketDispatcher(av_format_context, decoders);

    if (!audio_decoder) {
        sendNativeMessage(PREPARED_ERROR, "can not create audio_decoder");
        return;
    }

    if (!video_decoder) {
        sendNativeMessage(PREPARED_ERROR, "can not create video_decoder");
        return;
    }

    if (!packet_dispatcher) {
        sendNativeMessage(PREPARED_ERROR, "can not create packet_dispatcher");
        return;
    }

    sendNativeMessage(CODE_PREPARED, "");
}

void AVPlayer::Play() {
    if (audio_decoder) {
        audio_decoder->Start();
    }
    if (audio_render) {
        audio_render->Start();
    }
    if (video_decoder) {
        video_decoder->Start();
    }
    if (video_render) {
        video_render->Start();
    }
    if (packet_dispatcher) {
        packet_dispatcher->Start();
    }
}


void AVPlayer::Pause() {
    if (audio_decoder) {
        audio_decoder->Pause();
    }
    if (video_decoder) {
        video_decoder->Pause();
    }
    if (packet_dispatcher) {
        packet_dispatcher->Pause();
    }
}

bool AVPlayer::IsPlaying() {
    if (!audio_decoder || !audio_decoder->IsDecoding()) {
        LOGD(TAG, "audio_decoder not decoding");
        return false;
    }
    if (!video_decoder || !video_decoder->IsDecoding()) {
        LOGD(TAG, "video_decoder not decoding");
        return false;
    }
    if (!packet_dispatcher || !packet_dispatcher->IsDispatching()) {
        LOGD(TAG, "packet_dispatcher not dispatching");
        return false;
    }
    return true;
}

void AVPlayer::Reset() {}

void AVPlayer::Release() {}

bool AVPlayer::SetSurfaceToRender(jobject surface) {
    if (!surface) {
        return false;
    }

    JNIEnv *env = nullptr;
    bool try_attached = false;
    int ret = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6);
    LOGD(TAG, "GetEnv %d", ret);
    if (ret == JNI_EDETACHED) {
        ret = javaVm->AttachCurrentThread(&env, nullptr);
        try_attached = true;
    }
    if (ret != JNI_OK) {
        LOGE(TAG, "can not attach to current thread! %d", ret);
        sendNativeMessage(PREPARED_ERROR, "can not attach to current thread!");
        return false;
    }

    if (video_render) {
        video_render->SetSurface(env, surface);
    }
    if (try_attached) {
        javaVm->DetachCurrentThread();
    }
    return true;
}