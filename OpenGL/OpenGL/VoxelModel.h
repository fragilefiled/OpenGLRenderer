#pragma once
#include"Model.h"
class VoxelModel
{
public:
	VoxelModel(int pointNum);
	~VoxelModel();
	void Draw(Shader& shader, vector<Texture3D> &voxel_RT);
	unsigned int VAO;
	unsigned int VBO;
	std::vector<float> pointArray;
	int pointNum = 0;
private:

};

