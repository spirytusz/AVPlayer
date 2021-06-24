
#ifndef AVPLAYER_PCMDATA_H
#define AVPLAYER_PCMDATA_H

#include <cstdint>
#include "BaseFrameData.h"

class PCMData: public BaseFrameData {
public:
    uint8_t *pcm = nullptr;
    int size = -1;

    ~PCMData() {
        free(pcm);
    }
};

#endif //AVPLAYER_PCMDATA_H
