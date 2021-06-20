

#ifndef AVPLAYER_JNI_UTIL_H
#define AVPLAYER_JNI_UTIL_H

#include "log.h"
#include "jni.h"

class JniUtil {

public:
    static int GetEnv(JavaVM* jvm, JNIEnv** env) {
        int result = jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
        switch (result) {
            case JNI_OK:
                LOGD("JniUtil", "GetEnv success");
                break;
            case JNI_ERR:
                LOGE("JniUtil", "GetEnv err by JNI_ERR");
                break;
            case JNI_EDETACHED:
                LOGE("JniUtil", "GetEnv err by JNI_EDETACHED");
                break;
            case JNI_EVERSION:
                LOGE("JniUtil", "GetEnv err by JNI_EVERSION");
                break;
            case JNI_ENOMEM:
                LOGE("JniUtil", "GetEnv err by JNI_ENOMEM");
                break;
            case JNI_EEXIST:
                LOGE("JniUtil", "GetEnv err by JNI_EEXIST");
                break;
            case JNI_EINVAL:
                LOGE("JniUtil", "GetEnv err by JNI_EINVAL");
                break;
            default:
                LOGE("JniUtil", "GetEnv err by unknown error");
                break;
        }
        return result;
    }
};

#endif //AVPLAYER_JNI_UTIL_H