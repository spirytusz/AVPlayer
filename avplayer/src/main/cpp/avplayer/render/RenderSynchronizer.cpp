
#include "include/RenderSynchronizer.h"

bool RenderSynchronizer::Sync(void *frame_data) {
    auto base_frame_data = static_cast<BaseFrameData *>(frame_data);
    if (base_frame_data->media_type == AVMEDIA_TYPE_AUDIO) {
        return ReceiveAudioFrame(static_cast<PCMData *>(frame_data));
    } else if (base_frame_data->media_type == AVMEDIA_TYPE_VIDEO) {
        return ReceiveVideoFrame(static_cast<RGBAData *>(frame_data));
    }
    return false;
}

bool RenderSynchronizer::ReceiveAudioFrame(PCMData *pcm_data) {
    audio_pts = pcm_data->pts;
    return false;
}

bool RenderSynchronizer::ReceiveVideoFrame(RGBAData *rgba_data) {
    video_pts = rgba_data->pts;

    if (audio_pts <= 0 || video_pts <= 0) {
        return false;
    }

    double diff = video_pts - audio_pts;
    if (diff > 0) {
        if (diff > 1) {
            av_usleep((unsigned int) (rgba_data->extra_delay * 1000000.0));
        } else {
            av_usleep((unsigned int) ((diff + rgba_data->extra_delay) * 1000000.0));
        }
        return false;
    } else if (diff < 0) {
        LOGD(TAG, "drop video frame");
        return true;
    } else {
        return false;
    }
}