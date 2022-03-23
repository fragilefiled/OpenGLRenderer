#pragma once

#include"Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    /*  函数   */
    Model(const char* path, bool drawinstance = false, vector<glm::mat4> modelMatrices=vector<glm::mat4>(),int instanceAmount=0):drawinstance(drawinstance),modelMatrices(modelMatrices),instanceAmount(instanceAmount)
    {
        loadModel(path);
    }
    
    
    void Draw(Shader& shader, vector<Texture> textures_RT = vector<Texture>(),CubeMap* cubemap=nullptr,vector<Texture3D> voxel_RT=vector<Texture3D>());
   
    vector<Mesh> meshes;
    ~Model();
private:
    /*  模型数据  */
    
    string directory;
    /*  函数   */
    vector<Texture> textures_loaded;
    vector<glm::mat4> modelMatrices;
  
    bool drawinstance;
    int  instanceAmount;
    
    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    void LoadInstance();
    void  CreateMesh();
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture::TextureType textureType);
    
};
