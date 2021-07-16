
#include "BaseStreamAware.h"

BaseStreamAware::BaseStreamAware(const char * stream_name) {
    this->m_stream_name = stream_name;
}

BaseStreamAware::~BaseStreamAware() = default;

void BaseStreamAware::OnHeavy() {
    pthread_mutex_lock(&mutex);
    intercept = true;
    pthread_mutex_unlock(&mutex);
}

void BaseStreamAware::OnLight() {
    pthread_mutex_lock(&mutex);
    intercept = false;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void BaseStreamAware::Quota() {
    pthread_mutex_lock(&mutex);
    if (intercept) {
        LOGD("BaseStreamAware", "%s Quota", m_stream_name);
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}
