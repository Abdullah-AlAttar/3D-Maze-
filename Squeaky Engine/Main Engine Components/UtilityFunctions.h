#pragma once

#include <glm\glm.hpp>

class UtilityFunctions
{
public:
	static glm::vec3 IndicesToWorldCoordiantes(glm::vec2 index,int width,int height)
	{
		return glm::vec3(-width / 2 + index.x * 2, 1, height / 2 + index.y * 2);
	}
};