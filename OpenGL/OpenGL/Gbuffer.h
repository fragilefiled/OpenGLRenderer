#pragma once
#include <glad\glad.h>
#include <iostream>
#include "Texture.h"
class Gbuffer
{
public:
	void setupFBO();
	Gbuffer(int width, int height);
	
	void BindFrameBufferOver();
	void BindFrameBufferInit();
	~Gbuffer();
	int width,height;
	GLuint  gDepth,gPosition, gNormal, gAlbedoSpec;
	GLuint rboDepth,gBuffer;;
	Texture gBufferTex[4];
private:

};

