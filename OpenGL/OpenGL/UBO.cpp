#include"UBO.h"
UBO::UBO(unsigned int size):size(size)
{
	glGenBuffers(1, &uboID);
}

UBO::~UBO()
{
	glDeleteBuffers(1, &uboID);
}

void UBO::SetUnifromBlockIndexAndBind( std::vector<Shader> shaders, std::string structName,int bindPos)
{
	this->bindPos = bindPos;
	uniformBlockIndex = std::vector<unsigned int>(shaders.size());
	for (int i = 0; i < shaders.size(); i++) {
		uniformBlockIndex[i] = glGetUniformBlockIndex(shaders[i].ID, structName.c_str());
		glUniformBlockBinding(shaders[i].ID, uniformBlockIndex[i], bindPos);
	}
	
}



void UBO::BindUniformBuffer() {

	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, bindPos, uboID, 0, size);
}
void UBO::SetMatrix(glm::mat4 matrix) 
{

	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	glBufferSubData(GL_UNIFORM_BUFFER, sizenow, sizeof(glm::mat4), glm::value_ptr(matrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	if(sizenow<size)
	sizenow += sizeof(glm::mat4);
	SizeValidate();
}
void UBO::SetVec3(glm::vec3 value)
{

	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	glBufferSubData(GL_UNIFORM_BUFFER, sizenow, sizeof(glm::vec3), glm::value_ptr(value));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	if (sizenow < size)
	sizenow += sizeof(glm::vec3);
	SizeValidate();

}
void UBO::SetVec4(glm::vec4 value)
{

	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	glBufferSubData(GL_UNIFORM_BUFFER, sizenow, sizeof(glm::vec4), glm::value_ptr(value));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	if (sizenow < size)
	sizenow += sizeof(glm::vec4);
	SizeValidate();

}
void UBO::SetInt(const std::string& name, int value) 
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	glBufferSubData(GL_UNIFORM_BUFFER, sizenow, sizeof(int), &value);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	if (sizenow < size)
	sizenow += sizeof(int);
	SizeValidate();
}
