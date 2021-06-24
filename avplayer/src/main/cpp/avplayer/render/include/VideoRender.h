
#ifndef AVPLAYER_VIDEORENDER_H
#define AVPLAYER_VIDEORENDER_H

#include "IRender.h"
#include "log.h"

class VideoRender : public IRender {
public:
    void Render(void *frame_data) override;

private:
    const char *TAG = "VideoRender";
};


#endif //AVPLAYER_VIDEORENDER_H
