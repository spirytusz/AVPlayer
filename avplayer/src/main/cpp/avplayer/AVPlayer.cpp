
#include "include/AVPlayer.h"

#include <utility>

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
    JniUtil::GetEnv(javaVm, &env);
    if (env) {
        env->DeleteGlobalRef(this->j_object);
    }
}

void AVPlayer::Prepare(const char* url) {
    LOGE(TAG, "Prepare %s", url);
    this->datasource_url = url;

    std::thread t(PrepareBackground, this);
    t.detach();
    LOGE(TAG, "Prepare detach");
}

void AVPlayer::PrepareBackground(AVPlayer* that) {
    // 初始化上下文
    that->av_format_context = avformat_alloc_context();
    if (!that->av_format_context) {
        LOGE(that->TAG, "avformat_alloc_context failed");
        return;
    }

    // 设置参数
    AVDictionary *dictionary = nullptr;
    av_dict_set(&dictionary, "timeout", "5000000", 0);

    // 打开媒体源
    int ret = avformat_open_input(
            &that->av_format_context,
            that->datasource_url,
            nullptr,
            &dictionary
    );
    av_dict_free(&dictionary);
    if (ret) {
        LOGE(that->TAG, "avformat_open_input failed by ret=%d", ret);
        return;
    }

    // 寻找流信息
    ret = avformat_find_stream_info(that->av_format_context, nullptr);
    if (ret < 0) {
        LOGE(that->TAG, "avformat_find_stream_info failed by ret=%d", ret);
        return;
    }

    // 初始化音频解码器
    that->audio_decoder = new AudioDecoder(that->av_format_context);
    that->audio_decoder->Init();
    // 初始化视频解码器

    that->video_decoder = new VideoDecoder(that->av_format_context);
    that->video_decoder->Init();
}

void AVPlayer::Play() {}


void AVPlayer::Pause() {

}

bool AVPlayer::IsPlaying() {
    return true;
}

void AVPlayer::Reset() {}

void AVPlayer::Release() {}