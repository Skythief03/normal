//
// Created by HP on 2025/6/4.
//

#ifndef FFM_HPP
#define FFM_HPP

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}
#include <memory>
namespace ffm {
    struct FormatContextDeleter {
        void operator()(AVFormatContext* ptr) const {
            if (ptr) avformat_close_input(&ptr);
        }
    };
    using FormatContextPtr = std::unique_ptr<AVFormatContext, FormatContextDeleter>;

    struct CodecContextDeleter {
        void operator()(AVCodecContext* ptr) const {
            avcodec_free_context(&ptr);
        }
    };
    using CodecContextPtr = std::unique_ptr<AVCodecContext, CodecContextDeleter>;

    struct FrameDeleter {
        void operator()(AVFrame* ptr) const {
            av_frame_free(&ptr);
        }
    };
    using FramePtr = std::unique_ptr<AVFrame, FrameDeleter>;

    struct PacketDeleter {
        void operator()(AVPacket* ptr) const {
            av_packet_free(&ptr);
        }
    };
    using PacketPtr = std::unique_ptr<AVPacket, PacketDeleter>;

    struct SwsContextDeleter {
        void operator()(SwsContext* ptr) const {
            sws_freeContext(ptr);
        }
    };
    using SwsContextPtr = std::unique_ptr<SwsContext, SwsContextDeleter>;

}
#endif //FFM_HPP
