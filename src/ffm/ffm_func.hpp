//
// Created by HP on 2025/6/13.
//

#ifndef FFM_FUNC_HPP
#define FFM_FUNC_HPP
#include "ffm.hpp"
#include "spdlog.h"

inline void func() {
    AVFormatContext *pContext = avformat_alloc_context();
    int ret = avformat_open_input(&pContext, "intro.flac", nullptr, nullptr);
    if (ret < 0) {
        spdlog::error("avformat_open_input error");
    }
    ret = avformat_find_stream_info(pContext, nullptr);
    if (ret < 0) {
        spdlog::error("avformat_find_stream_info error");
    }
    av_dump_format(pContext, 0, "dump.txt", 0);
    int videoStream = -1;
    for (int i = 0; i < pContext->nb_streams; i++) {
        if (pContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            videoStream = i;
        }
    }
    if (videoStream == -1) {
        spdlog::error("avformat_find_stream error");
    }

    AVCodecContext *pCodecContext = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(pCodecContext, pContext->streams[videoStream]->codecpar);
    const AVCodec *pCodec = avcodec_find_decoder(pCodecContext->codec_id);
    if (pCodec == nullptr) {
        spdlog::error("avcodec_find_decoder error");
    }
    avcodec_open2(pCodecContext, pCodec, nullptr);
}
#endif //FFM_FUNC_HPP
