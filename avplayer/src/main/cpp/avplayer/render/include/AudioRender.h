
#ifndef AVPLAYER_AUDIORENDER_H
#define AVPLAYER_AUDIORENDER_H

#include "IRender.h"

class AudioRender: public IRender {

public:
    void Render(AVFrame* av_frame) override;
};


#endif //AVPLAYER_AUDIORENDER_H
