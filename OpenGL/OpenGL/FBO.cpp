#include"FBO.h"



FBO::FBO(int width, int height, std::string name,FBOType fboType, GLenum format, GLenum format1, GLenum storeform, bool noRepeat):width(width),height(height),type(fboType),name(name), format(format), format1(format1), storeform(storeform),noRepeat(noRepeat)
{
	
	glGenFramebuffers(1, &fboBuffer);

	glGenTextures(1, &fboTexture);
	Texture::allTextures.push_back(fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	if (fboType == Depth) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	}
	
	if (fboType == Depth) 
	{

		glBindFramebuffer(GL_FRAMEBUFFER, fboBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fboTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		texture = Texture(Texture::Other,name, fboTexture, width, height, 1);
	}
	else 
	{

		glBindFramebuffer(GL_FRAMEBUFFER, fboBuffer);
		// create a color attachment texture

		glBindTexture(GL_TEXTURE_2D, fboTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format1, storeform, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		texture = Texture(Texture::Other, name, fboTexture, width, height, 4);
		
	}
	
}

void FBO::BindFrameBufferInit()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboBuffer);
}

void FBO::BindFrameBufeerOver()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


FBO::~FBO()
{
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1,&fboBuffer);
}
