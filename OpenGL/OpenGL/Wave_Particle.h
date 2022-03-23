#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Wave_Particle
{
public:
	Wave_Particle(glm::vec2 birthPos,
		glm::vec2 pos,
		glm::vec2 wave_speed,
		float  amplitude,
		float  radius,float degree,float time=0.0);
	glm::vec2 pos;
	glm::vec2 birthPos;
	glm::vec2 wave_speed;
	float time;
	float  amplitude;
	float  radius;
	float degree;
	int count = 0;
	bool enable = true;
	Wave_Particle();
	void SetProperty(glm::vec2 birthPos,
		glm::vec2 pos,
		glm::vec2 wave_speed,
		float  amplitude,
		float  radius, float degree,float time);
	void SetProperty(Wave_Particle object);
private:

};



