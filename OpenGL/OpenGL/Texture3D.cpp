#include"Texture3D.h"
#include <glad/glad.h>


Texture3D::~Texture3D()
{
	/*const GLuint a =id;
	glDeleteTextures(1, &a);*/
}

void Texture3D::Copy(const Texture3D& t)
{
	this->id = t.id;
	this->width = t.width;
	this->height = t.height;
	this->nrChannels = t.nrChannels;
	this->rw = t.rw;

}


void Texture3D::Swap(Texture3D& t)
{
	Texture3D temp;
	temp.Copy(t);
	t.Copy(*this);
	this->Copy(temp);

}
std::vector<float> Texture3D::getData()
{

	if (data.size() == 0) {
		data = std::vector<float>(width * height*depth * 4);
		glBindTexture(GL_TEXTURE_3D, this->id);
		glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_FLOAT, data.data());
		glBindTexture(GL_TEXTURE_3D, 0);
	}
	return data;
}
void Texture3D::setData(std::vector<float> colorarray)
{
	data = colorarray;
	glBindTexture(GL_TEXTURE_3D, this->id);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, getWidth(), getHeight(),getDepth(), 0, GL_RGBA, GL_FLOAT,
		colorarray.data());
	glBindTexture(GL_TEXTURE_3D, 0);

}
unsigned int Texture3D::Bind()
{


	if (id != -1)
		return id;

	unsigned int texture;
	glGenTextures(1, &texture);
	allTextures.push_back(texture);
	//format = GL_RGBA32F;
	//readformat = GL_RGBA;
	id = texture;
	initid = id;
	//glGenTextures(1, &texture); //Ææ¹Ö
	glBindTexture(GL_TEXTURE_3D, id);
	glTexImage3D(GL_TEXTURE_3D, 0, format, getWidth(), getHeight(),getDepth(), 0, readformat, datatype,
		NULL);

	glGenerateMipmap(GL_TEXTURE_3D);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL,5);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/*glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL,1);*/
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP, GL_TRUE);
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

int Texture3D::getDepth()
{
	return this->depth;
}
void Texture3D::Clear()
{
	glBindTexture(GL_TEXTURE_3D, this->id);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, getWidth(), getHeight(), getDepth(), 0, GL_RGBA, GL_FLOAT,
		nullptr);
	glBindTexture(GL_TEXTURE_3D, 0);
}
