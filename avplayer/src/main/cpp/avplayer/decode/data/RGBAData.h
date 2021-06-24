
#ifndef AVPLAYER_RGBADATA_H
#define AVPLAYER_RGBADATA_H

#include <cstdint>
extern "C" {
#include <libavutil/rational.h>
};

class RGBAData {
public:
    uint8_t* data;
    int line_size;
    int pts;
    AVRational time_base;

    ~RGBAData() {
        free(data);
    }
};

#endif //AVPLAYER_RGBADATA_H
