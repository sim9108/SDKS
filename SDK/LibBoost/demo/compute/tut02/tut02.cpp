#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/compute/system.hpp>
#include <boost/compute/interop/opencv/core.hpp>
#include <boost/compute/interop/opencv/highgui.hpp>
#include <boost/compute/utility/source.hpp>


int main(int argc, char* argv[]){
	namespace compute = boost::compute;
	do {
		if (argc < 2) {
			std::cerr << "usage:" << argv[0] << " FILENAME\n";break;			
		}
		cv::Mat cv_img = cv::imread(argv[1], cv::IMREAD_COLOR);
		
		if (!cv_img.data) {
			std::cerr << "failed to load image\n";break;
		}

		cv::cvtColor(cv_img, cv_img, cv::COLOR_BGR2BGRA);

		compute::device gpu = compute::system::default_device();
		compute::context context{ gpu };
		compute::command_queue queue{ context, gpu };

		compute::image2d input_img = compute::opencv_create_image2d_with_mat(cv_img, compute::image2d::read_write, queue);
		compute::image2d output_img { context, input_img.width(), input_img.height(), input_img.format(),compute::image2d::write_only};

		const char src[] = BOOST_COMPUTE_STRINGIZE_SOURCE(
			__kernel void flip_kernel(__read_only image2d_t input,
				__write_only image2d_t output)
		{
			const sampler_t sampler = CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;
			int height = get_image_height(input);
			int2 input_coord = { get_global_id(0), get_global_id(1) };
			int2 output_coord = { input_coord.x, height - input_coord.y - 1 };
			float4 value = read_imagef(input, sampler, input_coord);
			write_imagef(output, output_coord, value);
		}
		);

		compute::program flip_pro = compute::program::create_with_source(src, context);
		flip_pro.build();

		compute::kernel flip_kernel{ flip_pro, "flip_kernel" };
		flip_kernel.set_arg(0, input_img);
		flip_kernel.set_arg(1, output_img);

		size_t origin[2] = { 0, 0 };
		size_t region[2] = { input_img.width(), input_img.height() };
		queue.enqueue_nd_range_kernel(flip_kernel, 2, origin, region, 0);

		cv::imshow("opencv image", cv_img);
		compute::opencv_imshow("filtered image", output_img, queue);
		cv::waitKey(0);
		return EXIT_SUCCESS;
	} while (false);
	return EXIT_FAILURE;	
}
