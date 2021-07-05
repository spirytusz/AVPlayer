
#ifndef AVPLAYER_BASERENDER_H
#define AVPLAYER_BASERENDER_H


#include <queue>
#include <pthread.h>
#include "IRender.h"
#include "RenderSynchronizer.h"

class BaseRender : public IRender {
public:
    void Start() override;

    void Render(void *frame_data) override;

    void Stop() override;

    bool Rendering() {
        return rendering;
    }

    void SetSynchronizer(RenderSynchronizer *render_synchronizer) {
        m_render_synchronizer = render_synchronizer;
    }

protected:
    std::queue<void *> frame_queue;

    RenderSynchronizer *m_render_synchronizer;

    void Lock() {
        pthread_mutex_lock(&mutex);
    }

    void UnLock() {
        pthread_mutex_unlock(&mutex);
    }

    void Wait() {
        pthread_cond_wait(&cond, &mutex);
    }

    void Signal() {
        pthread_cond_signal(&cond);
    }

private:

    pthread_t render_thread_tid = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    bool rendering = false;

    virtual void InitChild() = 0;

    static void *RenderRoutine(void *pVoid);

    virtual void RealRender() = 0;
};


#endif //AVPLAYER_BASERENDER_H
