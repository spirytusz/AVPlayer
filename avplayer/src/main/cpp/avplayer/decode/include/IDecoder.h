
#ifndef AVPLAYER_IDECODER_H
#define AVPLAYER_IDECODER_H

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
     * 重置解码器
     */
    virtual void Reset() = 0;

    /**
     * 是否解码中
     */
    virtual bool IsDecoding() = 0;

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
