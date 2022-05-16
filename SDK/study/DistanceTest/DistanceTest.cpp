// DistanceTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <glm/glm.hpp>
#include <chrono>
#include <format>
#include <limits>
#include "WarpPerspective.h"

const glm::vec2 max_distance = { 1000,1000 };

glm::vec2 cal_distance(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) {
  //  float box[4] = { std::min(p1[0], p2[0])-1,std::min(p1[1], p2[1])-1, std::max(p1[0], p2[0])+1,std::max(p1[1], p2[1])+1 };
  //  if (box[0] > p3[0] || box[1] > p3[1] || box[2] < p3[0]|| box[3]< p3[1]) return max_distance;

    float m1 = p1[0] * p2[1] + p2[0] * p3[1] + p3[0] * p1[1];
    float m2 = p2[0] * p1[1] + p3[0] * p2[1] + p1[0] * p3[1];
    float area = m1 - m2;
    glm::vec2 ret{};
    do {
        if (area == 0.0) break;
        glm::vec2 a_vec = p2 - p1;
        glm::vec2 d_vec = { a_vec[1], -a_vec[0] };
        float d_area = a_vec[0] * a_vec[0] + a_vec[1] * a_vec[1];
        float ratio = d_area / area;
        ret = d_vec / ratio;

    } while (false);

    return ret;
}

void distance_tracking(const glm::vec2& p1, const glm::vec2& p2, const uint32_t idx) {
    glm::vec2 a_vec = p2 - p1;
    float r = 2.0f;
    float len = std::sqrt(a_vec[0] * a_vec[0] + a_vec[1] * a_vec[1]);    
    glm::vec2 src[4] = {
        {0.0f,    0.0f},
        {len * 2.0f, 0.0f},
        {len * 2.0f, 2.0f * r * 2.0f},
        {0.0f, 2.0f * r * 2.0f}
    };

    glm::vec2 d_vec_unit = glm::vec2{ a_vec[1], -a_vec[0] }/len;
    

    glm::vec2 dest[4] = {
        p1- d_vec_unit*r, 
        p2 - d_vec_unit * r,
        p2 + d_vec_unit * r, 
        p1 + d_vec_unit * r
    };

    auto mat = WarpPerspective::getPerspectiveTransform(src, dest);

    for (float x = 0; x < len * 2.0f;++x)
    for( float y = 0; y< 2.0f * r * 2.0f; ++y) {
        glm::vec4 item = { x,y, 1.0f, 1.0f };
        auto location = mat * item;
        auto v_len = cal_distance(p1, p2, glm::vec2{ location.x / location.w ,location.y / location.w });
        float len = std::sqrt(v_len[0] * v_len[0] + v_len[1] * v_len[1]);
        std::cout << std::format("{}\n", len);
    }

}

int main(){
    glm::vec2 p1{ 0,0 };
    glm::vec2 p2{ 5,5 };

    for (int i = 0; i < 1; ++i) {
        distance_tracking(p1, p2, 4);
    }
}

