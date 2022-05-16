#pragma once
#include <iostream>
#include <atomic>
#include <CL/cl.hpp>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>
#include <ranges>
#include <tuple>
#include <opencv2/core/hal/intrin.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>


#define  CV_TOGGLE_FLT(x) ((x)^((int)(x) < 0 ? 0x7fffffff : 0))

namespace dr {
	namespace core {
		template<std::ranges::contiguous_range Con>
		glm::i32vec4 getBoundingRect(const Con& points)
            requires std::is_same_v<std::ranges::range_value_t<Con>, glm::vec2> 
        {
            using container_type = Con;
            using value_type = std::ranges::range_value_t<Con>;

            auto npoints = std::size(points);
            if (npoints == 0)
                return {};

            int  xmin = 0, ymin = 0, xmax = -1, ymax = -1, i;

            using namespace cv;
#if CV_SIMD
            const int64_t* pts = (int64_t*)std::data(points);

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
            if (i <= npoints - v_float32::nlanes / 4) {
                v_float32 ptXY = v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(vx_load_low(pts + i))));
                minval = v_min(ptXY, minval);
                maxval = v_max(ptXY, maxval);
                i += v_float32::nlanes / 4;
            }
            for (int j = 16; j < CV_SIMD_WIDTH; j *= 2){
                minval = v_min(v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(minval))), v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(minval))));
                maxval = v_max(v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(maxval))), v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(maxval))));
            }
            xmin = cvFloor(minval.get0());
            xmax = cvFloor(maxval.get0());
            ymin = cvFloor(v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(minval))).get0());
            ymax = cvFloor(v_reinterpret_as_f32(v_expand_high(v_reinterpret_as_u32(maxval))).get0());
#if CV_SIMD_WIDTH > 16
            if (i < npoints) {
                v_float32x4 minval2, maxval2;
                minval2 = maxval2 = v_reinterpret_as_f32(v_expand_low(v_reinterpret_as_u32(v_load_low(pts + i))));
                for (i++; i < npoints; i++) {
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
            const glm::i32vec2* pts = (glm::i32vec2*)std::data(points);
            glm::i32vec2 pt = pts[0];

            Cv32suf v;
            // init values
            xmin = xmax = CV_TOGGLE_FLT(pt.x);
            ymin = ymax = CV_TOGGLE_FLT(pt.y);

            for (i = 1; i < npoints; i++) {
                pt = pts[i];
                pt.x = CV_TOGGLE_FLT(pt.x);
                pt.y = CV_TOGGLE_FLT(pt.y);

                if (xmin > pt.x) xmin = pt.x;
                if (xmax < pt.x) xmax = pt.x;
                if (ymin > pt.y) ymin = pt.y;
                if (ymax < pt.y) ymax = pt.y;
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

        template<std::ranges::contiguous_range Con>
        glm::i32vec4 getBoundingRect(const Con& points) 
            requires std::is_same_v<std::ranges::range_value_t<Con>, glm::i32vec2>
        {
            using container_type = Con;
            using value_type = std::ranges::range_value_t<Con>;

            auto npoints = std::size(points);
            if (npoints == 0)
                return {};

            int  xmin = 0, ymin = 0, xmax = -1, ymax = -1, i;

            using namespace cv;
#if CV_SIMD
            const int64_t* pts = (int64_t*)std::data(points);

            v_int32 minval, maxval;
            minval = maxval = v_reinterpret_as_s32(vx_setall_s64(*pts)); //min[0]=pt.x, min[1]=pt.y, min[2]=pt.x, min[3]=pt.y
            for (i = 1; i <= npoints - v_int32::nlanes / 2; i += v_int32::nlanes / 2) {
                v_int32 ptXY2 = v_reinterpret_as_s32(vx_load(pts + i));
                minval = v_min(ptXY2, minval);
                maxval = v_max(ptXY2, maxval);
            }
            minval = v_min(v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(minval))), v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(minval))));
            maxval = v_max(v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(maxval))), v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(maxval))));
            if (i <= npoints - v_int32::nlanes / 4) {
                v_int32 ptXY = v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(vx_load_low(pts + i))));
                minval = v_min(ptXY, minval);
                maxval = v_max(ptXY, maxval);
                i += v_int64::nlanes / 2;
            }
            for (int j = 16; j < CV_SIMD_WIDTH; j *= 2) {
                minval = v_min(v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(minval))), v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(minval))));
                maxval = v_max(v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(maxval))), v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(maxval))));
            }
            xmin = minval.get0();
            xmax = maxval.get0();
            ymin = v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(minval))).get0();
            ymax = v_reinterpret_as_s32(v_expand_high(v_reinterpret_as_u32(maxval))).get0();
#if CV_SIMD_WIDTH > 16
            if (i < npoints) {
                v_int32x4 minval2, maxval2;
                minval2 = maxval2 = v_reinterpret_as_s32(v_expand_low(v_reinterpret_as_u32(v_load_low(pts + i))));
                for (i++; i < npoints; i++){
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
            const container_value_type* pts = std::data(points);
            container_value_type pt = pts[0];

            xmin = xmax = pt.x;
            ymin = ymax = pt.y;

            for (i = 1; i < npoints; i++) {
                pt = pts[i];

                if (xmin > pt.x) xmin = pt.x;
                if (xmax < pt.x) xmax = pt.x;
                if (ymin > pt.y) ymin = pt.y;
                if (ymax < pt.y) ymax = pt.y;
            }
#endif
            return glm::i32vec4{ xmin, ymin, xmax - xmin + 1, ymax - ymin + 1 };
        }

	
		glm::mat4 getPerspectiveTransform(const glm::vec2(&src)[4], const glm::vec2(&dst)[4]);	

        template<class T>
        T intersection(const T& a, const T& b)
            requires requires(T a) {
              a[0] < a[1] < a[2] < a[3];
              a[0] + a[1];
              std::min(a[0], a[1]);
            }
        {
            using value_type = T;
            value_type ret{};

            const value_type& Rx_min = (a[0] < b[0]) ? a : b;
            const value_type& Rx_max = (a[0] < b[0]) ? b : a;
            const value_type& Ry_min = (a[1] < b[1]) ? a : b;
            const value_type& Ry_max = (a[1] < b[1]) ? b : a;
            // Looking at the formula below, we will compute Rx_min.width - (Rx_max.x - Rx_min.x)
            // but we want to avoid overflows. Rx_min.width >= 0 and (Rx_max.x - Rx_min.x) >= 0
            // by definition so the difference does not overflow. The only thing that can overflow
            // is (Rx_max.x - Rx_min.x). And it can only overflow if Rx_min.x < 0.
            // Let us first deal with the following case.
            if ((Rx_min[0] < 0 && Rx_min[0] + Rx_min[2] < Rx_max[0]) ||
                (Ry_min[1] < 0 && Ry_min[1] + Ry_min[3] < Ry_max[1])) {
                return ret;
            }
            // We now know that either Rx_min.x >= 0, or
            // Rx_min.x < 0 && Rx_min.x + Rx_min.width >= Rx_max.x and therefore
            // Rx_min.width >= (Rx_max.x - Rx_min.x) which means (Rx_max.x - Rx_min.x)
            // is inferior to a valid int and therefore does not overflow.
            ret[0] = Rx_max[0];
            ret[1] = Ry_max[1];

            ret[2] = ret[0] + std::min(Rx_min[2] - (Rx_max[0] - Rx_min[0]), Rx_max[2]);
            ret[3] = ret[1] + std::min(Ry_min[3] - (Ry_max[1] - Ry_min[1]), Ry_max[3]);
            return ret;
        }
	}

    namespace cl {

        class cmd_queue {
        public:
            using bound_type = glm::i32vec4;
            using size_type = ::cl::size_type;

            struct DEVICE_INFO{
                ::cl::CommandQueue queue;
                ::cl::vector<::cl::size_type> max_work_item_size;
            };

            struct ND {
                ::cl::NDRange offset;
                ::cl::NDRange global;
            };

            struct TILE {
                ::cl::array<::cl::size_type, 3> origin;
                ::cl::array<::cl::size_type, 3> region;
            };

        public:
            std::vector<DEVICE_INFO> device_info;

            unsigned int size;
            std::atomic<unsigned long> idx;

        private:
            DEVICE_INFO & next();

        public:
            cmd_queue(const ::cl::Context& context, const std::vector<::cl::Device>& device, bool ordered=true, bool profile=false);
            
            ::cl::CommandQueue& get_queue();

            cl_int enqueue_wait_marker() const;
            cl_int finish() const;
            cl_int flush() const;

            cl_int enqueueCopyImage(
                const ::cl::Image& src,
                const ::cl::Image& dst,
                const ::cl::array<::cl::size_type, 3>& src_origin,
                const ::cl::array<::cl::size_type, 3>& dst_origin,
                const ::cl::array<::cl::size_type, 3>& region,
                const ::cl::vector<::cl::Event>* events = NULL,
                ::cl::Event* event = NULL);

            cl_int enqueueCopyImage(
                const ::cl::Image& src,
                const ::cl::Image& dst,
                const bound_type& bound,
                const ::cl::vector<::cl::Event>* events = NULL,
                ::cl::Event* event = NULL);

            cl_int enqueueWriteImage(
                const ::cl::Image& image,
                cl_bool blocking,
                const ::cl::array<::cl::size_type, 3>& origin,
                const ::cl::array<::cl::size_type, 3>& region,
                const void* ptr,
                const ::cl::vector<::cl::Event>* events = NULL,
                ::cl::Event* event = NULL);

            cl_int enqueueWriteImage(
                const ::cl::Image& image,
                cl_bool blocking,
                const void* ptr,
                const ::cl::vector<::cl::Event>* events = NULL,
                ::cl::Event* event = NULL);

            cl_int enqueueReadImage(
                const ::cl::Image& image,
                cl_bool blocking,
                void* ptr,
                const ::cl::vector<::cl::Event>* events = NULL,
                ::cl::Event* event = NULL);

            cl_int enqueueReadImage(
                const ::cl::Image& image,
                cl_bool blocking,
                const ::cl::array<::cl::size_type, 3>& origin,
                const ::cl::array<::cl::size_type, 3>& region,
                void* ptr,
                const ::cl::vector<::cl::Event>* events = NULL,
                ::cl::Event* event = NULL);


            cl_int enqueueNDRangeKernel(
                const ::cl::Kernel& kernel,
                const bound_type& bound,
                const ::cl::NDRange& local = ::cl::NullRange,
                const ::cl::vector<::cl::Event>* events = NULL,
                ::cl::Event* event = NULL
            ) ;

            cl_int enqueueNDRangeKernel(
                const ::cl::Kernel& kernel,
                const ::cl::NDRange& offset,
                const ::cl::NDRange& global,
                const ::cl::NDRange& local = ::cl::NullRange,
                const ::cl::vector<::cl::Event>* events = NULL,
                ::cl::Event* event = NULL);
        private:
            std::vector<ND> get_ND(DEVICE_INFO& info, const ::cl::NDRange& offset, const ::cl::NDRange& global);
            std::vector<TILE> get_ND(DEVICE_INFO& info, const ::cl::array<::cl::size_type, 3>& origin, const ::cl::array<::cl::size_type, 3>& region);
          
        };

    }
}



