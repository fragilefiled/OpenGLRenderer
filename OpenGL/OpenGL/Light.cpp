#include"Light.h"
using namespace std;
int Light::pointLightAll = 0;


void Light::BindModel()
{
    
    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);

  
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW); //存储所有顶点数据的内存 
  
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);//glEnableVertexAttribArray启用index指定的通用顶点属性数组
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);//glEnableVertexAttribArray启用index指定的通用顶点属性数组  
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);//glEnableVertexAttribArray启用index指定的通用顶点属性数组  
}
void Light::SetLight(Shader& shader)
{
    camera.SetPos(pos);
    camera.SetFrontDir(lightDir);
    if (this->lightType == SPOTLIGHT) 
    {
        shader.setVec4("spotLight.lightColor", color);
        shader.setVec3("spotLight.lightPos", pos);
        shader.setVec3("spotLight.lightDir", lightDir);
        shader.setFloat("spotLight.cutOff", cutOffAngle);
       
    }
    else if (this->lightType == POINTLIGHT) 
    {
       char count = '0' + pointLightCount;
       string pointLightArray = ("pointLights");
        pointLightArray= pointLightArray +"[" + count + "]";
        string propertyName[4] = { (".lightproperty") ,".lightColor" ,".lightPos",".lightDir" };

        shader.setVec3(pointLightArray +propertyName[0],lightProperty);
        shader.setVec4(pointLightArray + propertyName[1], color);
        shader.setVec3(pointLightArray +propertyName[2], pos);
        shader.setVec3(pointLightArray +propertyName[3], lightDir);
        
    }
    else 
    {
        shader.setVec4("dirLight.lightColor", color);
        shader.setVec3("dirLight.lightPos", pos);
        shader.setVec3("dirLight.lightDir", lightDir);
      // shader.setMatrix("lightMat", (camera.GetProjection() * camera.GetView()));
    }
   

}

Light::~Light()
{
    glDeleteVertexArrays(1, &LightVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
