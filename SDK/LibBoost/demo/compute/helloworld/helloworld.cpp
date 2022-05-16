#include <iostream>
#include <format>
#include <boost/compute/core.hpp>

int main() {
	namespace compute = boost::compute;

	std::vector<compute::platform> pfs = compute::system::platforms();
	std::cout << "platforms list:\n";
	for (auto&& item : pfs) {
		std::cout << item.name() << '\n';
	}
	std::cout << "\n";

	compute::device dev= compute::system::default_device();
	std::cout << "default device:\n";
	std::cout << std::format(
		"device name:{}\n"
		"platform name:{}\n",
		dev.name(),
		dev.platform().name()
	);


	return EXIT_SUCCESS;
}