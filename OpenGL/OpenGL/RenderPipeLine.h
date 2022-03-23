#pragma once
#include "Model.h"
class RenderPipeLine
{

public:
	RenderPipeLine(vector<Model> models, vector<Shader> shaders, vector<glm::mat4> modelmats);
	~RenderPipeLine();
	vector<Model> models;
	vector<Shader> shaders;
	vector<glm::mat4> modelmats;
	void Draw();
private:

};

