#pragma once
#include<iostream>
#include "stb_image.h"
#include <string>
#include <glad/glad.h>
#include <vector>

class Texture {
public:
	static std::vector<int> allTextures;
public:
	int id;
	int initid;
	GLenum format;
	GLenum readformat;
	GLenum datatype;
	std::string path;
	GLenum filterType;
	 enum TextureType
	{
		Diffuse,
		Normal,
		Specular,
		Other,
		ComputeShader
	};
	 
	 TextureType type;
	
	 Texture(const char* path, TextureType Type = Diffuse, int id = -1);
	 Texture(TextureType Type = Diffuse);
	 Texture(TextureType Type,std::string nameInShader ,int id,int width,int height,int nrChannels=4);
	int getWidth();
	int getHeight();
	int getChannels();
	unsigned char* getData();
	void setData(std::vector<float> colorarray);
	void Free();
	void Clear();
	void InitId();
	
	unsigned int Bind();
	std::string GetNameInShader(int index);
	~Texture();
	std::string nameInShader;
protected:
	int width, height, nrChannels;
	unsigned char* data;
	
private:
	
	
	
	




	

};