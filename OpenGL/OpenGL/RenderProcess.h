#pragma once
#include"Camera.h"
#include"string"
class RenderProcess 
{
public:
	RenderProcess() {};
	virtual void Init()=0;
	virtual void Process() = 0;
	Camera camera;
	float Time = 0.0f;
	float timeFrame = 0;
	int countFrame=0;
	float deltaTime;
	int frameNow = 0;
	virtual void SetCameraAndTime(Camera& camera, float time) =0;
	virtual void CalculateDeltaTime() = 0;
	void CalcAvgDeltaTime(std::string name,float count=60.0) {


		timeFrame += deltaTime;
		countFrame++;
		if (countFrame == count) {
			timeFrame /= count;
			std::cout << 1000 * timeFrame << name << std::endl;
			timeFrame = 0.0;
			countFrame = 0;

			
		}

	}
};