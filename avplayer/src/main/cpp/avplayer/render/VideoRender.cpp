
#include "include/VideoRender.h"

void VideoRender::Render(void *frame_data) {
    pthread_mutex_lock(&mutex);
    auto rgba_data = static_cast<RGBAData *>(frame_data);
    frame_queue.push(rgba_data);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void VideoRender::SetSurface(JNIEnv *env, jobject g_surface) {
    native_window = ANativeWindow_fromSurface(env, g_surface);

    window_width = ANativeWindow_getWidth(native_window);
    window_height = ANativeWindow_getHeight(native_window);
    LOGD(TAG, "window_width=%d, window_height=%d", window_width, window_height);
}

void VideoRender::Start() {
    rendering = true;
    if (render_thread_tid) {
        return;
    }
    pthread_create(&render_thread_tid, nullptr, RenderRoutine, this);
}

void *VideoRender::RenderRoutine(void *pVoid) {
    auto video_render = static_cast<VideoRender *>(pVoid);
    video_render->RealRender();
    return nullptr;
}

void VideoRender::RealRender() {
    while (rendering) {
        while (!frame_queue.empty() && frame_queue.front()->used) {
            auto rgba_frame = frame_queue.front();
            frame_queue.pop();
            delete rgba_frame;
        }
        while (frame_queue.empty()) {
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
        }
        if (!rendering) {
            break;
        }
        RGBAData *rgba_data = frame_queue.front();
        frame_queue.pop();

        ANativeWindow_setBuffersGeometry(native_window, window_width, window_height,
                                         WINDOW_FORMAT_RGBA_8888);

        ANativeWindow_acquire(native_window);
        ANativeWindow_Buffer m_out_buffer;
        int lock_ret = ANativeWindow_lock(native_window, &m_out_buffer, nullptr);
        if (lock_ret < 0) {
            LOGE(TAG, "ANativeWindow_lock failed by %d", lock_ret);
            pthread_mutex_unlock(&mutex);
            return;
        }
        auto *dst_data = static_cast<uint8_t *>(m_out_buffer.bits);

        int dstStride = m_out_buffer.stride * 4;
        int srcStride = rgba_data->line_size;

        for (int h = 0; h < m_out_buffer.height; h++) {
            memcpy(dst_data + h * dstStride, rgba_data->data + h * srcStride, dstStride);
        }
        rgba_data->used = true;
        //释放窗口
        ANativeWindow_unlockAndPost(native_window);
    }
}

void VideoRender::Stop() {
    rendering = false;
}