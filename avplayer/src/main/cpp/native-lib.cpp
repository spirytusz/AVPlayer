#include <jni.h>
#include <string>
#include <AVPlayer.h>
#include <thread>

extern "C" {
#include <libavutil/avutil.h>
}

jstring getFFmpegVersion(JNIEnv *env, jclass clazz) {
    std::string hello = av_version_info();
    return env->NewStringUTF(hello.c_str());
}

jlong init(JNIEnv *env, jobject object) {
    return (jlong) new AVPlayer(env, object);
}

void prepare(JNIEnv *env, jobject object, jlong ptr, jstring uri) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    av_player->Prepare(env->GetStringUTFChars(uri, nullptr));
}

void setSurface(JNIEnv *env, jobject object, jlong ptr, jobject surface) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    jobject g_surface = env->NewGlobalRef(surface);
    av_player->SetSurfaceView(g_surface);
}

void play(JNIEnv *env, jobject object, jlong ptr) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    av_player->Play();
}

void pause(JNIEnv *env, jobject object, jlong ptr) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    av_player->Pause();
}

void reset(JNIEnv *env, jobject object, jlong ptr) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    av_player->Reset();
}

void release(JNIEnv *env, jobject object, jlong ptr) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    av_player->Release();
    delete av_player;
}

jboolean isPlaying(JNIEnv *env, jobject object, jlong ptr) {
    auto av_player = static_cast<AVPlayer *>((void *) ptr);
    return av_player->IsPlaying();
}

static JNINativeMethod gMethods[] = {
        {"getFFmpegVersion", "()Ljava/lang/String;",       (void *) getFFmpegVersion},
        {"nativeInit",       "()J",                        (void *) init},
        {"nativePrepare",    "(JLjava/lang/String;)V",     (void *) prepare},
        {"nativeSetSurface", "(JLandroid/view/Surface;)V", (void *) setSurface},
        {"nativePlay",       "(J)V",                       (void *) play},
        {"nativePause",      "(J)V",                       (void *) pause},
        {"nativeIsPlaying",  "(J)Z",                       (void *) isPlaying},
        {"nativeReset",      "(J)V",                       (void *) reset},
        {"nativeRelease",    "(J)V",                       (void *) release}
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