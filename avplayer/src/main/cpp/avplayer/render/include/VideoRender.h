
#ifndef AVPLAYER_VIDEORENDER_H
#define AVPLAYER_VIDEORENDER_H

#include "IRender.h"

class VideoRender: public IRender {
public:
    void Render(void* frame, int stream_index) override;
};


#endif //AVPLAYER_VIDEORENDER_H
