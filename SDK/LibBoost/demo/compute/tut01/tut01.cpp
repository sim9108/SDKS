#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <format>
#include <chrono>
#include <boost/compute.hpp>


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

constexpr std::size_t g_cnt = 100'000'000;

void compute_op() {
	namespace compute = boost::compute;
	namespace ranges = std::ranges;	

	std::vector<float> host_vector(g_cnt);
	ranges::generate(host_vector, rand);
	do {
		Timer time{ "\nboost.compute sort:" };

		compute::device gpu = compute::system::default_device();
		compute::context ctx{ gpu };
		compute::command_queue queue{ ctx, gpu };

		compute::vector<float> dev_vector{ g_cnt, ctx };
		compute::copy(
			std::begin(host_vector), std::end(host_vector),
			std::begin(dev_vector), queue
		);

		compute::sort(
			std::begin(dev_vector), std::end(dev_vector), queue
		);

		compute::copy(
			std::begin(dev_vector), std::end(dev_vector),
			std::begin(host_vector),
			queue
		);
	} while (false);

	std::cout << std::format("\nis sorted:{}", ranges::is_sorted(host_vector));
}

void normal_op() {
	namespace ranges = std::ranges;

	std::vector<float> host_vector(g_cnt);
	ranges::generate(host_vector, rand);

	do {
		Timer time{ "\nnormal sort:" };
		std::sort(
			std::begin(host_vector), std::end(host_vector)
		);
	} while (false);


	std::cout << std::format("\nis sorted:{}", ranges::is_sorted(host_vector));
}


int main() {
	compute_op();
	normal_op();
	return EXIT_SUCCESS;
}