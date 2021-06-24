
#ifndef AVPLAYER_BASEFRAMEDATA_H
#define AVPLAYER_BASEFRAMEDATA_H

#include <malloc.h>

class BaseFrameData {
public:
    int stream_index;
    int64_t pts;
};

#endif //AVPLAYER_BASEFRAMEDATA_H
