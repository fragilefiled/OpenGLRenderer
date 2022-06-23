#pragma once

#include"Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Camera.h"
#include <GLFW\glfw3.h>
#include <map>
struct BoneInfo
{
    int id;
    glm::mat4 offset;
};
class Model
{
public:
    /*  函数   */
    Model(const char* path, bool drawinstance = false, vector<glm::mat4> modelMatrices=vector<glm::mat4>(),int instanceAmount=0):drawinstance(drawinstance),modelMatrices(modelMatrices),instanceAmount(instanceAmount)
    {
        loadModel(path);
    }
    
    
    void Draw(Shader& shader, vector<Texture> textures_RT = vector<Texture>(),CubeMap* cubemap=nullptr,vector<Texture3D> voxel_RT=vector<Texture3D>(),Camera*camera=nullptr);
    void UpdateBoundingBox(glm::mat3x3 rotation, glm::vec3 translate, glm::vec3 scale,bool enableRotate);
    vector<Mesh> meshes;
    inline  std::map<string, BoneInfo> & GetBoneInfoMap() { return m_BoneInfoMap; };
    inline int &GetBoneCount() { return m_BoneCounter; };
    ~Model();
private:
    /*  模型数据  */
  
    std::map<string, BoneInfo> m_BoneInfoMap; //
    int m_BoneCounter = 0;

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
    void GetBoneWeightForVertices(vector<Mesh::Vertex>& vertices,aiMesh* mesh);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture::TextureType textureType);
    inline glm::mat4 Model::aiMatrix4x4ToGlm(const aiMatrix4x4* from)
    {
        glm::mat4 to;


        to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
        to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
        to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
        to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

        return to;
    }
};
