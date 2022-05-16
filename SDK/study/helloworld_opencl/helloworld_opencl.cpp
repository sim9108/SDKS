#include <iostream>
#include <fstream>
#include <chrono>

#include "fpng.h"
#include "Lib/Core/Core.h"

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
        auto diff = (end - start);
        auto value = diff.count() / cnt;
        std::cout << "title:" << title << " Time :" << value << "ms FPS:" << (1000.0 / value) << "\n";
    }
};

int main(){
    int* p = ::nullptr;

    using namespace dr::core;
    // input image ready
    std::vector<uint8_t> img;

    uint32_t width{ 0 }, height{ 0 }, channels;
    do {
        std::ifstream fp("Img512.png", std::ios_base::binary);
        std::vector<unsigned char> file_data(std::istreambuf_iterator<char>(fp), (std::istreambuf_iterator<char>()));

        int r = fpng::fpng_get_info(file_data.data(), file_data.size(), width, height, channels);
        if (r != fpng::FPNG_DECODE_SUCCESS) break;

        fpng::fpng_decode_memory(file_data.data(), file_data.size(), img, width, height, channels, 4);
    } while (false);

    glm::vec2 src_list[4] = {
       {0.0f,       0.0f},
       { width,     0.0f },
       { width,     height },
       { 0.0f,      height }
    };
       
    //[point(797.2312, -815.3074), point(4122.7688, 1104.6926), point(3042.7688, 2975.3074), point(-282.7688, 1055.3074)]
    glm::vec2 dest_list[4] = {
        { 0.0, 0.0 },
        { 3840-1, 0 },
        { 3840-1, 2160-1},
        { 0, 2160 -1}
    };

    for (auto&& item : dest_list) item += glm::vec2{ 3840/2,2160/2 };

    std::vector<uint8_t> dst;
    dst.resize(img.size());
        
    // opencl configuration
    // platform
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    for (auto& platform : platforms) {
       std::vector<cl::Device> devices;
       platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

       for (auto& device : devices) {
           auto max_work_group_size = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
           auto max_work_item_size = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
           auto max_work_item_dimensions = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>();
           auto extension_with_version = platform.getInfo<CL_PLATFORM_EXTENSIONS_WITH_VERSION>();
           auto openc_c_all_versions = device.getInfo<CL_DEVICE_OPENCL_C_ALL_VERSIONS>();
           std::cout << "DEVICE OPENCL_C_ALL_VERSIONS:";
           for (auto&& item : openc_c_all_versions) {
               std::cout << std::format("\n{} {}.{}.{}", item.name, 
                   CL_VERSION_MAJOR(item.version),
                   CL_VERSION_MINOR(item.version),
                   CL_VERSION_PATCH(item.version)
               );
           }
           std::cout << "\n";

           auto openc_c_features = device.getInfo<CL_DEVICE_OPENCL_C_FEATURES>();
           std::cout << "DEVICE OPENCL_C_FEATURES:";
           for (auto&& item : openc_c_features) {
               std::cout << std::format("\n{} {}.{}.{}", item.name,
                   CL_VERSION_MAJOR(item.version),
                   CL_VERSION_MINOR(item.version),
                   CL_VERSION_PATCH(item.version)
               );
           }
           std::cout << "\n";

           std::cout <<
               std::format(
                   "PLATFORM NAME:{}\n"
                   "PLATFORM VERSION:{}\n"
                   "PLATFORM EXTENSIONS:\n{}\n"
                   "DEVICE NAME:{}\n"
                   "DEVICE TYPE:{}\n"
                   "DEVICE OPENCL_C_VERSION:{}\n"
                   "DEVICE DEVICE VERSION:{}\n"
                   "DEVICE DRIVER VERSION:{}\n"
                   "DEVICE IL VERSION:{}\n"
                   "DEVICE MAX_COMPUTE_UNITS:{}\n"
                   "DEVICE PREFERRED_VECTOR_WIDTH_HALF:{}\n"
                   "DEVICE PREFERRED_VECTOR_WIDTH_FLOAT:{}\n"
                   "DEVICE ADDRESS BITS:{}\n"
                   "DEVICE MAX_MEM_ALLOC_SIZE:{}MB\n"
                   "DEVICE IMAGE_SUPPORT:{}\n"
                   "DEVICE MAX_READ_WRITE_IMAGE_ARGS:{}\n"
                   "DEVICE PROFILE:{}\n"
                   "DEVICE IMAGE2D_MAX_WIDTH:{}\n"
                   "DEVICE IMAGE2D_MAX_HEIGHT:{}\n"     
                   "DEVICE MEM_BASE_ADDR_ALIGN:{}bits\n"
                   "DEVICE BUILT_IN_KERNELS:{}\n"
                   "DEVICE MAX_WORK_GROUP_SIZE:{}\n"
                   "DEVICE MAX_WORK_ITEM_SIZES:{}x{}x{}\n"
                   "DEVICE MAX_WORK_ITEM_DIMENSIONS:{}\n"                   
                   "//////////////////////\n",
                   platform.getInfo<CL_PLATFORM_NAME>(),
                   platform.getInfo<CL_PLATFORM_VERSION>(),
                   platform.getInfo<CL_PLATFORM_EXTENSIONS>(),
                   device.getInfo<CL_DEVICE_NAME>(),
                   device.getInfo<CL_DEVICE_TYPE>(),
                   device.getInfo<CL_DEVICE_OPENCL_C_VERSION>(),
                   device.getInfo<CL_DEVICE_VERSION>(),
                   device.getInfo<CL_DRIVER_VERSION>(),
                   device.getInfo<CL_DEVICE_IL_VERSION>(),
                   device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>(),
                   device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF>(),
                   device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT>(),
                   device.getInfo<CL_DEVICE_ADDRESS_BITS>(),
                   device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>()/1024/1024,
                   device.getInfo<CL_DEVICE_IMAGE_SUPPORT>(),
                   device.getInfo<CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS>(),
                   device.getInfo<CL_DEVICE_PROFILE>(),
                   device.getInfo<CL_DEVICE_IMAGE2D_MAX_WIDTH>(),
                   device.getInfo<CL_DEVICE_IMAGE2D_MAX_HEIGHT>(),
                   device.getInfo<CL_DEVICE_MEM_BASE_ADDR_ALIGN>(),
                   device.getInfo<CL_DEVICE_BUILT_IN_KERNELS>(),
                   max_work_group_size,
                   max_work_item_size[0], max_work_item_size[1], max_work_item_size[2],
                   max_work_item_dimensions
               );
       }
    }
    
    // platform && device && context
    auto platform = platforms[0];
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    cl::Context context(devices);


    std::ifstream helloWorldFile("hello.cl");
    std::string src(std::istreambuf_iterator<char>(helloWorldFile), (std::istreambuf_iterator<char>()));
    cl::Program::Sources sources= {src};    
    cl::Program program(context, sources);

    auto err = program.build("-cl-std=CL3.0");// -cl - ext = +__opencl_c_images");
    std::cout << "\ncompile:" << err << '\n';
   
    dr::cl::cmd_queue ordered_queue{ context, devices };
    
    /*std::vector<glm::vec4> lines;
    for (int i = 0; i < 50; ++i) {
        auto x = cos( i * 3.14 / 180.0)* 1000;
        auto y = sin( i * 3.14 / 180.0)* 1000;
        lines.emplace_back(900, 900, 900 + x, 900 + y);
    }*/

    do {
        cl::ImageFormat format_f(CL_RGBA, CL_FLOAT);
        cl::Image2D imgInf(context, CL_MEM_READ_WRITE, format_f, width, height);
        cl::Image2D imgStagef(context, CL_MEM_READ_WRITE, format_f, width, height);
        cl::Image2D imgOutf(context, CL_MEM_READ_WRITE, format_f, width, height);

        cl::ImageFormat format(CL_RGBA, CL_UNSIGNED_INT8);
        cl::Image2D imgStage(context, CL_MEM_READ_WRITE, format, width, height);


        cl::Kernel to_uint8_rgba_kernel(program, "to_uint8_rgba", &err);
        cl::Kernel to_float_rgba_kernel(program, "to_float_rgba", &err);
        {
            cl::Image2D imgIn(context, CL_MEM_READ_WRITE, format, width, height);
            cl::Image2D imgOut(context, CL_MEM_READ_WRITE, format, width, height);
            for (auto&& item : { imgIn, imgStage, imgOut }) {
                ordered_queue.enqueueWriteImage(item, CL_FALSE, img.data());
            }

            to_float_rgba_kernel.setArg(0, imgIn);
            to_float_rgba_kernel.setArg(1, imgInf);
            ordered_queue.enqueueNDRangeKernel(to_float_rgba_kernel, { 0,0, width, height });

            to_float_rgba_kernel.setArg(0, imgOut);
            to_float_rgba_kernel.setArg(1, imgOutf);
            ordered_queue.enqueueNDRangeKernel(to_float_rgba_kernel, { 0,0, width, height });

            to_float_rgba_kernel.setArg(0, imgStage);
            to_float_rgba_kernel.setArg(1, imgStagef);
            ordered_queue.enqueueNDRangeKernel(to_float_rgba_kernel, { 0,0, width, height });
        }

        cl::Kernel draw_lines_kernel(program, "draw_lines", &err);
        cl::Kernel render_kernel(program, "render", &err);

        std::cout << "error:" << err << std::endl;

#define LOOP_CNT 5
        {            
            Timer timer{ "opencl time:", 1 };            

            for (int i = 0; i < LOOP_CNT; ++i) {
                auto bound = dr::core::getBoundingRect(dest_list);
                auto dest_mat = getPerspectiveTransform(dest_list, src_list);
                auto in_bound = dr::core::intersection(bound, { 0, 0, width, height });

                do {
                    render_kernel.setArg(0, imgInf); 
                    render_kernel.setArg(1, imgStagef);
                    //render_kernel.setArg(2, imgOutf);
                    render_kernel.setArg(2, sizeof(dest_mat), &dest_mat);
                    auto re = ordered_queue.enqueueNDRangeKernel(render_kernel, in_bound);  
                    std::cout << re << "\n";
                } while (false);

            /*   // ordered_queue.enqueueCopyImage(imgOutf, imgStagef, in_bound);
                              
          
               // line draw
                for (auto&& item : lines) {
                    glm::vec2 p1 = { item[0], item[1] };
                    glm::vec2 p2 = { item[2], item[3] };
                    glm::vec2 a_vec = p2 - p1;
                    std::vector vec = { p1,p2 };

                    float r = 1.0f;
                    float w = r * 2;
                    float len = glm::length(a_vec);
                    glm::vec2 src[4] = {
                        {0.0f,        0.0f },
                        {len,         0.0f },
                        {len,         w  },
                        {0.0f,        w  }
                    };

                    glm::vec2 d_vec_unit = glm::vec2{ a_vec[1], -a_vec[0] } / len;

                    glm::vec2 dest[4] = {
                        p1 - d_vec_unit * r,
                        p2 - d_vec_unit * r,
                        p2 + d_vec_unit * r,
                        p1 + d_vec_unit * r
                    }; 

                    float color[4] = { 1.0f,0.0f,0.0f,1.0f};

                    auto src_mat = dr::core::getPerspectiveTransform(src, dest);
                    draw_lines_kernel.setArg(0, sizeof(p1), &p1);
                    draw_lines_kernel.setArg(1, sizeof(p2), &p2);
                    draw_lines_kernel.setArg(2, sizeof(r), &r);
                    draw_lines_kernel.setArg(3, sizeof(color), &color);
                    draw_lines_kernel.setArg(4, sizeof(src_mat), &src_mat);
                    draw_lines_kernel.setArg(5, imgStagef);
                    draw_lines_kernel.setArg(6, imgOutf); 

                    auto src_bound = dr::core::getBoundingRect(src);
                    auto in_src_bound = dr::core::intersection(src_bound, { 0, 0, width, height });
                    ordered_queue.enqueueNDRangeKernel(draw_lines_kernel, in_src_bound);

                    auto dest_bound = dr::core::getBoundingRect(dest);
                    auto in_dest_bound = dr::core::intersection(dest_bound, { 0, 0, width, height });
                    ordered_queue.enqueueCopyImage(imgOutf, imgStagef, in_dest_bound);
                }
                */
                auto mat = getPerspectiveTransform(src_list, dest_list);
                mat = glm::rotate(mat, 2.f * 3.14f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));

                for (int zz = 0; zz < 4; ++zz) {
                    auto&& src = src_list[zz];
                    glm::vec4 m1 = { src.x, src.y, 0.0f, 1.0f };
                    dest_list[zz] = mat * m1;
                }
            }
             {
                to_uint8_rgba_kernel.setArg(0, imgStagef);
                to_uint8_rgba_kernel.setArg(1, imgStage);
                ordered_queue.enqueueNDRangeKernel(to_uint8_rgba_kernel, { 0,0, width, height });

                //ordered_queue.enqueue_wait_marker();
                ordered_queue.enqueueReadImage(imgStage, CL_FALSE, dst.data());                
                ordered_queue.finish();
            }
        }
        
    } while (false);

    fpng::fpng_encode_image_to_file("result.png", dst.data(), width, height, 4);
    //std::cin.get();
}