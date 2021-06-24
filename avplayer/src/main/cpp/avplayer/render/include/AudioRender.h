
#ifndef AVPLAYER_AUDIORENDER_H
#define AVPLAYER_AUDIORENDER_H

#include "IRender.h"

class AudioRender: public IRender {

public:
    void Render(void* frame, int stream_index) override;

};


#endif //AVPLAYER_AUDIORENDER_H
