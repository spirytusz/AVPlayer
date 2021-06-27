
#include "include/BaseRender.h"

void BaseRender::Start() {
    if (render_thread_tid) {
        return;
    }
    rendering = true;
    InitChild();
    pthread_create(&render_thread_tid, nullptr, RenderRoutine, this);
}

void BaseRender::Render(void *frame_data) {
    Lock();
    frame_queue.push(frame_data);
    Signal();
    UnLock();
}

void *BaseRender::RenderRoutine(void *pVoid) {
    auto base_render = static_cast<BaseRender *>(pVoid);
    base_render->RealRender();
    return nullptr;
}

void BaseRender::Stop() {
    Lock();
    rendering = false;
    UnLock();
}