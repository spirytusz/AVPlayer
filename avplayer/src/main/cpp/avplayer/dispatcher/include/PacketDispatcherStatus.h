
#ifndef AVPLAYER_PACKETDISPATCHERSTATUS_H
#define AVPLAYER_PACKETDISPATCHERSTATUS_H

enum PacketDispatcherStatus {
    DISPATCHER_IDLE,  // 闲置
    DISPATCHER_START, // 开始分发
    DISPATCHER_PAUSE, // 暂停分发
    DISPATCHER_STOP,  // 停止分发
};

#endif //AVPLAYER_PACKETDISPATCHERSTATUS_H
