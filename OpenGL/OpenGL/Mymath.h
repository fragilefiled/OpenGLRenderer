#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define PI 3.14159265358979323846
//.cpp
//.h


inline glm::vec2 rotate(glm::vec2 prev,float angle) 
{
	float radians = angle / 180.0f * PI;
	float x = prev.x * cos(radians) - prev.y * sin(radians);
	float y = prev.x * sin(radians) + prev.y * cos(radians);
	return glm::vec2(x, y);
}