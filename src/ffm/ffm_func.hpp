//
// Created by HP on 2025/6/13.
//

#ifndef FFM_FUNC_HPP
#define FFM_FUNC_HPP
#include "media.hpp"

inline void func() {
    media m;
    m.open("NO.2-1.mp4");
    auto streams = m.get_streams();
    AVStream *stream = nullptr;
    for (const auto& s : streams) {
        if (s->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            stream = s;
            break;
        }
    }
    if (!stream) {
        spdlog::error("Could not find a video stream");
    }
    const auto codec_ctx = m.get_codec_ctx(stream);
    if (codec_ctx == nullptr) {
        spdlog::error("Could not find a video codec context");
    }
    m.dump();

    // AVFrame *pFrame = av_frame_alloc();
    // pFrame->format = pCodecContext->pix_fmt;
    // pFrame->width = pCodecContext->width;
    // pFrame->height = pCodecContext->height;
    //
    // if (av_frame_get_buffer(pFrame, 32) < 0) {
    //     spdlog::error("Could not allocate frame buffer");
    // }
    // AVPacket packet;
    // auto *sws = sws_getContext(pCodecContext->width, pCodecContext->height, pCodecContext->pix_fmt,
    //                            pCodecContext->width, pCodecContext->height, AV_PIX_FMT_RGB24,
    //                            SWS_BILINEAR, nullptr, nullptr, nullptr);
    // while (av_read_frame(pContext, &packet) >= 0) {
    //     if (packet.stream_index == videoStream) {
    //         if (avcodec_send_packet(pCodecContext, &packet) == 0) {
    //             while (avcodec_receive_frame(pCodecContext, pFrame) >= 0) {
    //                 AVFrame *dst_frame = av_frame_alloc();
    //                 dst_frame->format = AV_PIX_FMT_RGB24;
    //                 dst_frame->width = pFrame->width;
    //                 dst_frame->height = pFrame->height;
    //                 dst_frame->pts = pFrame->pts;
    //                 if (av_frame_get_buffer(dst_frame, 32) < 0) {
    //                     spdlog::error("av_frame_get_buffer error");
    //                     continue;
    //                 }
    //                 sws_scale(sws, pFrame->data, pFrame->linesize, 0, pFrame->height, dst_frame->data,
    //                           dst_frame->linesize);
    //                 av_frame_free(&dst_frame);
    //                 av_frame_unref(pFrame);
    //             }
    //         }
    //     }
    //     av_packet_unref(&packet);
    // }
    // av_frame_free(&pFrame);
    // sws_freeContext(sws);
    // avcodec_send_packet(pCodecContext, nullptr);
    // avcodec_free_context(&pCodecContext);
    // avformat_close_input(&pContext);
}
#endif //FFM_FUNC_HPP
