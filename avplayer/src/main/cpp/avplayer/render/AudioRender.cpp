
#include <PCMData.h>
#include "include/AudioRender.h"

void AudioRender::Render(void *frame_data) {
    auto pcm_data = static_cast<PCMData *>(frame_data);
    LOGD(TAG, "pcm=%ld, size=%d, pts=%lld, stream_index=%d", pcm_data->pcm,
         pcm_data->size, pcm_data->pts, pcm_data->stream_index);
}

void AudioRender::Start() {}

void AudioRender::Stop() {}
