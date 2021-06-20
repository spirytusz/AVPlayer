
#ifndef AVPLAYER_DECODERSTATUS_H
#define AVPLAYER_DECODERSTATUS_H

enum DecodeState {
    IDLE,    // 闲置
    PREPARE, // 准备中
    READY,   // 就绪
    START,   // 开始解码
    STOP,    // 停止解码
    ERROR    // 错误
};

#endif //AVPLAYER_DECODERSTATUS_H
