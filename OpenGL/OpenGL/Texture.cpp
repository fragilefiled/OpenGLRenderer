#include "Texture.h"
#include <glad/glad.h>
std::vector<int> Texture::allTextures;

Texture::Texture(const char* path,TextureType Type,int id)
{
	this->path = path;
	this->id = id;
//s	stbi_set_flip_vertically_on_load(true);
	this->type = Type;
	 data = stbi_load((this->path).c_str(), &width, &height, &nrChannels, 0);
}
Texture::Texture(TextureType Type)
{
}
Texture::Texture(TextureType Type,std::string nameInShader ,int id, int width, int height, int nrChannels):type(Type),id(id),width(width),height(height),nrChannels(nrChannels),nameInShader(nameInShader)
{
}
int Texture::getWidth()
{
	return this->width;
}
int Texture::getHeight()
{
	return this->height;
}

int Texture::getChannels()
{
	return this->nrChannels;
}

unsigned char* Texture::getData()
{
	if (data == nullptr) {
		glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, data);
	}
	return data;
}

void Texture::setData(std::vector<float> colorarray) {

	
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT,
		colorarray.data());
	glBindTexture(GL_TEXTURE_2D, 0);

}
void Texture::Free()
{
	
}
void Texture::Clear() 
{
	glBindTexture(GL_TEXTURE_2D, id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, format, getWidth(), getHeight(), 0, format, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::InitId()
{
	this->id = initid;
}
unsigned int Texture::Bind()
{
	//if (data == nullptr)
		if(id!=-1)
		return id;
	
	unsigned int texture;
	glGenTextures(1, &texture);
	allTextures.push_back(texture);
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	else
		format = GL_RGBA;
	readformat = format;
	id = texture;
	initid = id;
	//glGenTextures(1, &texture); //Ææ¹Ö
	glBindTexture(GL_TEXTURE_2D, id);
	if(type==Diffuse&&nrChannels==4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, getWidth(), getHeight(), 0, format, GL_UNSIGNED_BYTE, getData());
	else if(type == Diffuse && nrChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, getWidth(), getHeight(), 0, format, GL_UNSIGNED_BYTE, getData());
	else
	glTexImage2D(GL_TEXTURE_2D, 0, format, getWidth(), getHeight(), 0, format, GL_UNSIGNED_BYTE, getData());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);
	return texture;
}

std::string Texture::GetNameInShader(int index)
{
	switch (type)
	{
	case Texture::Diffuse:
		nameInShader = "texture_Diffuse_"+ std::to_string(index);
		
		break;
	case Texture::Normal:
		nameInShader = "texture_Normal_" + std::to_string(index);
		
		break;
	case Texture::Specular:
		nameInShader = "texture_Specular_" + std::to_string(index);
	
		break;
	case Texture::Other:
		nameInShader = "texture_Other_" + std::to_string(index);
		
		break;
	default:
		break;
	}
	return nameInShader;
}

Texture::~Texture()
{
	
	/*const GLuint a= id;
	glDeleteTextures(1, &a);*/
	//stbi_image_free(data);
}

