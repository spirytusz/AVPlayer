
#include <RGBAData.h>
#include "include/VideoRender.h"

void VideoRender::Render(void *frame_data) {
    auto rgba_data = static_cast<RGBAData *>(frame_data);
    LOGD(TAG, "data=%ld, line_size=%d, extra_delay=%lf, stream_index=%d, pts=%lld", rgba_data->data,
         rgba_data->line_size, rgba_data->extra_delay, rgba_data->stream_index, rgba_data->pts);
}