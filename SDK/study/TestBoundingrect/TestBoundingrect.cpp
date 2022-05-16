#include <iostream>
#include <limits>
#include <vector>
#include <glm/glm.hpp>
#include <chrono>
#include <format>

#include <opencv2/core/hal/intrin.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

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

#define  CV_TOGGLE_FLT(x) ((x)^((int)(x) < 0 ? 0x7fffffff : 0))

glm::i32vec4 pointSetBoundingRect(const std::vector<glm::vec2>& points) {
    using container_type = std::vector<glm::vec2>;
    using container_value_type = container_type::value_type;
    using value_type = container_value_type::value_type;

    auto npoints = points.size();
    if (npoints == 0)
        return glm::i32vec4{};

    int  xmin = 0, ymin = 0, xmax = -1, ymax = -1, i;

    using namespace cv;
#if CV_SIMD
    const int64_t* pts = (int64_t*)points.data();

    v_float32 minval, maxval;
    minval = maxval = v_reinterpret_as_f32(vx_setall_s64(*pts)); //min[0]=pt.x, min[1]=pt.y, min[2]=pt.x, min[3]=pt.y
    for (i = 1; i <= npoints - v_float32::nlanes / 2; i += v_float32::nlanes / 2)
    {
        v_float32 ptXY2 = v_reinterpret_as_f32(vx_load(pts + i));
        minval = v_min(ptXY2, minval);
        maxval = v_max(ptXY2, maxval);
    }
    minval = v_min(v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(minval))), v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(minval))));
    maxval = v_max(v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(maxval))), v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(maxval))));
    if (i <= npoints - v_float32::nlanes / 4)
    {
        v_float32 ptXY = v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(vx_load_low(pts + i))));
        minval = v_min(ptXY, minval);
        maxval = v_max(ptXY, maxval);
        i += v_float32::nlanes / 4;
    }
    for (int j = 16; j < CV_SIMD_WIDTH; j *= 2)
    {
        minval = v_min(v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(minval))), v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(minval))));
        maxval = v_max(v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(maxval))), v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(maxval))));
    }
    xmin = cvFloor(minval.get0());
    xmax = cvFloor(maxval.get0());
    ymin = cvFloor(v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(minval))).get0());
    ymax = cvFloor(v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(maxval))).get0());
#if CV_SIMD_WIDTH > 16
    if (i < npoints)
    {
        v_float32x4 minval2, maxval2;
        minval2 = maxval2 = v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(v_load_low(pts + i))));
        for (i++; i < npoints; i++)
        {
            v_float32x4 ptXY = v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(v_load_low(pts + i))));
            minval2 = v_min(ptXY, minval2);
            maxval2 = v_max(ptXY, maxval2);
        }
        xmin = min(xmin, cvFloor(minval2.get0()));
        xmax = max(xmax, cvFloor(maxval2.get0()));
        ymin = min(ymin, cvFloor(v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(minval2))).get0()));
        ymax = max(ymax, cvFloor(v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(maxval2))).get0()));
    }
#endif
#else
    const glm::i32vec2* pts = (glm::i32vec2*)points.data();
    glm::i32vec2 pt = pts[0];

    Cv32suf v;
    // init values
    xmin = xmax = CV_TOGGLE_FLT(pt.x);
    ymin = ymax = CV_TOGGLE_FLT(pt.y);

    for (i = 1; i < npoints; i++){
        pt = pts[i];
        pt.x = CV_TOGGLE_FLT(pt.x);
        pt.y = CV_TOGGLE_FLT(pt.y);

        if (xmin > pt.x)
            xmin = pt.x;

        if (xmax < pt.x)
            xmax = pt.x;

        if (ymin > pt.y)
            ymin = pt.y;

        if (ymax < pt.y)
            ymax = pt.y;
    }

    v.i = CV_TOGGLE_FLT(xmin); xmin = cvFloor(v.f);
    v.i = CV_TOGGLE_FLT(ymin); ymin = cvFloor(v.f);
    // because right and bottom sides of the bounding rectangle are not inclusive
    // (note +1 in width and height calculation below), cvFloor is used here instead of cvCeil
    v.i = CV_TOGGLE_FLT(xmax); xmax = cvFloor(v.f);
    v.i = CV_TOGGLE_FLT(ymax); ymax = cvFloor(v.f);
#endif
    return glm::i32vec4{ xmin, ymin, xmax - xmin + 1, ymax - ymin + 1 };
}

glm::i32vec4 pointSetBoundingRect(const std::vector<glm::i32vec2>& points) {
    using container_type = std::vector<glm::i32vec2>;
    using container_value_type = container_type::value_type;
    using value_type = container_value_type::value_type;

    auto npoints = points.size();
    if (npoints == 0)
        return glm::i32vec4{};

    int  xmin = 0, ymin = 0, xmax = -1, ymax = -1, i;

    using namespace cv;
#if CV_SIMD
    const int64_t* pts = (int64_t*)points.data();

    v_int32 minval, maxval;
    minval = maxval = v_reinterpret_as_s32(vx_setall_s64(*pts)); //min[0]=pt.x, min[1]=pt.y, min[2]=pt.x, min[3]=pt.y
    for (i = 1; i <= npoints - v_int32::nlanes / 2; i += v_int32::nlanes / 2)
    {
        v_int32 ptXY2 = v_reinterpret_as_s32(vx_load(pts + i));
        minval = v_min(ptXY2, minval);
        maxval = v_max(ptXY2, maxval);
    }
    minval = v_min(v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(minval))), v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(minval))));
    maxval = v_max(v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(maxval))), v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(maxval))));
    if (i <= npoints - v_int32::nlanes / 4)
    {
        v_int32 ptXY = v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(vx_load_low(pts + i))));
        minval = v_min(ptXY, minval);
        maxval = v_max(ptXY, maxval);
        i += v_int64::nlanes / 2;
    }
    for (int j = 16; j < CV_SIMD_WIDTH; j *= 2)
    {
        minval = v_min(v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(minval))), v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(minval))));
        maxval = v_max(v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(maxval))), v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(maxval))));
    }
    xmin = minval.get0();
    xmax = maxval.get0();
    ymin = v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(minval))).get0();
    ymax = v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(maxval))).get0();
#if CV_SIMD_WIDTH > 16
    if (i < npoints)
    {
        v_int32x4 minval2, maxval2;
        minval2 = maxval2 = v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(v_load_low(pts + i))));
        for (i++; i < npoints; i++)
        {
            v_int32x4 ptXY = v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(v_load_low(pts + i))));
            minval2 = v_min(ptXY, minval2);
            maxval2 = v_max(ptXY, maxval2);
        }
        xmin = min(xmin, minval2.get0());
        xmax = max(xmax, maxval2.get0());
        ymin = min(ymin, v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(minval2))).get0());
        ymax = max(ymax, v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(maxval2))).get0());
    }
#endif
#else
    const container_value_type* pts = points.data();
    container_value_type pt = pts[0];
    
    xmin = xmax = pt.x;
    ymin = ymax = pt.y;

    for (i = 1; i < npoints; i++)
    {
        pt = pts[i];

        if (xmin > pt.x)
            xmin = pt.x;

        if (xmax < pt.x)
            xmax = pt.x;

        if (ymin > pt.y)
            ymin = pt.y;

        if (ymax < pt.y)
            ymax = pt.y;
    }
#endif
    return glm::i32vec4{ xmin, ymin, xmax - xmin + 1, ymax - ymin + 1 };
}

std::vector<glm::i32vec2> int_list = {};
std::vector<glm::vec2> float_list = {};

void generator_int_list() {
    int_list.emplace_back(0, 0);
    int_list.emplace_back(100, 0);
    int_list.emplace_back(100, 100);
    int_list.emplace_back(0, 100);

    for (int i = 0; i <= 1920; ++i) {
        for (int j = 0; j <= 1080; ++j) {
            int_list.emplace_back(i, j);
        }
    } 
}

void generator_float_list() {
    float_list.emplace_back(0, 0);
    float_list.emplace_back(100, 0);
    float_list.emplace_back(100, 100);
    float_list.emplace_back(0, 100);

    for (int i = 0; i <= 1920; ++i) {
        for (int j = 0; j <= 1080; ++j) {
            float_list.emplace_back(i, j);
        }
    }
}

void glm_version() {
    glm::i32vec4 int1{};
    {
#define LOOP_CNT 100
        Timer timer("glm int", LOOP_CNT);
        for (int i = 0; i < LOOP_CNT; ++i) {
            int1 = pointSetBoundingRect(int_list);
        }
    }
    std::cout << std::format("cnt:{}, ({},{},{},{})\n", int_list.size(), int1.x, int1.y, int1.z, int1.w);

    glm::i32vec4 float1{};
    {
#define LOOP_CNT 100
        Timer timer("glm float", LOOP_CNT);
        for (int i = 0; i < LOOP_CNT; ++i) {
            float1 = pointSetBoundingRect(float_list);
        }
    }
    std::cout << std::format("cnt:{}, ({},{},{},{})\n", float_list.size(), float1.x, float1.y, float1.z, float1.w);
}

int main()
{
    generator_int_list();
    generator_float_list();
    glm_version();

}
