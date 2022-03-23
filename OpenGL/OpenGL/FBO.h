#pragma once
#include <glad/glad.h>
#include <cstddef>
#include "Texture.h"
#include <string>
class FBO
{
public:
	 enum FBOType
	{
		Depth,
		Color
	};
	FBO(int width, int height, std::string name,FBOType fboType=Color, GLenum format = GL_RGB, GLenum format1 = GL_RGB, GLenum storeform = GL_UNSIGNED_BYTE, bool noRepeat=true);
	void BindFrameBufferInit();
	void BindFrameBufeerOver();
	~FBO();
	int width, height;	 
	unsigned int fboBuffer;
	unsigned int fboTexture;
	unsigned int rbo;
	GLenum format;
	GLenum format1;
	GLenum storeform;
	bool msaaEnable;
	std::string name;
	Texture texture;
	FBOType type;
	bool noRepeat;
private:

};

