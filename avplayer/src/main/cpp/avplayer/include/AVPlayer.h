
#ifndef AVPLAYER_AVPLAYER_H
#define AVPLAYER_AVPLAYER_H

#include <string>

class AVPlayer {
public:
    void prepare(std::string url);

    void play();

    void pause();

    bool isPlaying();

    void reset();

    void release();
};


#endif //AVPLAYER_AVPLAYER_H
