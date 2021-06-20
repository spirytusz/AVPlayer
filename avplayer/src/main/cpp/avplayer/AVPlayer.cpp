
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

void *AVPlayer::PrepareBackground(void* pVoid) {
    auto av_player = static_cast<AVPlayer*>(pVoid);

    av_player->RealPrepareBackground();

    return nullptr;
}

void AVPlayer::RealPrepareBackground() {
    // 初始化上下文
    av_format_context = avformat_alloc_context();
    if (!av_format_context) {
        LOGE(TAG, "avformat_alloc_context failed");
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
        LOGE(TAG, "avformat_open_input failed by ret=%d", ret);
        return;
    }

    // 寻找流信息
    ret = avformat_find_stream_info(av_format_context, nullptr);
    if (ret < 0) {
        LOGE(TAG, "avformat_find_stream_info failed by ret=%d", ret);
        return;
    }

    // 初始化音频解码器
    audio_decoder = new AudioDecoder(av_format_context);
    audio_decoder->Init();
    // 初始化视频解码器

    video_decoder = new VideoDecoder(av_format_context);
    video_decoder->Init();
    sendNativeMessage(CODE_PREPARED, "");
}

void AVPlayer::Play() {
    if (audio_decoder) {
        audio_decoder->Start();
    }
    if (video_decoder) {
        video_decoder->Start();
    }
}


void AVPlayer::Pause() {

}

bool AVPlayer::IsPlaying() {
    return true;
}

void AVPlayer::Reset() {}

void AVPlayer::Release() {}