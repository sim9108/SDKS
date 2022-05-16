// WrapTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include "WarpPerspective.h"

std::vector<glm::vec4> test_list = {};

struct float2 {
    float x, y;
};

std::vector<float2> glm_result{};
std::vector<float2> opencv_result{};

std::vector<float2> src_list = {
    { 0.0f, 0.0f },
    { 100.0f, 0.0f },
    { 100.0f, 100.0f },
    { 0.0f, 100.0f }
};

std::vector<float2> dest_list = {
    { 0.0f, 0.0f },
    { 400.0f, 0.0f },
    { 450.0f, 450.0f },
    { 0.0f, 400.0f }
};


void generator_list() {
    for (int i = 0; i <= 20; ++i) {
        for (int j = 0; j <= 20; ++j) {
            test_list.emplace_back(i*5, j*5, 1.0f, 1.0f); /// !!!! 1.0f(opencv), 1.0f(glm)
        }
    }
}


struct Timer {
    std::string title;
    int cnt = 1;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double, std::milli>> start;

    Timer(const char* t, int cnt = 1) :title{ t }, cnt{ cnt }{
        start = std::chrono::time_point_cast<
            std::chrono::duration<double, std::milli>
        >(std::chrono::high_resolution_clock::now());
    }
    ~Timer() {
        auto end =
            std::chrono::time_point_cast<
            std::chrono::duration<double, std::milli>
            >(std::chrono::high_resolution_clock::now());
        auto diff = end - start;
        auto value = diff.count() / cnt;
        std::cout << "title:" << title << " Time :" << value << "ms FPS:" << (1000.0 / value) << "\n";
    }
};

void glm_version() {
    glm::vec2 src[4];
    for (int i=0; i < 4;++i) {
        src[i] = glm::vec2{ src_list[i].x,  src_list[i].y };
    }
    glm::vec2 dest[4];
    for (int i=0; i < 4; ++i) {
        dest[i] = glm::vec2{ dest_list[i].x,  dest_list[i].y };
    }

    do {
#define LOOP_CNT 1000
        Timer timer("glm", LOOP_CNT);
        for (int i = 0; i < LOOP_CNT; ++i) {
            auto mat = WarpPerspective::getPerspectiveTransform(src, dest);

            for (auto&& item : test_list) {
               auto location = mat * item;
                glm_result.emplace_back(location.x/location.w, location.y/location.w);
            }
        }

    } while (false);
}

void opencv_version() {
    std::vector<cv::Point2f> src;
    for (auto&& item:src_list) {
        src.emplace_back(item.x, item.y );
    }

    std::vector<cv::Point2f> dst;
    for (auto&& item : dest_list) {
        dst.emplace_back(item.x, item.y);
    }

    do {
#define LOOP_CNT 1000
        Timer timer("opencv", LOOP_CNT);
        for (int i = 0; i < LOOP_CNT; ++i) {
            auto mat = cv::getPerspectiveTransform(src, dst);

           std::vector<cv::Point3f> location(1);
            for (auto&& item : test_list) {
                std::vector<cv::Point3f> src = { {item.x,item.y, item.z} };
                cv::transform(src, location, mat);
                location[0].x /= location[0].z;
                location[0].y /= location[0].z;
                opencv_result.emplace_back(location[0].x, location[0].y);
            }
        }
    } while (false);

}

void view_result() {
    int cnt = glm_result.size();
    for (int i = 0; i < cnt; ++i) {
        if ( std::fabs(glm_result[i].x - opencv_result[i].x) > 0.0001 ||
            std::fabs(glm_result[i].y - opencv_result[i].y) > 0.0001
        ) {
            std::cout << glm_result[i].y << "!=" << opencv_result[i].y << '\n';
        }
    }
}

int main()
{
    generator_list();

    glm_version();
    opencv_version();

    view_result();
}

