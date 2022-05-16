#include "Core.h"


namespace dr {
    namespace core {     

        void gaussianElimination(float(&input)[8][9]) {
            auto i = 0;
            auto j = 0;
            constexpr int n = 9;
            constexpr auto m = n - 1;

            while (i < m && j < n) {
                auto iMax = i;
                for (auto k = i + 1; k < m; ++k) {
                    if (std::fabs(input[k][j]) > std::fabs(input[iMax][j])) {
                        iMax = k;
                    }
                }

                if (input[iMax * n + j] != 0) {
                    if (i != iMax) {
                        for (auto k = 0; k < n; ++k) {
                            auto ikIn = input[i][k];
                            input[i][k] = input[iMax][k];
                            input[iMax][k] = ikIn;
                        }
                    }

                    float ijIn = input[i][j];
                    for (auto k = 0; k < n; ++k) {
                        input[i][k] /= ijIn;
                    }

                    for (auto u = i + 1; u < m; ++u) {
                        auto ujIn = input[u][j];
                        for (auto k = 0; k < n; ++k) {
                            input[u][k] -= ujIn * input[i][k];
                        }
                    }
                    ++i;
                }
                ++j;
            }

            for (auto i = m - 2; i >= 0; --i) {
                for (auto j = i + 1; j < n - 1; ++j) {
                    input[i][m] -= input[i][j] * input[j][m];
                }
            }
        }


        //--------------------------------------------------------------
        // Adapted from: http://forum.openframeworks.cc/t/quad-warping-homography-without-opencv/3121/19
        glm::mat4 getPerspectiveTransform(const glm::vec2(&src)[4], const glm::vec2(&dst)[4]) {
            float p[8][9] =
            {
                { -src[0][0], -src[0][1], -1, 0, 0, 0, src[0][0] * dst[0][0], src[0][1] * dst[0][0], -dst[0][0] }, // h11
                { 0, 0, 0, -src[0][0], -src[0][1], -1, src[0][0] * dst[0][1], src[0][1] * dst[0][1], -dst[0][1] }, // h12
                { -src[1][0], -src[1][1], -1, 0, 0, 0, src[1][0] * dst[1][0], src[1][1] * dst[1][0], -dst[1][0] }, // h13
                { 0, 0, 0, -src[1][0], -src[1][1], -1, src[1][0] * dst[1][1], src[1][1] * dst[1][1], -dst[1][1] }, // h21
                { -src[2][0], -src[2][1], -1, 0, 0, 0, src[2][0] * dst[2][0], src[2][1] * dst[2][0], -dst[2][0] }, // h22
                { 0, 0, 0, -src[2][0], -src[2][1], -1, src[2][0] * dst[2][1], src[2][1] * dst[2][1], -dst[2][1] }, // h23
                { -src[3][0], -src[3][1], -1, 0, 0, 0, src[3][0] * dst[3][0], src[3][1] * dst[3][0], -dst[3][0] }, // h31
                { 0, 0, 0, -src[3][0], -src[3][1], -1, src[3][0] * dst[3][1], src[3][1] * dst[3][1], -dst[3][1] }, // h32
            };

            gaussianElimination(p);

            return glm::mat4(   p[0][8], p[3][8], 0, p[6][8],
                                p[1][8], p[4][8], 0, p[7][8],
                                0, 0, 1, 0,
                                p[2][8], p[5][8], 0, 1);
        }
                         
        
    }// 

    namespace cl {

        cmd_queue::cmd_queue(const ::cl::Context& context, const std::vector<::cl::Device>& devices, bool ordered, bool profile) {
            size = devices.size();
            cl_command_queue_properties pro = ordered ?  0: CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
            pro |= profile ? CL_QUEUE_PROFILING_ENABLE : 0;

            for (auto&& item: devices) {
                auto max_size = item.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
                device_info.emplace_back(::cl::CommandQueue{ context, item, pro }, max_size);
            }
            idx = 0;
        }

        cmd_queue::DEVICE_INFO& cmd_queue::next() {
            auto p = (idx++) % (size);
            return device_info[p];
        }

        ::cl::CommandQueue& cmd_queue::get_queue() {
            auto p = (idx++) % (size);
            return device_info[p].queue;
        }

        cl_int cmd_queue::enqueue_wait_marker() const {
            cl_int ret{ CL_SUCCESS };
            std::vector<::cl::Event> events;
            for (auto&& item : this->device_info) {
                ::cl::Event my_event;
                ret = item.queue.enqueueMarkerWithWaitList(&events, &my_event);
                events.push_back(my_event);
            }
            return ret;
        }

        cl_int  cmd_queue::finish() const {
            cl_int ret{ CL_SUCCESS };
            for (auto&& item : this->device_info) {
                ret = item.queue.finish();
            }
            return ret;
        }

        cl_int  cmd_queue::flush() const {
            cl_int ret{ CL_SUCCESS };
            for (auto&& item : this->device_info) {
                ret = item.queue.flush();
            }
            return ret;
        }

        cl_int cmd_queue::enqueueWriteImage(
            const ::cl::Image& image,
            cl_bool blocking,
            const void* ptr,
            const ::cl::vector<::cl::Event>* events,
            ::cl::Event* event) {
            auto width = image.getImageInfo<CL_IMAGE_WIDTH>();
            auto height = image.getImageInfo<CL_IMAGE_HEIGHT>();

            ::cl::array<::cl::size_type, 3> origin{};
            ::cl::array<::cl::size_type, 3> region{ width , height, 1 };
            return this->enqueueWriteImage(image, blocking, origin, region, ptr, events, event);
        }

        cl_int cmd_queue::enqueueReadImage(
            const ::cl::Image& image,
            cl_bool blocking,
            void* ptr,
            const ::cl::vector<::cl::Event>* events,
            ::cl::Event* event) {
            auto width = image.getImageInfo<CL_IMAGE_WIDTH>();
            auto height = image.getImageInfo<CL_IMAGE_HEIGHT>();

            ::cl::array<::cl::size_type, 3> origin{};
            ::cl::array<::cl::size_type, 3> region{ width , height, 1 };
            return this->enqueueReadImage(image, blocking, origin, region, ptr, events, event);
        }

        cl_int cmd_queue::enqueueReadImage(
            const ::cl::Image& image,
            cl_bool blocking,
            const ::cl::array<::cl::size_type, 3>& origin,
            const ::cl::array<::cl::size_type, 3>& region,
            void* ptr,
            const ::cl::vector<::cl::Event>* events,
            ::cl::Event* event) {
            auto row_pitch = image.getImageInfo<CL_IMAGE_ROW_PITCH>();
            auto elem_size = image.getImageInfo<CL_IMAGE_ELEMENT_SIZE>();

            cl_int ret{ CL_SUCCESS };

            auto& info = this->next();

            auto tiles = this->get_ND(info, origin, region);
            for (auto& [orgin, region] : tiles) {
                void* off_ptr = row_pitch * orgin[1] + orgin[0] * elem_size + (char*)ptr;
                ret = info.queue.enqueueReadImage(image, blocking, orgin, region, row_pitch, 0, off_ptr);
            }
            return ret;
        }

        cl_int cmd_queue::enqueueWriteImage(
            const ::cl::Image& image,
            cl_bool blocking,
            const ::cl::array<::cl::size_type, 3>& origin,
            const ::cl::array<::cl::size_type, 3>& region,
            const void* ptr,
            const ::cl::vector<::cl::Event>* events,
            ::cl::Event* event) {
            auto row_pitch = image.getImageInfo<CL_IMAGE_ROW_PITCH>();
            auto elem_size = image.getImageInfo<CL_IMAGE_ELEMENT_SIZE>();

            cl_int ret{ CL_SUCCESS };

            auto& info = this->next();

            auto tiles = this->get_ND(info, origin, region);
            for (auto& [orgin, region] : tiles) {
                void* off_ptr = row_pitch * orgin[1] + orgin[0] * elem_size + (char*)ptr;
                ret = info.queue.enqueueWriteImage(image, blocking, orgin, region, row_pitch, 0, off_ptr);
            }
            return ret;
        }

        cl_int cmd_queue::enqueueCopyImage(
            const ::cl::Image& src,
            const ::cl::Image& dst,
            const ::cl::array<::cl::size_type, 3>& src_origin,
            const ::cl::array<::cl::size_type, 3>& dst_origin,
            const ::cl::array<::cl::size_type, 3>& region,
            const ::cl::vector<::cl::Event>* events,
            ::cl::Event* event) {
            cl_int ret{ CL_SUCCESS };
            auto& info = this->next();

            ::cl::array<::cl::size_type, 3> zero_orgin{};
            auto tiles = this->get_ND(info, zero_orgin, region);
            for (auto& [orgin, region]: tiles) {
                ret = info.queue.enqueueCopyImage(
                    src, dst, 
                    ::cl::array<::cl::size_type, 3>{src_origin[0] + orgin[0], src_origin[1] + orgin[1], src_origin[2] + orgin[2]},
                    ::cl::array<::cl::size_type, 3>{dst_origin[0] + orgin[0], dst_origin[1] + orgin[1], dst_origin[2] + orgin[2]},
                    region,events, event);
            }
            return ret;
        }

        cl_int cmd_queue::enqueueCopyImage(
            const ::cl::Image& src,
            const ::cl::Image& dst,
            const bound_type& bound,
            const ::cl::vector<::cl::Event>* events,
            ::cl::Event* event) {
            ::cl::array<::cl::size_type, 3> origin{ (::cl::size_type)bound[0], (::cl::size_type)bound[1], 0 };
            ::cl::array<::cl::size_type, 3> region{ (::cl::size_type)(bound[2] - bound[0]), ::cl::size_type(bound[3] - bound[1]), 1 };
            return enqueueCopyImage(src, dst, origin, origin, region, events, event);
        }

        cl_int cmd_queue::enqueueNDRangeKernel(
            const ::cl::Kernel& kernel,
            const bound_type& bound,
            const ::cl::NDRange& local,
            const ::cl::vector<::cl::Event>* events,
            ::cl::Event* event ) {
            ::cl::NDRange offset = { (::cl::size_type)bound[0], (::cl::size_type)bound[1], 0 };
            ::cl::NDRange global = { (::cl::size_type)(bound[2] - bound[0]), (::cl::size_type)(bound[3] - bound[1]), 1 };
            return this->enqueueNDRangeKernel(kernel, offset, global, local, events, event);
        }

        cl_int cmd_queue::enqueueNDRangeKernel(
            const ::cl::Kernel& kernel,
            const ::cl::NDRange& offset,
            const ::cl::NDRange& global,
            const ::cl::NDRange& local,
            const ::cl::vector<::cl::Event>* events, ::cl::Event* event)  {
            auto& info = this->next();
            auto tiles = this->get_ND(info, offset, global);

            cl_int ret{ CL_SUCCESS };
            for (auto& [orgin, region] : tiles) {
                ret = info.queue.enqueueNDRangeKernel(kernel, orgin, region, local,events, event);
            }

            return ret;
        }

        std::vector<cmd_queue::ND> cmd_queue::get_ND(DEVICE_INFO& info, const ::cl::NDRange& offset, const ::cl::NDRange& global) {
            std::vector<cmd_queue::ND> zones{};
            do {
                if (global[0] < 1 || global[1] < 1 || global[2] < 1) break;

                for (size_type z = 0; z < global[2]; z += info.max_work_item_size[2]) {
                    for (size_type y = 0; y < global[1]; y += info.max_work_item_size[1]) {
                        for (size_type x = 0; x < global[0]; x += info.max_work_item_size[0]) {
                            zones.emplace_back(
                                ::cl::NDRange{ x + offset[0], y + offset[1], z + offset[2] },
                                ::cl::NDRange{
                                    std::min(x + info.max_work_item_size[0], global[0]) - x,
                                    std::min(y + info.max_work_item_size[1], global[1]) - y,
                                    std::min(z + info.max_work_item_size[2], global[2]) - z
                                });
                        }
                    }
                }
            } while (false);

            return zones;
        }

        std::vector<cmd_queue::TILE> 
            cmd_queue::get_ND(DEVICE_INFO& info, const ::cl::array<::cl::size_type, 3>& origin, const ::cl::array<::cl::size_type, 3>& region){
            std::vector<cmd_queue::TILE> zones{};
            do {
                if (region[0] < 1 || region[1] < 1 || region[2] < 1) break;

                for (size_type z = 0; z < region[2]; z += info.max_work_item_size[2]) {
                    for (size_type y = 0; y < region[1]; y += info.max_work_item_size[1]) {
                        for (size_type x = 0; x < region[0]; x += info.max_work_item_size[0]) {
                            zones.emplace_back(
                                ::cl::array<::cl::size_type, 3>{ x + origin[0], y + origin[1], z + origin[2] },
                                ::cl::array<::cl::size_type, 3>{
                                    std::min(x + info.max_work_item_size[0], region[0]) - x,
                                    std::min(y + info.max_work_item_size[1], region[1]) - y,
                                    std::min(z + info.max_work_item_size[2], region[2]) - z
                                });
                        }
                    }
                }
            } while (false);

            return zones;
        }              

    }
}

