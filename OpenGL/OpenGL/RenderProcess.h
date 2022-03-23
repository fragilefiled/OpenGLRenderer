#pragma once
#include"Camera.h"
class RenderProcess 
{
public:
	RenderProcess() {};
	virtual void Init()=0;
	virtual void Process() = 0;
	Camera camera;
	float Time = 0.0f;
	virtual void SetCameraAndTime(Camera& camera, float time) =0;
};