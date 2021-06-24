
#ifndef AVPLAYER_RGBADATA_H
#define AVPLAYER_RGBADATA_H

#include <cstdint>
#include "BaseFrameData.h"

extern "C" {
#include <libavutil/rational.h>
};

class RGBAData: public BaseFrameData {
public:
    uint8_t* data;
    int line_size;
    double extra_delay;
    AVRational time_base;

    ~RGBAData() {
        free(data);
    }
};

#endif //AVPLAYER_RGBADATA_H
