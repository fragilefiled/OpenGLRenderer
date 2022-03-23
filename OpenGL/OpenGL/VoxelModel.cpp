#include"VoxelModel.h"
VoxelModel::VoxelModel(int pointNum)
{
	this->pointNum = pointNum;
	pointArray = vector<float>(3 * pointNum, 0);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	//定义数据

	glPointSize(5);
	float* vertexArray = pointArray.data();

	//绑定 buffer 对象
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3*pointNum*sizeof(float), (vertexArray), GL_STATIC_DRAW);

	//开启顶点属性
	glEnableVertexAttribArray(0);
	//指定属性使用的 buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	auto a = vector<float>(0);
	pointArray.swap(a);
}

VoxelModel::~VoxelModel()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

}



 void VoxelModel::Draw(Shader &shader,vector<Texture3D>& voxel_RT)
{

	 for (int i = 0; i < voxel_RT.size(); i++)
	 {

		 glActiveTexture(GL_TEXTURE0 + i);
		 shader.setInt(voxel_RT[i].nameInShader, i + 0);
		 glBindTexture(GL_TEXTURE_3D, voxel_RT[i].id);
		 if (voxel_RT[i].enableTextureImage)
			 glBindImageTexture(i, voxel_RT[i].id, 0, GL_FALSE, 0, GL_READ_WRITE, voxel_RT[i].format);

	 }

	glPointSize(1);

	glBindVertexArray(VAO);
	//绘制顶点 
	glDrawArrays(GL_POINTS, 0, pointNum);
}
