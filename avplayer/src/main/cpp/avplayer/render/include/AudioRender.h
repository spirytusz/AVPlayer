
#ifndef AVPLAYER_AUDIORENDER_H
#define AVPLAYER_AUDIORENDER_H

#include "IRender.h"
#include "log.h"

class AudioRender: public IRender {

public:
    void Render(void* frame_data) override;

private:
    const char * TAG = "AudioRender";

};


#endif //AVPLAYER_AUDIORENDER_H
