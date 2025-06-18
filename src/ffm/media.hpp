//
// Created by HP on 2025/6/18.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>
#include <vector>

#include "ffm.hpp"
#include "spdlog.h"

class media {
public:
    media() = default;

    ~media() = default;

    /**
     * @brief 打开指定路径下的文件
     * @param file_path
     * @return
     */
    bool open(const std::string &file_path) {
        fp = file_path;
        AVFormatContext *ctx = nullptr;
        auto ret = avformat_open_input(&ctx, file_path.c_str(), nullptr, nullptr);
        p_format_ctx.reset(ctx);
        if (ret < 0) {
            return false;
        }
        ret = avformat_find_stream_info(p_format_ctx.get(), nullptr);
        if (ret < 0) {
            spdlog::error("avformat_find_stream_info failed");
        }
        return true;
    }

    /**
     * @brief 获取文件的所有流信息
     * @return 流信息数组
     */
    [[nodiscard]] std::vector<AVStream *> get_streams() const {
        std::vector<AVStream *> streams;
        for (int i = 0; i < p_format_ctx->nb_streams; i++) {
            streams.push_back(p_format_ctx->streams[i]);
        }
        return streams;
    }

    /**
     * @brief dump文件信息
     */
    void dump() const {
        if (!p_format_ctx) {
            spdlog::error("file open failed before dump");
            return;
        }
        av_dump_format(p_format_ctx.get(), 0, fp.c_str(), 0);
    }

    /**
     * @brief 从流中获取解码器上下文
     * @param stream 要解码的流
     * @return
     */
    ffm::CodecContextPtr get_codec_ctx(const AVStream *stream) const {
        const AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
        if (!codec) {
            spdlog::error("avcodec_find_decoder failed");
            return nullptr;
        }

        AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
        if (!codec_ctx) {
            spdlog::error("avcodec_alloc_context3 failed");
            return nullptr;
        }

        if (avcodec_parameters_to_context(codec_ctx, stream->codecpar) < 0) {
            spdlog::error("avcodec_parameters_to_context failed");
            avcodec_free_context(&codec_ctx);
            return nullptr;
        }

        if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
            spdlog::error("avcodec_open2 failed");
            avcodec_free_context(&codec_ctx);
            return nullptr;
        }
        return ffm::CodecContextPtr(codec_ctx);
    }

private:
    std::string fp;
    ffm::FormatContextPtr p_format_ctx = nullptr;
};
#endif //PLAYER_HPP
