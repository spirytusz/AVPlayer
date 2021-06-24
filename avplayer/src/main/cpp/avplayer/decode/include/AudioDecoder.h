
#ifndef AVPLAYER_AUDIODECODER_H
#define AVPLAYER_AUDIODECODER_H

#include "BaseDecoder.h"
#include "log.h"
#include <PCMData.h>

extern "C" {
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
};

class AudioDecoder : public BaseDecoder {

public:
    AudioDecoder(AVFormatContext *context): BaseDecoder(context) {}

    const char* LogSpec() override {
        return "AudioDecoder";
    }

    AVMediaType GetMediaType() override {
        return AVMEDIA_TYPE_AUDIO;
    }

    const char* GetPrintMediaType() override {
        return "AVMEDIA_TYPE_AUDIO";
    }

private:
    // 音频转换器
    SwrContext *m_swr = nullptr;

    // 输出缓冲
    uint8_t *m_out_buffer[2] = {nullptr, nullptr};

    // 重采样后，每个通道包含的采样数
    // acc默认为1024，重采样后可能会变化
    int m_dest_nb_sample = 1024;

    // 重采样以后，一帧数据的大小
    int m_dest_data_size = 0;

    void* DecodeFrame(AVFrame* av_frame) override;

    void InitInternal() override;

    void InitSwr();

    void CalculateSampleArgs();

    void InitOutBuffer();
};


#endif //AVPLAYER_AUDIODECODER_H
