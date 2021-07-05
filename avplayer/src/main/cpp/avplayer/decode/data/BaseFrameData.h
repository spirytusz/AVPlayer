
#ifndef AVPLAYER_BASEFRAMEDATA_H
#define AVPLAYER_BASEFRAMEDATA_H

#include <malloc.h>

extern "C" {
#include <libavutil/avutil.h>
};

class BaseFrameData {
public:
    int stream_index;
    AVMediaType media_type;
    double pts;

    // 渲染当前帧后标记为已用，下一帧时检查队列，
    // 遇到used==true的时候，delete这一帧
    bool used = false;

    const char *GetPrintMediaType() {
        if (media_type == AVMEDIA_TYPE_VIDEO) {
            return "AVMEDIA_TYPE_VIDEO";
        } else if (media_type == AVMEDIA_TYPE_AUDIO) {
            return "AVMEDIA_TYPE_AUDIO";
        } else {
            return "UNKNOWN";
        }
    }
};

#endif //AVPLAYER_BASEFRAMEDATA_H
