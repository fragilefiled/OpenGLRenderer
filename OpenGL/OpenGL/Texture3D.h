#pragma once
#pragma once
#include"Texture.h"

class Texture3D :public Texture
{
public:
	Texture3D() {};
	Texture3D(int width, int height,int depth, bool enableTextureImage = false, GLenum internalformat=GL_RGBA32F, GLenum format = GL_RGBA,GLenum datatype=GL_FLOAT,int rw = 3, int id = -1, int nrChannels = 4) {
		this->id = id;
		this->width = width;
		this->height = height;
		this->depth = depth;
		this->nrChannels = nrChannels;
		this->rw = rw;
		this->enableTextureImage = enableTextureImage;
		this->format = internalformat;
		this->readformat = format;
		this->datatype = datatype;
	}
	~Texture3D();
	std::vector<float>data;
	void Copy(const Texture3D& t);
	void Swap(Texture3D& t);
	std::vector<float> getData();
	void setData(std::vector<float> colorarray);
	unsigned int Bind();
	int getDepth();
	void Clear();
	int depth;
	int rw;
	bool enableTextureImage;
};