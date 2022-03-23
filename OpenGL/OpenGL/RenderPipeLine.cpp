#include"RenderPipeLine.h"
RenderPipeLine::RenderPipeLine(vector<Model> models, vector<Shader> shaders, vector<glm::mat4> modelmats):models(models),shaders(shaders),modelmats(modelmats)
{
}

RenderPipeLine::~RenderPipeLine()
{
}

void RenderPipeLine::Draw()
{

}
