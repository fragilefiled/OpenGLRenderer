#pragma once
#include"Wave_Particle.h"
#include <vector>
#include"Mymath.h"
class Wave_Particle_Pool
{
public:
	std::vector<Wave_Particle> particles;
	std::vector<bool> enableArray;
	std::vector<int> indexArray;
	std::vector<int> indexArray_out;
	std::vector<int> indexArray_end;
	Wave_Particle_Pool(int count,int bound,bool enablehvfliter=false);
	std::vector<Wave_Particle> new_particles;
	int new_particle_size = 0;
	int min_enable_pos = 0;
	int count = 0;
	int bound = 0;
	unsigned int pointVAO, pointVBO;
	bool enablehvfliter;
	void PushToNewParticles(glm::vec2 birthPos, glm::vec2 pos, glm::vec2 wave_speed, float amplitude, float radius, float degree, float time);
	void PushToParticles(glm::vec2 birthPos, glm::vec2 pos, glm::vec2 wave_speed, float amplitude, float radius, float degree, float time);
	void Merge();
	void ExPand();
	void InitPointDraw();
	void UpdatePoints(std::vector<float> points);
	int GetPosInParticles();
private:
	
};
