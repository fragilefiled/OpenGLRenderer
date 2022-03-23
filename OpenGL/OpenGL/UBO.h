#pragma once
#include "ShaderCompile.h"
#include <string>
#include<vector>
class UBO
{
public:
	UBO(unsigned int size);
	~UBO();
	void SetUnifromBlockIndexAndBind( std::vector<Shader> shaders, std::string structName,int bindPos=0);
	void BindUniformBuffer();

	void SetMatrix(glm::mat4 matrix);

	void SetVec3(glm::vec3 value);

	void SetVec4(glm::vec4 value);

	void SetInt(const std::string& name, int value) ;
	
	void SizeValidate() {

		if (sizenow > size)
			std::cout << "size out of Range" << std::endl;
	}

	void ClearSizeNow() {
		sizenow = 0;
	}
	unsigned int uboID;
	std::vector<unsigned int> uniformBlockIndex;
	unsigned int size;
	GLsizeiptr sizenow=0;
	int bindPos;
private:

};

