
#ifndef AVPLAYER_IRENDER_H
#define AVPLAYER_IRENDER_H

class IRender {
public:
    virtual void Start() = 0;

    virtual void Render(void *frame_data) = 0;

    virtual void Stop() = 0;
};

#endif //AVPLAYER_IRENDER_H
