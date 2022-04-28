#include"TextureImage.h"
#include <glad/glad.h>


TextureImage::~TextureImage()
{
	/*const GLuint a =id;
	glDeleteTextures(1, &a);*/
	auto temp = std::vector<float>(0);
		data.swap(temp);
}

void TextureImage::Copy(const TextureImage& t)
{
	this->id = t.id;
	this->width = t.width;
	this->height = t.height;
	this->nrChannels = t.nrChannels;
	this->rw = t.rw;
	
}


void TextureImage::Swap(TextureImage& t)
{
	TextureImage temp;
	temp.Copy(t);
	t.Copy(*this);
	this->Copy(temp);

}
std::vector<float> TextureImage::getData(bool enableupdate)
{
	
	if (data.size()==0||enableupdate) {
		data = std::vector<float>(width * height * 4);
		glBindTexture(GL_TEXTURE_2D, this->id);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data.data());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return data;
}
void TextureImage::setData(std::vector<float> colorarray)
{
	//data = colorarray;
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT,
		colorarray.data());
	glBindTexture(GL_TEXTURE_2D, 0);
	
}
unsigned int TextureImage::Bind()
{


	if (id != -1)
		return id;
	
	unsigned int texture;
	glGenTextures(1, &texture);
	allTextures.push_back(texture);
	format = GL_RGBA32F;
	readformat = GL_RGBA;
	id = texture;
	initid = id;
	//glGenTextures(1, &texture); //Ææ¹Ö
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT,
		NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/*if(rw==1)
	glBindImageTexture(idinCs, id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	else if(rw==2)
		glBindImageTexture(idinCs, id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	else
	{
		glBindImageTexture(idinCs, id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	}*/
	return texture;
}