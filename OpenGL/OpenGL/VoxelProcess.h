#pragma once
#pragma once
#include"RenderProcess.h"
#include<iostream>
#include "stb_image.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Model.h"
#include "PostEffect.h"
#include "CubeMap.h"
#include "UBO.h"
#include "FBO.h"
#include"Mymath.h"
#include"Wave_Particle_Pool.h"
#include "B_Spine.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include"Texture3D.h"
#include"VoxelModel.h"
class VoxelProcess :public RenderProcess
{
public:
    VoxelProcess(Camera& camera);
    void SetCameraAndTime(Camera& camera, float time);
    void Init();
    void Process();
    glm::mat4 view;
    glm::vec4 windspeed = glm::vec4(11.0f, 23.0f, 28.0f, 0.0f);
    glm::vec3 limit = glm::vec3(0.2f, 0.4f, 1.0f);
    int width = 1024;
    int height = 1024;
    int CSwidth = 256;
    int CSheight = 256;

    Shader* depthVoxelShader = nullptr;
    Shader* myshader = nullptr;
    Shader* drawVoxel = nullptr;
    Shader* myshader_gs = nullptr;
    // Shader lightshader = Shader(".//Shader//MyVertexShader.vs", ".//Shader//LightShader.fs", ".//Shader//LightShader.gs");
    Shader* lightshader = nullptr;
    Shader* screenShader = nullptr;
    Shader* rasterizationShader = nullptr;
    Shader* gammaCorrectionShader = nullptr;
    Shader* InstanceShader = nullptr;
    Shader* FloorShader = nullptr;
    Model* ourModel = nullptr;
    VoxelModel* voxelModel = nullptr;
    Model* roomModel = nullptr;
    Shader* depthShader = nullptr;
    Shader* skyBoxShader = nullptr;
    Shader* GaussianBlurShader_h = nullptr;
    Shader* GaussianBlurShader_v = nullptr;
    ComputeShader* Calcradiance = nullptr;
    ComputeShader* ClearVoxelMap = nullptr;
    PostEffect* blit1 = nullptr;
    PostEffect* blit2= nullptr;
    PostEffect* blit = nullptr;
    PostEffect* copy = nullptr;
    PostEffect* voxelization = nullptr;
    PostEffect* rasterization = nullptr;
    FBO* temp1 = nullptr;
    FBO* temp2 = nullptr;
    FBO* voxelRT = nullptr;

    PostEffect* posteffect = nullptr;





    glm::mat4 trans = glm::mat4(1.0f);

    glm::vec3 lightPos;
    glm::vec3 lightPos1;
    glm::vec3 lightDir;;
    glm::vec4 lightColor;
    
    Light* light = nullptr;
    Light* pointLight = nullptr;
    Light* pointLight1 = nullptr;
    Light* spotLight = nullptr;
    Light lights[4];
    /*   CubeMap cubemap = CubeMap(vector<string>{".//Model//skybox//right.jpg",
           ".//Model//skybox//left.jpg",
           ".//Model//skybox//top.jpg",
           ".//Model//skybox//bottom.jpg",
           ".//Model//skybox//front.jpg",
           ".//Model//skybox//back.jpg"});*/
    CubeMap* cubemap = nullptr;
    Texture3D* voxel_diffuse = nullptr;
    Texture3D* voxel_normal = nullptr;
    Texture3D* voxel_radiance = nullptr;
    bool showwindow;
    UBO* ubotest = nullptr;

    vector<Shader> shaders;

    int shadowwidth =  width;
    int shadowheight =  height;
    int voxel_resolution = 256;
    int voxel_width_world = 16;
    float RasterScale = 1.3f;
    float Xscale = 0.063f;
    int normalBlend = 0;
    int voxelnormalBlend = 0;
    FBO* depthMap = nullptr;
    Wave_Particle_Pool* pool;
    int particle_num = 700;
    std::vector<Texture> rt = std::vector<Texture>(10);
    std::vector<Texture3D> voxel_rt = std::vector<Texture3D>(3);
    std::vector<glm::mat4> voxelViewProjction;
    std::vector<glm::mat4> voxelViewProjctionInverse;
   // std::vector<float> data;
    glm::vec3  voxelWorldMinPoint;
    ~VoxelProcess();



private:

};

