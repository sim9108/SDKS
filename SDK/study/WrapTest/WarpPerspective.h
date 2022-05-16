#pragma once
#include <glm/glm.hpp>

class WarpPerspective
{
public:
	static glm::mat4 getPerspectiveTransform(const glm::vec2 (&src)[4], const glm::vec2 (&dst)[4]);
private:
	static void gaussianElimination(float(&input)[8][9]);
};