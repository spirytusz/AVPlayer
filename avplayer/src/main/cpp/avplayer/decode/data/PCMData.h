
#ifndef AVPLAYER_PCMDATA_H
#define AVPLAYER_PCMDATA_H

#include <cstdint>

class PCMData {
public:
    uint8_t *pcm = nullptr;
    int size = -1;

    ~PCMData() {
        free(pcm);
    }
};

#endif //AVPLAYER_PCMDATA_H
