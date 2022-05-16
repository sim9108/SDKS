#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>
#include <chrono>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

extern "C" {
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libavutil/cpu_internal.h>
}

struct Timer {
    std::string title;
    int cnt;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double, std::milli>> start;

    Timer(const char* t, int cnt = 1) :title{ t }, cnt{ cnt } {
        start = std::chrono::time_point_cast<
            std::chrono::duration<double, std::milli>
        >(std::chrono::high_resolution_clock::now());
    }
    ~Timer() {
        auto end =
            std::chrono::time_point_cast<
            std::chrono::duration<double, std::milli>
            >(std::chrono::high_resolution_clock::now());
        auto diff = (end - start) / cnt;
        std::cout <<  "title:" << title << " Time :" << diff.count() << "ms FPS:" << 1000.0 / diff.count() << "\n";
    }
};

void test() {
    const long long _Freq = _Query_perf_frequency(); // doesn't change after system boot
}

int main(int argc, char** argv){
    constexpr int alignment = 1;
    
        int src_width = 3840;
        int src_height = 2160; 
        int dst_width = src_width;
        int dst_height = src_height;
        // 1000/120 = 8.3ms under
        auto SRC_FMT = AV_PIX_FMT_BGR32;
                                             // AV_PIX_FMT_YUV444P -> AV_PIX_FMT_YUV420P; 8.7ms 
                                             // AV_PIX_FMT_NV12    -> AV_PIX_FMT_YUV420P; 2.8ms
                                             // AV_PIX_FMT_BGR24   -> AV_PIX_FMT_YUV420P; 12.4ms
                                             // AV_PIX_FMT_BGR32   -> AV_PIX_FMT_YUV420P; 17.0ms
                                             // AV_PIX_FMT_P010    -> AV_PIX_FMT_YUV420P; 13.0ms
                                             // AV_PIX_FMT_P016    -> AV_PIX_FMT_YUV420P; 11.4ms
                                             // AV_PIX_FMT_P210    -> AV_PIX_FMT_YUV420P; 13.3ms
                                             // AV_PIX_FMT_P216    -> AV_PIX_FMT_YUV420P; 10.9ms
                                             // AV_PIX_FMT_YVYU422 -> AV_PIX_FMT_YUV420P; 13.7ms
                                             // AV_PIX_FMT_YUYV422 -> AV_PIX_FMT_YUV420P; 6.4ms
        auto DES_FMT = AV_PIX_FMT_YUV420P;   // AV_PIX_FMT_YUV420P -> AV_PIX_FMT_BGRA; 4ms

        auto rgb32_ctx_ = sws_getContext(src_width, src_height,
            SRC_FMT,
            dst_width, dst_height,
            DES_FMT,
            SWS_POINT, NULL, NULL, NULL); // SWS_POINT 

        uint8_t* srcSlice[AV_NUM_DATA_POINTERS] = {};
        int 	srcStride[AV_NUM_DATA_POINTERS] = {};
        unsigned int src_size{};
        {
            av_image_fill_linesizes(srcStride, SRC_FMT, src_width);
            for (int i = 0; i < AV_NUM_DATA_POINTERS; i++) {
                srcStride[i] = FFALIGN(srcStride[i], alignment);
                if (srcStride[i]) {
                    src_size += srcStride[i] * src_height + alignment;
                    srcSlice[i] = (uint8_t*)av_mallocz(srcStride[i] * src_height + alignment);
                }
            }
        }
        

        uint8_t* destSlice[AV_NUM_DATA_POINTERS] = {};
        int 	destStride[AV_NUM_DATA_POINTERS] = {};
        unsigned int dest_size{};
        {
            av_image_fill_linesizes(destStride, DES_FMT, dst_width);
            for (int i = 0; i < AV_NUM_DATA_POINTERS; i++) {
                destStride[i] = FFALIGN(destStride[i], alignment);
                if (destStride[i]) {
                    dest_size += destStride[i] * dst_height + alignment;
                    destSlice[i] = (uint8_t*)av_mallocz(destStride[i] * dst_height + alignment);
                }
            }
        }        
        
        {  
#define LOOP_CNT 200
           Timer time1{ "test1",LOOP_CNT };
           for (int j = 0; j < LOOP_CNT; ++j) {
               sws_scale(rgb32_ctx_, srcSlice, srcStride, 0, dst_height, destSlice, destStride);
           }
        }
        sws_freeContext(rgb32_ctx_);


        for (int i = 0; i < AV_NUM_DATA_POINTERS; i++) {
            if (srcStride[i])
                av_free(srcSlice[i]);
        }

        for (int i = 0; i < AV_NUM_DATA_POINTERS; i++) {
            if (destStride[i])
                av_free(destSlice[i]);
        }

    return 0;

}
