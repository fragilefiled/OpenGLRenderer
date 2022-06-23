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
#include "B_Spline.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include"Animator.h"
class AnimeProcess :public RenderProcess
{
public:
    AnimeProcess(Camera& camera);
    void SetCameraAndTime(Camera& camera, float time, float oneFrameTime);
    void Init();
    void Process();
    void CalculateDeltaTime();
    glm::mat4 view;
    bool showwindow=true;
    int width = 1024;
    int height = 1024;

    float lastFrame;

    Shader* myshader = nullptr;
    // Shader lightshader = Shader(".//Shader//MyVertexShader.vs", ".//Shader//LightShader.fs", ".//Shader//LightShader.gs");
    Shader* lightshader = nullptr;
    Shader* screenShader = nullptr;
    Shader* gammaCorrectionShader = nullptr;
    //  Shader OceanShader = Shader(".//Shader//oceanShader.vs", ".//Shader//oceanShader.fs");
    Model* ourModel = nullptr;
    
    Shader* depthShader = nullptr;

    Shader* skyBoxShader = nullptr;
    UBO* ubotest = nullptr;


    PostEffect* blit2 = nullptr;
    PostEffect* blit1 = nullptr;
    PostEffect* blit = nullptr;

    FBO* wavemap = nullptr;
    FBO* temp1 = nullptr;
    FBO* temp2 = nullptr;
    FBO* temp3 = nullptr;
    FBO* temp4 = nullptr;

    Animation* animation;
    Animator* animator;
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
    CubeMap* cubemap = nullptr;


    vector<Shader> shaders;

    int shadowwidth = 2 * width;
    int shadowheight = 2 * height;
    FBO* depthMap = nullptr;

    ~AnimeProcess();


private:

};

