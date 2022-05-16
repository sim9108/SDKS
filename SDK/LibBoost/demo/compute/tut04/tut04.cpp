// tut03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/compute/system.hpp>
#include <boost/compute/interop/opencv/core.hpp>
#include <boost/compute/interop/opencv/highgui.hpp>
#include <boost/compute/utility/source.hpp>

#include <chrono>

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

namespace compute = boost::compute;
const char source[] = BOOST_COMPUTE_STRINGIZE_SOURCE(
	//For out of boundary pixels, edge pixel value is returned
	const sampler_t sampler = CLK_ADDRESS_CLAMP_TO_EDGE |
	CLK_FILTER_NEAREST;
kernel void sobel_rgb(read_only image2d_t src, write_only image2d_t dst)
{
	int x = (int)get_global_id(0);
	int y = (int)get_global_id(1);

	if (x >= get_image_width(src) || y >= get_image_height(src))
		return;

	//  [(x-1, y+1), (x, y+1), (x+1, y+1)]
	//  [(x-1, y  ), (x, y  ), (x+1, y  )]
	//  [(x-1, y-1), (x, y-1), (x+1, y-1)]

	//  [p02, p12,   p22]
	//  [p01, pixel, p21]
	//  [p00, p10,   p20]

	//Basically finding influence of neighbour pixels on current pixel
	float4 p00 = read_imagef(src, sampler, (int2)(x - 1, y - 1));
	float4 p10 = read_imagef(src, sampler, (int2)(x, y - 1));
	float4 p20 = read_imagef(src, sampler, (int2)(x + 1, y - 1));

	float4 p01 = read_imagef(src, sampler, (int2)(x - 1, y));
	//pixel that we are working on
	float4 p21 = read_imagef(src, sampler, (int2)(x + 1, y));

	float4 p02 = read_imagef(src, sampler, (int2)(x - 1, y + 1));
	float4 p12 = read_imagef(src, sampler, (int2)(x, y + 1));
	float4 p22 = read_imagef(src, sampler, (int2)(x + 1, y + 1));

	//Find Gx = kernel + 3x3 around current pixel
	//           Gx = [-1 0 +1]     [p02, p12,   p22]
	//                [-2 0 +2]  +  [p01, pixel, p21]
	//                [-1 0 +1]     [p00, p10,   p20]
	float3 gx = -p00.xyz + p20.xyz +
		2.0f * (p21.xyz - p01.xyz)
		- p02.xyz + p22.xyz;

	//Find Gy = kernel + 3x3 around current pixel
	//           Gy = [-1 -2 -1]     [p02, p12,   p22]
	//                [ 0  0  0]  +  [p01, pixel, p21]
	//                [+1 +2 +1]     [p00, p10,   p20]
	float3 gy = p00.xyz + p20.xyz +
		2.0f * (-p12.xyz + p10.xyz) -
		p02.xyz - p22.xyz;
	//Find G
	float3 g = native_sqrt(gx * gx + gy * gy);

	// we could also approximate this as g = fabs(gx) + fabs(gy)
	write_imagef(dst, (int2)(x, y), (float4)(g.x, g.y, g.z, 1.0f));
}
);


int main() {
	do {
		cv::VideoCapture cap(0);

		cv::Mat cv_mat;
		cap >> cv_mat;
		if (!cv_mat.data) break;

		compute::device gpu = compute::system::default_device();
		compute::context  context{ gpu };
		compute::command_queue queue{ context, gpu };

		cv::cvtColor(cv_mat, cv_mat, cv::COLOR_BGR2BGRA);

		compute::image2d dev_input_image = compute::opencv_create_image2d_with_mat(
			cv_mat, compute::image2d::read_write, queue
		);

		compute::image2d dev_output_image(
			context,
			dev_input_image.width(),
			dev_input_image.height(),
			dev_input_image.format(),
			compute::image2d::write_only
		);

		compute::program optical_program = compute::program::create_with_source(source, context);
		optical_program.build();


		compute::kernel filter_kernel(optical_program, "sobel_rgb");

		filter_kernel.set_arg(0, dev_input_image);
		filter_kernel.set_arg(1, dev_output_image);

		std::size_t origin[2] = { 0,0 };
		size_t region[2] = { dev_input_image.width(),dev_input_image.height() };
		queue.enqueue_nd_range_kernel(filter_kernel, 2, origin, region, 0);

		char key = '\0';
		while (key != 27) {
			cap >> cv_mat;
			cv::cvtColor(cv_mat, cv_mat, cv::COLOR_BGR2BGRA);

			do {
				Timer ti{ "time:" };
				compute::opencv_copy_mat_to_image(cv_mat,
					dev_input_image,
					queue);
				queue.enqueue_nd_range_kernel(filter_kernel, 2, origin, region, 0);
				compute::opencv_imshow("Filtered RGB Frame", dev_output_image, queue);
			} while (false);

			cv::imshow("Camera Frame", cv_mat);			
			key = cv::waitKey(10);
		}

		return EXIT_SUCCESS;

	} while (false);

	std::cerr << "failed to capture frame" << std::endl;
	return EXIT_FAILURE;
}

