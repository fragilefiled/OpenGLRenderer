#include"Wave_Particle.h"

Wave_Particle::Wave_Particle(glm::vec2 birthPos, glm::vec2 pos, glm::vec2 wave_speed, float amplitude, float radius,float degree,float time):birthPos(birthPos),pos(pos),wave_speed(wave_speed),amplitude(amplitude),radius(radius),degree(degree),time(time)
{
	
}

Wave_Particle::Wave_Particle()
{
	this->birthPos = glm::vec2(0,0);
	this->pos = glm::vec2(0, 0);
	this->wave_speed = glm::vec2(0, 0);
	this->amplitude = 0;
	this->radius = 0;
	this->degree = 0;
	this->time = 0;
}

void Wave_Particle::SetProperty(glm::vec2 birthPos, glm::vec2 pos, glm::vec2 wave_speed, float amplitude, float radius, float degree,float time)
{
	this->birthPos = birthPos;
	this->pos = pos;
	this->wave_speed = wave_speed;
	this->amplitude = amplitude;
	this->radius = radius;
	this->degree = degree;
	this->time = time;
}
void Wave_Particle::SetProperty(Wave_Particle object)
{
	this->birthPos = object.birthPos;
	this->pos = object.pos;
	this->wave_speed = object.wave_speed;
	this->amplitude = object.amplitude;
	this->radius = object.radius;
	this->degree = object.degree;
	this->time = object.time;
}

