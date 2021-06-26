
#ifndef AVPLAYER_BASEFRAMEDATA_H
#define AVPLAYER_BASEFRAMEDATA_H

#include <malloc.h>

class BaseFrameData {
public:
    int stream_index;
    int64_t pts;

    // 渲染当前帧后标记为已用，下一帧时检查队列，
    // 遇到used==true的时候，delete这一帧
    bool used = false;
};

#endif //AVPLAYER_BASEFRAMEDATA_H
