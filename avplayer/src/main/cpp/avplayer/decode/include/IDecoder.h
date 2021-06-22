
#ifndef AVPLAYER_IDECODER_H
#define AVPLAYER_IDECODER_H

#include <IRender.h>

extern "C" {
#include <libavcodec/packet.h>
};

class IDecoder {
public:
    /**
     * 开始解码
     */
    virtual void Start() = 0;

    /**
     * 暂停解码
     */
    virtual void Pause() = 0;

    /**
     * 释放解码器
     */
    virtual void Release() = 0;

    /**
     * 推入封装数据
     * @param av_packet 封装数据
     */
    virtual void Push(AVPacket* av_packet) = 0;

    virtual void SetRender(IRender* render) = 0;

    /**
     * 获取pts
     */
    virtual long GetCurrentPosition() = 0;

    /**
     * 获取媒体长度
     */
    virtual long GetDuration() = 0;
};

#endif //AVPLAYER_IDECODER_H
