// PolygonTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <limits>
#include <vector>
#include <glm/glm.hpp>
#include <chrono>
#include <format>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>


double pointPolygonTest(const std::vector<glm::i64vec2>& contour, const glm::i64vec2& pt) {
    using container_type = std::vector<glm::i64vec2>;
    using container_value_type = container_type::value_type;
    using value_type = container_value_type::value_type;

    auto total = contour.size();
    if (total == 0) return  -1;

    const container_value_type* cnt = contour.data();
    container_value_type v0, v = cnt[total - 1];

    double result = 0;
    int counter = 0;

    for (std::size_t i = 0; i < total; i++){
        v0 = v;
        v = cnt[i];

        if ((v0.y <= pt.y && v.y <= pt.y) ||
            (v0.y > pt.y && v.y > pt.y) ||
            (v0.x < pt.x && v.x < pt.x))
        {
            if (pt.y == v.y && (pt.x == v.x || (pt.y == v0.y &&
                ((v0.x <= pt.x && pt.x <= v.x) || (v.x <= pt.x && pt.x <= v0.x)))))
                return 0;
            continue;
        }

        value_type dist = static_cast<value_type>(pt.y - v0.y) * (v.x - v0.x) - static_cast<value_type>(pt.x - v0.x) * (v.y - v0.y);
        if (dist == 0)
            return 0;
        if (v.y < v0.y)
            dist = -dist;
        counter += dist > 0;
    }

    result = counter % 2 == 0 ? -1 : 1;
    return result;
}

double pointPolygonTest(const std::vector<glm::vec2>& contour, const glm::vec2& pt){
    using container_type = std::vector<glm::vec2>;
    using container_value_type = container_type::value_type;
    using value_type = container_value_type::value_type;

    auto total = contour.size();
    if (total == 0) return  -1;

    const container_value_type* cntf = contour.data();
    container_value_type v0, v = cntf[total - 1];

    double result = 0;
    int counter = 0;

    for (std::size_t i = 0; i < total; i++){
        double dist;
        v0 = v;
        v = cntf[i];

        if ((v0.y <= pt.y && v.y <= pt.y) ||
            (v0.y > pt.y && v.y > pt.y) ||
            (v0.x < pt.x && v.x < pt.x))
        {
            if (pt.y == v.y && (pt.x == v.x || (pt.y == v0.y &&
                ((v0.x <= pt.x && pt.x <= v.x) || (v.x <= pt.x && pt.x <= v0.x)))))
                return 0;
            continue;
        }

        dist = (double)(pt.y - v0.y) * (v.x - v0.x) - (double)(pt.x - v0.x) * (v.y - v0.y);
        if (dist == 0)  return 0;
        if (v.y < v0.y) dist = -dist;
        counter += dist > 0;
    }

    result = counter % 2 == 0 ? -1 : 1;
    return result;
}

struct _pointPolygonTest_measure_mode {} _measure_mode;

double pointPolygonTest(const std::vector<glm::vec2>& contour, const glm::vec2& pt, struct _pointPolygonTest_measure_mode){
    using container_type = std::vector<glm::vec2>;
    using container_value_type = container_type::value_type;
    using value_type = container_value_type::value_type;

    auto total = contour.size();    
    double min_dist_num = std::numeric_limits<double>::max();
    double min_dist_denom = 1;
    if (total == 0) return -std::numeric_limits<value_type>::max();

    const container_value_type* cntf = contour.data();

    container_value_type v0, v= cntf[total - 1];

    double result = 0;
    int counter = 0;
    double dist_num;

    for (std::size_t i = 0; i < total; i++)  {
        v0 = v;
        v = cntf[i];

        double dx = v.x - v0.x;
        double dy = v.y - v0.y;
        double dx1 = pt.x - v0.x; 
        double dy1 = pt.y - v0.y;
        double dx2 = pt.x - v.x; 
        double dy2 = pt.y - v.y;
        double dist_denom = 1;

        if (dx1 * dx + dy1 * dy <= 0)
            dist_num = dx1 * dx1 + dy1 * dy1;
        else if (dx2 * dx + dy2 * dy >= 0)
            dist_num = dx2 * dx2 + dy2 * dy2;
        else
        {
            dist_num = (dy1 * dx - dx1 * dy);
            dist_num *= dist_num;
            dist_denom = dx * dx + dy * dy;
        }

        if (dist_num * min_dist_denom < min_dist_num * dist_denom)
        {
            min_dist_num = dist_num;
            min_dist_denom = dist_denom;
            if (min_dist_num == 0)
                break;
        }

        if ((v0.y <= pt.y && v.y <= pt.y) ||
            (v0.y > pt.y && v.y > pt.y) ||
            (v0.x < pt.x && v.x < pt.x))
            continue;

        dist_num = dy1 * dx - dx1 * dy;
        if (dy < 0)
            dist_num = -dist_num;
        counter += dist_num > 0;
    }

    result = std::sqrt(min_dist_num / min_dist_denom);
    if (counter % 2 == 0)
        result = -result;
    return result;
}

////////////////////////////
struct Timer {
    std::string title;
    std::size_t cnt = 1;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double, std::milli>> start;

    Timer(const char* t, std::size_t cnt = 1) :title{ t }, cnt{ cnt }{
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

std::vector<glm::vec2> polygen_list = {};
std::vector<glm::vec2> test_list = {};
std::vector<double> glm_result = {};
std::vector<double> openv_result = {};

void generator_polygenlist() {
    polygen_list.emplace_back(0, 0);
    polygen_list.emplace_back(100, 0);
    polygen_list.emplace_back(100, 100);
    polygen_list.emplace_back(0, 100);
    polygen_list.emplace_back(0, 50);
    polygen_list.emplace_back(-50, 50);
}

void generator_test_list() {
    for (int i = 0; i <= 1920; ++i) {
        for (int j = 0; j <= 1080; ++j) {
            test_list.emplace_back(i , j );
        }
    }
}

void glm_version() {
    Timer timer("glm", 1);
    for (auto item:test_list) {
        auto result = pointPolygonTest(polygen_list, item, _measure_mode);
        glm_result.push_back(result);
    }
}

void opencv_version() {
    std::vector<cv::Point2f> contour{};
    for (auto item : polygen_list) {
        contour.emplace_back(item.x, item.y);
    }

    std::vector<cv::Point2f> test_data{};
    for (auto item : test_list) {
        test_data.emplace_back(item.x, item.y);
    }

    Timer timer("opencv", 1);
    for (auto item : test_data) {
        auto result = cv::pointPolygonTest(contour, item, true);
        openv_result.push_back(result);
    }
}

void view_result() {
    std::size_t cnt = glm_result.size();
    std::cout << std::format("test cnt:{}\n", cnt);

    for (std::size_t i = 0; i < cnt; ++i) {
        if (std::fabs(glm_result[i]- openv_result[i]) > 0.00000001 ) {
            std::cout << std::format("({}, {})  glm:{}, opencv:{}\n", test_list[i].x, test_list[i].y, glm_result[i] , openv_result[i]);
       }
    }
}

int main(){
    generator_polygenlist();
    generator_test_list();

    glm_version();
    opencv_version();

    view_result();
}