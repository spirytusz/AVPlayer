
#ifndef AVPLAYER_BASESTREAMAWARE_H
#define AVPLAYER_BASESTREAMAWARE_H

#include "IStreamAware.h"
#include <pthread.h>
#include <log.h>

class BaseStreamAware : public IStreamAware {
private:
    const char * m_stream_name = nullptr;
    bool intercept = false;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

public:
    /**
     * 生产者把数据推入消费者前，先执行这个方法
     */

    BaseStreamAware(const char * stream_name);
    ~BaseStreamAware();
    void Quota();

    void OnHeavy() override;

    void OnLight() override;

};


#endif //AVPLAYER_BASESTREAMAWARE_H
