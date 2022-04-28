#include"Wave_Particle_Pool.h"
#include"Mymath.h"
#include <iostream>
#include <glad\glad.h>


extern glm::vec2 rotate(glm::vec2 prev, float angle);
Wave_Particle_Pool::Wave_Particle_Pool(int count,int bound,bool enablehvfliter ):count(count),bound(bound),enablehvfliter(enablehvfliter)
{
	InitPointDraw();

	particles = std::vector<Wave_Particle>(count);
	enableArray= std::vector<bool>(count,false);
	threadPool = new ThreadPool(8);
	updatePool= std::function([this](int a, int b) { SinglePointUpdate(a, b); });
	//updatePool = std::bind([this](int a, int b) { SinglePointUpdate(a, b); }, 2, 2); 两种写法一样
}

void Wave_Particle_Pool::PushToNewParticles(glm::vec2 birthPos, glm::vec2 pos, glm::vec2 wave_speed, float amplitude, float radius, float degree, float time)
{
	int index = new_particle_size;
	particles[index].SetProperty(birthPos, pos, wave_speed, amplitude, radius, degree, time);
	new_particle_size++;
}

void Wave_Particle_Pool::PushToParticles(glm::vec2 birthPos, glm::vec2 pos, glm::vec2 wave_speed, float amplitude, float radius, float degree, float time)
{
	//int index = GetPosInParticles();
	int index = nowUse;
	nowUse++;
	particles[index].SetProperty( birthPos,  pos, wave_speed, amplitude,  radius,  degree,  time);
}

void Wave_Particle_Pool::Merge()
{
	for (int i = 0; i < new_particle_size; i++) {
		int index = GetPosInParticles();
		particles[index].SetProperty(new_particles[i]);
	}
	new_particle_size = 0;
}

void Wave_Particle_Pool::SinglePointUpdate(int a,int b) 
{


	
	for (int j = a; j < b; j++) {
		//int i = indexArray[j];
		int i = j;
		float radians = particles[i].degree / 2.0f / 180.0f * PI;
		float distance = 0;
		if (particles[i].degree == 360)
			distance = 2 * glm::length(particles[i].pos - particles[i].birthPos);
		else
			distance = glm::length(particles[i].pos - particles[i].birthPos) * sin(radians) * 2;
		
		if (!enablehvfliter)
		{
			if (particles[i].pos.x >= bound) {
				particles[i].wave_speed.x *= -1;
				particles[i].pos.x = bound;
			}
			if (particles[i].pos.x <= 0) {
				particles[i].wave_speed.x *= -1;
				particles[i].pos.x = 0;
			}
			if (particles[i].pos.y >= bound) {
				particles[i].wave_speed.y *= -1;
				particles[i].pos.y = bound;
			}
			if (particles[i].pos.y <= 0) {
				particles[i].wave_speed.y *= -1;
				particles[i].pos.y = 0;
			}
		}
		else 
		{

			if (particles[i].pos.x >= bound - 2 * particles[i].radius) {
				particles[i].wave_speed.x *= -1;
				particles[i].pos.x = bound - 2 * particles[i].radius;
			}
			if (particles[i].pos.x <= 2 * particles[i].radius) {
				particles[i].wave_speed.x *= -1;
				particles[i].wave_speed.x = abs(particles[i].wave_speed.x);
				particles[i].pos.x = 2 * particles[i].radius;
			}
			if (particles[i].pos.y >= bound - 2 * particles[i].radius) {
				particles[i].wave_speed.y *= -1;
				particles[i].pos.y = bound - 2 * particles[i].radius;
			}
			if (particles[i].pos.y <= 2 * particles[i].radius) {
				particles[i].wave_speed.y *= -1;
				particles[i].wave_speed.y = abs(particles[i].wave_speed.y);
				particles[i].pos.y = 2 * particles[i].radius;
			}
		}

		//if (distance>particles[i].radius/2.0f&&nowUse<2047)
		//{
		//	glm::vec2 dir = glm::normalize(particles[i].wave_speed);
		//	glm::vec2 wavespeed1 = rotate(glm::normalize(dir), -particles[i].degree / 2.0f) * glm::length(particles[i].wave_speed);
		//	glm::vec2 wavespeed2 = rotate(glm::normalize(dir), particles[i].degree / 2.0f) * glm::length(particles[i].wave_speed);
		//	PushToParticles(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed1, wavespeed1, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time);
		//	PushToParticles(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed2, wavespeed2, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time);
		//	/*PushToNewparticles(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed1, wavespeed1, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time);
		//	PushToNewparticles(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed2, wavespeed2, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time);*/
		//	particles[i].count = 0;
		//	particles[i].amplitude /= 3.0f;
		//	particles[i].degree /= 3.0f;
		//}
		//if (particles[i].time > 400.0f) {
		//	particles[i].enable = false;
		//	enableArray[i] = false;
		//} //will slow down cpu
		//else {
		//
		//	//indexArray_end.push_back(i);
		//
		//}
		particles[i].time += 0.1f * 0.3333f;
		particles[i].pos += 0.1f * 0.3333f * particles[i].wave_speed;

		particles[i].count++;
		
	
			

	}
	

	

}
void Wave_Particle_Pool::ExPand() {

	//for (int i = 0; i < particles.size(); i++)
	//{
	//	/*if (enableArray[i] == false)
	//		continue;*/
	//	
	//	if (particles[i].count > 250) 
	//	{
	//		glm::vec2 dir = glm::normalize(particles[i].wave_speed);
	//		glm::vec2 wavespeed1 = rotate(glm::normalize(dir), -particles[i].degree / 2.0f) * glm::length(particles[i].wave_speed);
	//		glm::vec2 wavespeed2 = rotate(glm::normalize(dir), particles[i].degree / 2.0f) * glm::length(particles[i].wave_speed);
	//		particles.push_back(Wave_Particle(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed1, wavespeed1, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time));
	//		particles.push_back(Wave_Particle(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed2, wavespeed2, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time));
	//		/*PushToNewParticles(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed1, wavespeed1, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time);
	//		PushToNewParticles(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed2, wavespeed2, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time);*/
	//		particles[i].count = 0;
	//		particles[i].amplitude /= 3.0f;
	//		particles[i].degree /= 3.0f;
	//	}
	//	if (particles[i].time > 20.0f) {
	//		particles[i].enable = false;
	//	}
	//	particles[i].time += 0.1f;
	//	particles[i].pos += 0.1f * particles[i].wave_speed;

	//	particles[i].count++;
	//}

//	{
//		omp_set_num_threads(4);
//#pragma omp parallel
//		{
//#pragma omp for
//			for (int j = 0; j < indexArray.size(); j++)
//			{
//
//				/*if (enableArray[i] == false)
//					continue;*/
//				int i = indexArray[j];
//				float radians = particles[i].degree / 2.0f / 180.0f * PI;
//				float distance = 0;
//				if (particles[i].degree == 360)
//					distance = 2 * glm::length(particles[i].pos - particles[i].birthPos);
//				else
//					distance = glm::length(particles[i].pos - particles[i].birthPos) * sin(radians) * 2;
//				if (!enablehvfliter)
//				{
//					if (particles[i].pos.x >= bound) {
//						particles[i].wave_speed.x *= -1;
//						particles[i].pos.x = bound;
//					}
//					if (particles[i].pos.x <= 0) {
//						particles[i].wave_speed.x *= -1;
//						particles[i].pos.x = 0;
//					}
//					if (particles[i].pos.y >= bound) {
//						particles[i].wave_speed.y *= -1;
//						particles[i].pos.y = bound;
//					}
//					if (particles[i].pos.y <= 0) {
//						particles[i].wave_speed.y *= -1;
//						particles[i].pos.y = 0;
//					}
//				}
//				else {
//
//					if (particles[i].pos.x >= bound - 2 * particles[i].radius) {
//						particles[i].wave_speed.x *= -1;
//						particles[i].pos.x = bound - 2 * particles[i].radius;
//					}
//					if (particles[i].pos.x <= 2 * particles[i].radius) {
//						particles[i].wave_speed.x *= -1;
//						particles[i].pos.x = 2 * particles[i].radius;
//					}
//					if (particles[i].pos.y >= bound - 2 * particles[i].radius) {
//						particles[i].wave_speed.y *= -1;
//						particles[i].pos.y = bound - 2 * particles[i].radius;
//					}
//					if (particles[i].pos.y <= 2 * particles[i].radius) {
//						particles[i].wave_speed.y *= -1;
//						particles[i].pos.y = 2 * particles[i].radius;
//					}
//				}
//
//				//if (distance>particles[i].radius/2.0f&&indexArray.size()<2047)
//				//{
//				//	glm::vec2 dir = glm::normalize(particles[i].wave_speed);
//				//	glm::vec2 wavespeed1 = rotate(glm::normalize(dir), -particles[i].degree / 2.0f) * glm::length(particles[i].wave_speed);
//				//	glm::vec2 wavespeed2 = rotate(glm::normalize(dir), particles[i].degree / 2.0f) * glm::length(particles[i].wave_speed);
//				//	PushToParticles(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed1, wavespeed1, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time);
//				//	PushToParticles(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed2, wavespeed2, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time);
//				//	/*PushToNewParticles(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed1, wavespeed1, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time);
//				//	PushToNewParticles(particles[i].birthPos, particles[i].birthPos + particles[i].time * wavespeed2, wavespeed2, particles[i].amplitude / 3.0f, particles[i].radius, particles[i].degree / 3.0f, particles[i].time);*/
//				//	particles[i].count = 0;
//				//	particles[i].amplitude /= 3.0f;
//				//	particles[i].degree /= 3.0f;
//				//}
//				if (particles[i].time > 400.0f) {
//					particles[i].enable = false;
//					enableArray[i] = false;
//				}
//				else
//					;// indexArray_end.push_back(i);//非常耗费资源
//				particles[i].time += 0.1f * 0.3333f;
//				particles[i].pos += 0.1f * 0.3333f * particles[i].wave_speed;
//
//				particles[i].count++;
//
//			}
//		}
//	}
	if (init == false) 
	{
		
		
		//for (int i = 0; i < indexArray.size(); i+=(indexArray.size())/4) {
		//	
	
		//	threadPool->enqueue(updatePool,i,i+ (indexArray.size()) / 4);
		//	
		//	
		//	
		//}//Update Pool
		for (int i = 0; i < nowUse; i += (nowUse) / 4) {


			threadPool->enqueue(updatePool, i, i + (nowUse) / 4);



		}//Update Pool
	
	}
  
	indexArray_out.clear();
	indexArray_out = indexArray;
	//indexArray.clear();
	//indexArray = indexArray_end;
	//indexArray_end.clear();
	//for (int a = 0; a < 32; a++)
	//	for (int b = 0; b < 32; b++) 
	//	{
	//		float d = 0;

	//		for (int i = 0; i < particles.size(); i++)
	//		{
	//			
	//			glm::vec2 pos = particles[i].pos;
	//			float a0 = particles[i].amplitude;
	//			float r = particles[i].radius;
	//			float temp = abs(sqrtf((b - pos.x) * (b - pos.x) + (a - pos.y) * (a - pos.y))) / r;
	//			int c = 0;
	//			if (temp / 2 < 0.5) {
	//				c = 1;
	//				//cout << a<<" " << b << " " << i << " " << j <<" "<<temp<< endl;
	//			}

	//			d += (a0 / 2.0f) * (cos((PI * temp)) + 1) * c * 1;
	//		}

	//		new_wave[128 * b + a * 4 + 1] = (d);
	//		//  cout << a << " " << b << " " << new_wave[128 * b + a * 4 + 2] << endl;
	//	}



}



void Wave_Particle_Pool::InitPointDraw()
{
	
	glGenVertexArrays(1, &pointVAO);
	glGenBuffers(1, &pointVBO);
	glBindVertexArray(pointVAO);
	//定义数据

	glPointSize(5);

	float vertex[] = { 0.0, 0.0, 0.7, 0.8,1,1,0.0, 0.0, 0.7, 0.8,1,1 };
	//绑定 buffer 对象
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), (vertex), GL_STATIC_DRAW);

	//开启顶点属性
	glEnableVertexAttribArray(0);
	//指定属性使用的 buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//开启顶点属性
	glEnableVertexAttribArray(1);
	//指定属性使用的 buffer
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
}
void Wave_Particle_Pool::UpdatePoints(std::vector<float> points)
{
	glBindVertexArray(pointVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(float), points.data(), GL_DYNAMIC_DRAW);
	//开启顶点属性
	glEnableVertexAttribArray(0);
	//指定属性使用的 buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//开启顶点属性
	glEnableVertexAttribArray(1);
	//指定属性使用的 buffer
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
}
int Wave_Particle_Pool::GetPosInParticles()
{
	if (min_enable_pos < count * count * 4) {
		enableArray[indexArray.size()] = true;
		min_enable_pos = indexArray.size();
		indexArray.push_back(indexArray.size());
		return indexArray.size()-1;
	}
	else {
		for (int i = 0; i < enableArray.size(); i++)
			if (enableArray[i] == false) {
				enableArray[i] = true;
				indexArray.push_back(i);
				min_enable_pos = i;
				return i;
			}
	}
}

Wave_Particle_Pool::~Wave_Particle_Pool()
{
	delete threadPool;
}
