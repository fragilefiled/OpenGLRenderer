#include "CubeMap.h"
#include <glad/glad.h>

CubeMap::CubeMap(std::vector<std::string> faces)
{
  

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        allData.push_back(data);
        

    }
    this->faces = faces;
    
}

unsigned int CubeMap::Bind() 
{

    unsigned int id;
    glGenTextures(1, &id);
    Texture::allTextures.push_back(id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    for (unsigned int i = 0; i < allData.size(); i++)
    {
        unsigned char* data = allData[i];
        if (data)
        {
            
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
            
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    BindModel();
    this->id = id;



    return id;
}
void CubeMap::Draw(Shader shader, std::string name, int i) {
    
        shader.use();
        glActiveTexture(GL_TEXTURE0 + i);
        shader.setInt(name, i);
        nameInShader = name;
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
        glBindVertexArray(CubeMapVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    
}
void CubeMap::BindModel() {

    glGenVertexArrays(1, &CubeMapVAO);
    glBindVertexArray(CubeMapVAO);

  
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW); //存储所有顶点数据的内存 
 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);//glEnableVertexAttribArray启用index指定的通用顶点属性数组






}


CubeMap::~CubeMap()
{
    glDeleteVertexArrays(1, &CubeMapVAO);
    glDeleteBuffers(1, &VBO);
    

}


