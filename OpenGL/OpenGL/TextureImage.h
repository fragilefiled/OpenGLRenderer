#pragma once
#include"Texture.h"

class TextureImage :public Texture 
{
public:
	TextureImage() {};
	TextureImage(int width,int height,int rw = 3,int id=-1, int nrChannels = 4) {
		this->id = id;
		this->width = width;
		this->height = height;
		this->nrChannels = nrChannels;
		this->rw = rw;
		
	}
	~TextureImage();
	std::vector<float>data;
	void Copy(const TextureImage& t);
	void Swap(TextureImage& t);
	std::vector<float> getData();
	void setData(std::vector<float> colorarray);
	 unsigned int Bind();
	int rw;
	
};