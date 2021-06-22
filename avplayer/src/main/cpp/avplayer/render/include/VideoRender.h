
#ifndef AVPLAYER_VIDEORENDER_H
#define AVPLAYER_VIDEORENDER_H

#include "IRender.h"

class VideoRender: public IRender {
public:
    void Render(AVFrame* av_frame) override;
};


#endif //AVPLAYER_VIDEORENDER_H
