
#include "include/VideoRender.h"

void VideoRender::SetSurface(JNIEnv *env, jobject g_surface) {
    native_window = ANativeWindow_fromSurface(env, g_surface);

    window_width = ANativeWindow_getWidth(native_window);
    window_height = ANativeWindow_getHeight(native_window);
    LOGD(TAG, "window_width=%d, window_height=%d", window_width, window_height);
}

void VideoRender::RealRender() {
    while (Rendering()) {
        while (!frame_queue.empty() && static_cast<RGBAData *>(frame_queue.front())->used) {
            auto rgba_frame = static_cast<RGBAData *>(frame_queue.front());
            frame_queue.pop();
            delete rgba_frame;
        }
        while (frame_queue.empty()) {
            Lock();
            Wait();
            UnLock();
        }
        if (!Rendering()) {
            break;
        }
        auto *rgba_data = static_cast<RGBAData *>(frame_queue.front());
        frame_queue.pop();

        LOGD(TAG, "m_render_synchronizer=%ld", m_render_synchronizer);
        if (m_render_synchronizer && m_render_synchronizer->Sync(rgba_data)) {
            continue;
        }
        ANativeWindow_setBuffersGeometry(native_window, window_width, window_height,
                                         WINDOW_FORMAT_RGBA_8888);

        ANativeWindow_acquire(native_window);
        ANativeWindow_Buffer m_out_buffer;
        int lock_ret = ANativeWindow_lock(native_window, &m_out_buffer, nullptr);
        if (lock_ret < 0) {
            LOGE(TAG, "ANativeWindow_lock failed by %d", lock_ret);
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

void VideoRender::InitChild() {}