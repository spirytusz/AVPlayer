
#ifndef AVPLAYER_IRENDER_H
#define AVPLAYER_IRENDER_H

extern "C" {
#include "libavutil/frame.h"
};

class IRender {
public:
    virtual void Render(AVFrame *av_frame) = 0;
};

#endif //AVPLAYER_IRENDER_H
