#include <jni.h>
#include <string>
#include <AVPlayer.h>

extern "C" {
#include <libavutil/avutil.h>
}

jstring getFFmpegVersion(JNIEnv *env, jclass clazz) {
    std::string hello = av_version_info();
    return env->NewStringUTF(hello.c_str());
}

jlong init(JNIEnv *env, jobject object) {
    return (jlong) new AVPlayer();
}

void prepare(JNIEnv *env, jobject object, jlong ptr, jstring uri) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    av_player->prepare("");
}

void play(JNIEnv *env, jobject object, jlong ptr) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    av_player->play();
}

void pause(JNIEnv *env, jobject object, jlong ptr) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    av_player->pause();
}

void reset(JNIEnv *env, jobject object, jlong ptr) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    av_player->reset();
}

void release(JNIEnv *env, jobject object, jlong ptr) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    av_player->release();
    delete av_player;
}

jboolean isPlaying(JNIEnv *env, jobject object, jlong ptr) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    return av_player->isPlaying();
}

static JNINativeMethod gMethods[] = {
        {"getFFmpegVersion", "()Ljava/lang/String;",  (void *) getFFmpegVersion},
        {"nativeInit",              "()J",                    (void *) init},
        {"nativePrepare",           "(JLjava/lang/String;)V", (void *) prepare},
        {"nativePlay",              "(J)V",                   (void *) play},
        {"nativePause",             "(J)V",                   (void *) pause},
        {"nativeIsPlaying",         "(J)Z",                   (void *) isPlaying},
        {"nativeReset",             "(J)V",                   (void *) reset},
        {"nativeRelease",           "(J)V",                   (void *) release}
};

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *javaVm, void *pVoid) {
    JNIEnv *jniEnv = nullptr;
    jint result = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);
    if (result != JNI_OK) {
        return JNI_ERR;
    }
    jclass avplayer_clazz = jniEnv->FindClass("com/spirytusz/lib/avplayer/AVPlayer");
    jniEnv->RegisterNatives(avplayer_clazz, gMethods,
                            sizeof(gMethods) / sizeof(JNINativeMethod));

    return JNI_VERSION_1_6;
}