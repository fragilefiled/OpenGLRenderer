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
class WaveProcess:public RenderProcess
{
public:
	WaveProcess(Camera &camera);
    void SetCameraAndTime(Camera&camera,float time);
	void Init();
	void Process();
	glm::mat4 view;
	glm::vec4 windspeed = glm::vec4(11.0f, 23.0f, 28.0f, 0.0f);
	glm::vec3 limit = glm::vec3(0.2f, 0.4f, 1.0f);
	int width = 1024;
	int height = 1024;
	int CSwidth = 256;
	int CSheight = 256;
	float A = 32.0f;
	float BubblesThreshold = 0.7f;
	float BubblesScale = 1.0f;
	float jacobScale = 100.0f;
	float Input_N = 64;
	float B_Spine_count = 1024;
	float scale_Wave = 0.2f;
	float scale_Ocean = 10.0f;
	float scale_subsurface = 1.0f;
    float glossiness = 0.987f;
    float metallic = 0.987f;
    float jacobScaleEdge = 2.0f;
	glm::vec4 scale_Control_WaveParicle = glm::vec4(0.3f, 1.0f, 0.8, 0.0f);
	//glm::vec4 scale_Control_WaveParicle = glm::vec4(0.0f, 1.0f, 0.0, 0.0f);
	int wave_particle_resolution = 64;
	int wave_particle_resolution_fliter =256;
	float radius = 100*(wave_particle_resolution_fliter/1024.0f );
    bool enableWaveParticle = false;
    bool enableWaveParticle_hvfliter = false;

    Shader* myshader = nullptr;
    // Shader lightshader = Shader(".//Shader//MyVertexShader.vs", ".//Shader//LightShader.fs", ".//Shader//LightShader.gs");
    Shader* lightshader = nullptr;
    Shader* screenShader = nullptr;
    Shader* gammaCorrectionShader = nullptr;
    Shader* InstanceShader = nullptr;
    Shader* FloorShader = nullptr;
    //  Shader OceanShader = Shader(".//Shader//oceanShader.vs", ".//Shader//oceanShader.fs");
    Model* ourModel = nullptr;
    Model* roomModel = nullptr;
    //Model sponzaModel(".//Model//sponza//sponza.obj");
    Shader* depthShader = nullptr;
    Shader* depthOceanShader = nullptr;
    // Model ourModel(".//Model//planet//planet.obj");
    //  Model ourModel1(".//Model//rock//rock.obj",true,modelMatrices,amount);
     //Model ourModel1(".//Model//rock//rock.obj");
    Shader* skyBoxShader = nullptr;
    ComputeShader* GaussianShader = nullptr;
    ComputeShader* HeightSpectrumShader = nullptr;
    Shader* GaussianBlurShader_h = nullptr;
    Shader* GaussianBlurShader_v = nullptr;
    Shader* AddDisplacementShader = nullptr;
    Shader* WaveParticleShader_h = nullptr;
    Shader* WaveParticleShader_v = nullptr;
    Shader* DrawPoint = nullptr;
    //  Shader GroundShader = Shader(".//Shader//ground.vs", ".//Shader//ground.fs");
    ComputeShader* OffsetSpectrumShader = nullptr;
    ComputeShader* FftXShader = nullptr;
    ComputeShader* FftYShader = nullptr;
    ComputeShader* FftXendShader = nullptr;
    ComputeShader* FftYendShader = nullptr;
    ComputeShader* DisplacementShader = nullptr;
    ComputeShader* normalShader = nullptr;
    ComputeShader* WaveParticleShader = nullptr;
    ComputeShader* GaussianBlur_h = nullptr;
    ComputeShader* GaussianBlur_v = nullptr;
    ComputeShader* GaussianBlur_h1 = nullptr;
    ComputeShader* GaussianBlur_v1 = nullptr;
    ComputeShader* normalShader1 = nullptr;
    PostEffect* blit6 = nullptr;
    PostEffect* blit5 = nullptr;
    PostEffect* blit4 = nullptr;
    PostEffect* blit3 = nullptr;
    PostEffect* blit2 = nullptr;
    PostEffect* blit1 = nullptr;
    PostEffect* blit = nullptr;
    PostEffect* copy = nullptr;
    PostEffect* copyWaveParticle = nullptr;
    FBO* wavemap = nullptr;
    FBO* temp1 = nullptr;
    FBO* temp2 = nullptr;
    FBO* temp3 = nullptr;
    FBO* temp4 = nullptr;
    FBO* temp5 = nullptr;
    FBO* temp6 = nullptr;
    FBO* temp7 = nullptr;
    FBO* temp8 = nullptr;

    PostEffect* posteffect = nullptr;
    // vector<glm::vec2> control = vector<glm::vec2>({ glm::vec2(0,0),glm::vec2(11,0),glm::vec2(17,8),glm::vec2(24,10),glm::vec2(26,19),glm::vec2(26,0),glm::vec2(26,0),glm::vec2(26,0),glm::vec2(40,0),glm::vec2(45,0) });
    vector<glm::vec2> control = vector<glm::vec2>({ glm::vec2(0,0),glm::vec2(11,0),glm::vec2(17,8),glm::vec2(24,18),glm::vec2(26,19),glm::vec2(26.2,0),glm::vec2(30,0) });
    //  vector<glm::vec2> control = vector<glm::vec2>({ glm::vec2(0,5),glm::vec2(11,7),glm::vec2(17,8),glm::vec2(24,18),glm::vec2(26,19),glm::vec2(26.2,0),glm::vec2(30,0) });
    B_Spine b_spine = B_Spine(B_Spine_count, 4, control, vector<double>(0));


    TextureImage *particleImage = nullptr;
    TextureImage *particleImage1 = nullptr;
    TextureImage *waveImage1=nullptr ;

    glm::mat4 trans = glm::mat4(1.0f);

    glm::vec3 lightPos;
    glm::vec3 lightPos1;
    glm::vec3 lightDir;;
    glm::vec4 lightColor;

    Light *light = nullptr;
    Light* pointLight = nullptr;
    Light *pointLight1 = nullptr;
    Light *spotLight = nullptr;
    Light lights[4] ;
    /*   CubeMap cubemap = CubeMap(vector<string>{".//Model//skybox//right.jpg",
           ".//Model//skybox//left.jpg",
           ".//Model//skybox//top.jpg",
           ".//Model//skybox//bottom.jpg",
           ".//Model//skybox//front.jpg",
           ".//Model//skybox//back.jpg"});*/
    CubeMap *cubemap = nullptr;
    unsigned int pointVAO, pointVBO;
    //Texture* bubbleNormal = nullptr;
    Texture* bubbleDiffuse = nullptr;
    //Texture* bubbleNormal = nullptr;
    //Texture* bubbleBump = nullptr;
  /*  Texture* waterSpec = nullptr;*/
     bool showwindow ;
    UBO *ubotest = nullptr;

    vector<Shader> shaders;

    int shadowwidth = 2 * width;
    int shadowheight = 2 * height;
    FBO *depthMap = nullptr;
    Wave_Particle_Pool* pool;
    int particle_num = 700;
    std::vector<Texture> rt = std::vector<Texture>(10);
    ~WaveProcess();
       
      
    
private:
    
};

