
#ifndef AVPLAYER_RENDERSYNCHRONIZER_H
#define AVPLAYER_RENDERSYNCHRONIZER_H

#include "log.h"
#include <BaseFrameData.h>
#include <PCMData.h>
#include <RGBAData.h>

extern "C" {
#include <libavutil/time.h>
};

class RenderSynchronizer {
public:
    bool Sync(void *frame_data);

private:
    const char *TAG = "RenderSynchronizer";

    int64_t audio_pts = 0;
    int64_t video_pts = 0;

    bool ReceiveAudioFrame(PCMData *pcm_data);

    bool ReceiveVideoFrame(RGBAData *rgba_data);
};


#endif //AVPLAYER_RENDERSYNCHRONIZER_H
