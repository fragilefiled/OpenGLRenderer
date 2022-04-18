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
#include"Gbuffer.h"
#include <GLFW\glfw3.h>
class VoxelProcess :public RenderProcess
{
public:
    VoxelProcess(Camera& camera);
    void SetCameraAndTime(Camera& camera, float time);
    void Init();
    void Process();
    void GenerateMipmap();
    
   // float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f;
    glm::mat4 view;
    glm::vec4 windspeed = glm::vec4(11.0f, 23.0f, 28.0f, 0.0f);
    glm::vec3 limit = glm::vec3(0.2f, 0.4f, 1.0f);
    //glm::vec3 ScaleUse = glm::vec3(8.0f, 8.0f, 7.0f);
    glm::vec3 ScaleUse = glm::vec3(8.0f, 8.0f, 7.0f);
    float maxdistance=0.5f;
    float stepLength = 0.5;
    float aperture = 1.0f;
    float occ_falloff = 800.0f;
    int width = 1024;
    int height = 1024;
    int CSwidth = 256;
    int CSheight = 256;
    bool InjectFirstBounce = false;
    bool EnableAmbientOcc = false;
    bool EnableVoxelization = true;
    Shader* deferVoxelConeTracing = nullptr;
    Shader* Gemetory_Pass = nullptr;
    Shader* depthVoxelShader = nullptr;
    Shader* myshader = nullptr;
    Shader* voxelShaderRender = nullptr;
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
    Shader* BilateralFilterShader_h = nullptr;
    Shader* BilateralFilterShader_v = nullptr;
    Gbuffer* gBuffer = nullptr;
    ComputeShader* Calcradiance = nullptr;
    ComputeShader* ClearVoxelMap = nullptr;
    ComputeShader* MipmapProduceFirst = nullptr;
    ComputeShader* MipmapProduceSecond = nullptr;
    ComputeShader* VoxelConeTracing = nullptr;
    Shader* AddShader = nullptr;
    PostEffect* blit1 = nullptr;
    PostEffect* blit2= nullptr;
    PostEffect* blit = nullptr;
    PostEffect* copy = nullptr;
    PostEffect* voxelization = nullptr;
    PostEffect* rasterization = nullptr;
    PostEffect* lightPass = nullptr;
    PostEffect* AddInDirLight = nullptr;
    FBO* temp1 = nullptr;
    FBO* temp2 = nullptr;
    FBO* voxelRT = nullptr;
    
    PostEffect* posteffect = nullptr;
    
    Camera VoxelCullCamera;



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
    Texture3D* voxel_static_mark = nullptr;
    Texture3D* voxel_anisotropicmipmap[6];;
    bool showwindow;
    UBO* ubotest = nullptr;

    vector<Shader> shaders;

    int shadowwidth = width;
    int shadowheight =  height;
    int voxel_resolution = 256;
    int voxel_width_world = 24;
    float RasterScale = 1.3f;
    float Xscale = voxel_width_world/(float)voxel_resolution;
    int normalBlend = 0;
    int voxelnormalBlend = 0;
    float IndirectLight = 3.0;
    float directLight = 1.0;
    float deferDirLight = 1.0;
    float deferInDirLight = 1.0;
    FBO* depthMap = nullptr;
    FBO* depthMapDefer = nullptr;
    Wave_Particle_Pool* pool;
    int particle_num = 700;
    std::vector<Texture> rt = std::vector<Texture>(10);
    std::vector<Texture3D> voxel_rt = std::vector<Texture3D>(4);
    std::vector<glm::mat4> voxelViewProjction;
    std::vector<glm::mat4> voxelViewProjctionInverse;
   // std::vector<float> data;
    glm::vec3  voxelWorldMinPoint;
    glm::vec3  voxelWorldMaxPoint;
    void CalculateDeltaTime();
    ~VoxelProcess();



private:

};

