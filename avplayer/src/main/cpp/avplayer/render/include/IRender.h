
#ifndef AVPLAYER_IRENDER_H
#define AVPLAYER_IRENDER_H

class IRender {
public:
    virtual void Render(void* frame_data) = 0;
};

#endif //AVPLAYER_IRENDER_H
