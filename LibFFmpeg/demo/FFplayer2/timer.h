#pragma once

#include <cstdint>

class Timer {
private:
	int64_t target_time_;

	int64_t proportional_{};
	int64_t integral_{};
	int64_t derivative_{};

	static double P_;
	static double I_;
	static double D_;

public:
	Timer();
	void wait(int64_t period);
	void update();

private:
	int64_t adjust() const;
};
