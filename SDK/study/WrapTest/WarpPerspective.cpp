#include "WarpPerspective.h"

	//--------------------------------------------------------------
	// Adapted from: http://forum.openframeworks.cc/t/quad-warping-homography-without-opencv/3121/19
	glm::mat4 WarpPerspective::getPerspectiveTransform(const glm::vec2 (&src)[4], const glm::vec2 (&dst)[4]){
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

		WarpPerspective::gaussianElimination(p);

		return glm::mat4(p[0][8], p[3][8], 0, p[6][8], 
						 p[1][8], p[4][8], 0, p[7][8], 
						 0,       0,       1, 0, 
						 p[2][8], p[5][8], 0, 1);
	}
	
	void WarpPerspective::gaussianElimination(float (&input)[8][9]) {
		auto i = 0;
		auto j = 0;
		constexpr int n = 9;
		constexpr auto m = n - 1;

		while (i < m && j < n) {
			auto iMax = i;
			for (auto k = i + 1; k < m; ++k){
				if (fabs(input[k][j]) > fabs(input[iMax][j])) {
					iMax = k;
				}
			}

			if (input[iMax * n + j] != 0) {
				if (i != iMax){
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

		for (auto i = m - 2; i >= 0; --i){
			for (auto j = i + 1; j < n - 1; ++j){
				input[i][m] -= input[i][j] * input[j][m];
			}
		}
	}

