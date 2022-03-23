#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include<vector>
enum  Direction
{
	FORWARD,
	BACK,
	LEFT,
	RIGHT
};
 class Camera 
{
	  glm::vec3 pos;
	  glm::vec3 upDir;
	  glm::vec3 frontDir;
	  glm::mat4 view;
	  glm::mat4 projection;
	  float fov;
	  float cameraSpeed;
	  
public:
		 Camera(glm::vec3 pos,  glm::vec3 frontDir = glm::vec3(0, 0, -1),bool isPerspective=true,glm::vec3 upDir=glm::vec3(0,1,0),float fov=45.0f,float cameraSpeed=2.0f,int width=1024,int height=1024,float nearPlane=0.1f,float farPlane=100.f) :pos(pos),  upDir(upDir),frontDir(frontDir),fov(fov),cameraSpeed(cameraSpeed),width(width),height(height),nearPlane(nearPlane),farPlane(farPlane),isPerspective(isPerspective)
	  {
		 
		  view= glm::lookAt(pos,
			  pos+frontDir,
			  upDir);
	  };
		 Camera(glm::vec3 pos, glm::vec3 frontDir = glm::vec3(0, 0, -1),  glm::vec3 upDir = glm::vec3(0, 1, 0), bool isPerspective = true, float fov = 45.0f, float cameraSpeed = 2.0f, int width = 1024, int height = 1024, float nearPlane = 0.1f, float farPlane = 100.f) :pos(pos), upDir(upDir), frontDir(frontDir), fov(fov), cameraSpeed(cameraSpeed), width(width), height(height), nearPlane(nearPlane), farPlane(farPlane), isPerspective(isPerspective)
		 {

			 view = glm::lookAt(pos,
				 pos + frontDir,
				 upDir);
		 };
		 Camera();
		 int width;
		 int height;
		 float nearPlane;
		 float farPlane;
		 bool isPerspective;
		 glm::mat4 SetPos(glm::vec3 pos);
		 glm::vec3 GetPos();
		 glm::mat4 SetFrontDir(glm::vec3 frontDir);
		 glm::vec3 GetFrontDir();
		 glm::mat4 GetView();
		 glm::mat4 GetProjection();
		 glm::vec3& getVoxelMinPoint(glm::vec3 cameraPos, int voxelSize);
		 std::vector<glm::mat4> GetVoxelViewProjection(glm::vec3 cameraPos, int voxelsize);
		 std::vector<glm::mat4> GetVoxelViewProjectionInverse(glm::vec3 cameraPos, int voxelsize);
		 void Move(Direction direction, float deltaTime);
		 float GetFov();
		 void SetFov(float fov);

		 glm::mat4 CaculateReflectMatrix(glm::vec3 normal, glm::vec3 positionOnPlane);

};