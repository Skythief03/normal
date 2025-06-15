//
// Created by HP on 2025/6/13.
//

#ifndef FFM_FUNC_HPP
#define FFM_FUNC_HPP
#include "ffm.hpp"
#include "spdlog.h"
#include <fstream>

inline void save_file(AVFrame *pFrame) {
    const std::string filename = std::format("frame{}.ppm", pFrame->pts);
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile.is_open()) {
        spdlog::error("Could not open file {}", filename);
    }
    outFile << "P6\n" << pFrame->width << " " << pFrame->height << "\n255\n";
    // 写入像素数据（每行 width * 3 字节）
    for (int y = 0; y < pFrame->height; ++y) {
        outFile.write(reinterpret_cast<const char *>(pFrame->data[0] + y * pFrame->linesize[0]), pFrame->width * 3);
    }
    outFile.close();
}

inline void func() {
    AVFormatContext *pContext = avformat_alloc_context();
    int ret = avformat_open_input(&pContext, "NO.2-1.mp4", nullptr, nullptr);
    if (ret < 0) {
        spdlog::error("avformat_open_input error");
    }
    ret = avformat_find_stream_info(pContext, nullptr);
    if (ret < 0) {
        spdlog::error("avformat_find_stream_info error");
    }
    av_dump_format(pContext, 0, "NO.2-1.mp4", 0);
    int videoStream = -1;
    for (int i = 0; i < pContext->nb_streams; i++) {
        if (pContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
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
    AVFrame *pFrame = av_frame_alloc();
    pFrame->format = pCodecContext->pix_fmt;
    pFrame->width = pCodecContext->width;
    pFrame->height = pCodecContext->height;

    if (av_frame_get_buffer(pFrame, 32) < 0) {
        spdlog::error("Could not allocate frame buffer");
    }
    AVPacket packet;
    auto *sws = sws_getContext(pCodecContext->width, pCodecContext->height, pCodecContext->pix_fmt,
                               pCodecContext->width, pCodecContext->height, AV_PIX_FMT_RGB24,
                               SWS_BILINEAR, nullptr, nullptr, nullptr);
    while (av_read_frame(pContext, &packet) >= 0) {
        if (packet.stream_index == videoStream) {
            if (avcodec_send_packet(pCodecContext, &packet) == 0) {
                while (avcodec_receive_frame(pCodecContext, pFrame) >= 0) {
                    AVFrame *dst_frame = av_frame_alloc();
                    dst_frame->format = AV_PIX_FMT_RGB24;
                    dst_frame->width = pFrame->width;
                    dst_frame->height = pFrame->height;
                    dst_frame->pts = pFrame->pts;
                    if (av_frame_get_buffer(dst_frame, 32) < 0) {
                        spdlog::error("av_frame_get_buffer error");
                        continue;
                    }
                    sws_scale(sws, pFrame->data, pFrame->linesize, 0, pFrame->height, dst_frame->data,
                              dst_frame->linesize);
                    if (pFrame->pts < 10000)
                        save_file(dst_frame);
                    av_frame_free(&dst_frame);
                    av_frame_unref(pFrame);
                }
            }
        }
        av_packet_unref(&packet);
    }
    av_frame_free(&pFrame);
    sws_freeContext(sws);
    avcodec_send_packet(pCodecContext, nullptr);
    avcodec_free_context(&pCodecContext);
    avformat_close_input(&pContext);
}
#endif //FFM_FUNC_HPP
