

#ifndef AVPLAYER_JNI_UTIL_H
#define AVPLAYER_JNI_UTIL_H

#include "log.h"
#include "jni.h"

const char* JNI_UTIL_TAG = "JniUtil";

int GetEnv(JavaVM* jvm, JNIEnv** env) {
    int result = jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    switch (result) {
        case JNI_OK:
            LOGD(JNI_UTIL_TAG, "GetEnv success");
            break;
        case JNI_ERR:
            LOGE(JNI_UTIL_TAG, "GetEnv err by JNI_ERR");
            break;
        case JNI_EDETACHED:
            LOGE(JNI_UTIL_TAG, "GetEnv err by JNI_EDETACHED");
            break;
        case JNI_EVERSION:
            LOGE(JNI_UTIL_TAG, "GetEnv err by JNI_EVERSION");
            break;
        case JNI_ENOMEM:
            LOGE(JNI_UTIL_TAG, "GetEnv err by JNI_ENOMEM");
            break;
        case JNI_EEXIST:
            LOGE(JNI_UTIL_TAG, "GetEnv err by JNI_EEXIST");
            break;
        case JNI_EINVAL:
            LOGE(JNI_UTIL_TAG, "GetEnv err by JNI_EINVAL");
            break;
        default:
            LOGE(JNI_UTIL_TAG, "GetEnv err by unknown error");
            break;
    }
    return result;
}

#endif //AVPLAYER_JNI_UTIL_H