
#ifndef AVPLAYER_LOG_H
#define AVPLAYER_LOG_H

#define LOGV(tag, fmt, ...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, tag, "%s: " fmt, __FUNCTION__, ## __VA_ARGS__))
#define LOGD(tag, fmt, ...) ((void)__android_log_print(ANDROID_LOG_DEBUG, tag, "%s: " fmt, __FUNCTION__, ## __VA_ARGS__))
#define LOGI(tag, fmt, ...) ((void)__android_log_print(ANDROID_LOG_INFO, tag, "%s: " fmt, __FUNCTION__, ## __VA_ARGS__))
#define LOGW(tag, fmt, ...) ((void)__android_log_print(ANDROID_LOG_WARN, tag, "%s: " fmt, __FUNCTION__, ## __VA_ARGS__))
#define LOGE(tag, fmt, ...) ((void)__android_log_print(ANDROID_LOG_ERROR, tag, "%s: " fmt, __FUNCTION__, ## __VA_ARGS__))

#include <android/log.h>

#endif //AVPLAYER_LOG_H
