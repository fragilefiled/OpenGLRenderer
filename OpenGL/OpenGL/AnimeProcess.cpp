#include"AnimeProcess.h"
AnimeProcess::AnimeProcess(Camera& camera)
{
    this->camera = camera;
}

void AnimeProcess::SetCameraAndTime(Camera& camera, float time,float oneFrameTime)
{
    this->camera = camera;
    this->Time = time;
    this->oneFrameTime = oneFrameTime;
}

void AnimeProcess::Init()
{
    
    myshader = new Shader(".//Shader//AnimationVertexShader.vs", ".//Shader//LightingCubeShader.fs");
    ourModel = new Model(".//Model/vampire/dancing_vampire.dae");
    animation=new Animation(".//Model/vampire/dancing_vampire.dae",ourModel);
     animator=new Animator((animation));
    depthShader = new Shader(".//Shader//DepthShader.vs", ".//Shader//DepthShader.fs");
    gammaCorrectionShader = new Shader(".//Shader//PostEffect.vs", ".//Shader//GammaCorrection.fs");
    lightshader = new Shader(".//Shader//LightShader.vs", ".//Shader//LightShader.fs");
    skyBoxShader = new Shader(".//Shader//CubeMap.vs", ".//Shader//CubeMap.fs");
    posteffect = new PostEffect(*gammaCorrectionShader, width, height, true, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    depthMap = new  FBO(shadowwidth, shadowheight, "_depthTexture", FBO::Depth);
    glm::vec3 lightPos(0.0f, 10.0f, 4.0f);
    glm::vec3 lightPos1(0.0f, 3.0f, 2.0f);
    glm::vec3 lightDir = lightPos;
    glm::vec4 lightColor(1, 1, 1, 0);

    light = new Light(DIRLIGHT, glm::vec3(0.0, 15.0, 1.0), -glm::vec3(0.0, 0.4, 0.5), lightColor);
    pointLight = new Light(POINTLIGHT, lightPos, -lightDir, lightColor);
    pointLight1 = new Light(POINTLIGHT, lightPos1, -lightDir, lightColor);
    pointLight->lightProperty = glm::vec3(1.0f, 0.9f, 0.032f);
    pointLight1->lightProperty = glm::vec3(1.0f, 0.9f, 0.032f);
    spotLight = new  Light(SPOTLIGHT, glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 0.0, -1.0), lightColor);
    spotLight->cutOffAngle = glm::cos(glm::radians(12.5));
    lights[0] = *pointLight;
    lights[1] = *pointLight1;
    lights[2] = *spotLight;
    lights[3] = *light;
    cubemap = new CubeMap(vector<string>{".//Model//skybox//FS000_Day_04.png_Rig.jpg",
        ".//Model//skybox//FS000_Day_04.png_Lef.jpg",
        ".//Model//skybox//FS000_Day_04.png_Top.jpg",
        ".//Model//skybox//FS000_Day_04.png_Bot.jpg",
        ".//Model//skybox//FS000_Day_04.png_Fro.jpg",
        ".//Model//skybox//FS000_Day_04.png_Bak.jpg"});
    for (int i = 0; i < 4; i++)
        lights[i].BindModel();


    posteffect->BindQuad();
    cubemap->Bind();
    posteffect->SetFrameBuffer();

    ubotest = new UBO(2 * sizeof(glm::mat4));
    shaders = vector<Shader>{ *lightshader, *myshader};
    ubotest->SetUnifromBlockIndexAndBind(shaders, "Matrices");
    ubotest->BindUniformBuffer();


}

void AnimeProcess::Process()
{
    animator->UpdateAnimation(oneFrameTime);
    ubotest->ClearSizeNow();
    float screenWidth = width;
    float screenHeight = height;
    glm::mat4 modelskyBox = glm::mat4(1.0f);
    modelskyBox = glm::scale(modelskyBox, glm::vec3(100.0f));
    glm::mat4 projection = glm::mat4(1.0f);;
    projection = glm::perspective(glm::radians(camera.GetFov()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetView();
    glm::mat4 floormodel = glm::mat4(1.0f);
    floormodel = glm::scale(floormodel, glm::vec3(10.0f));
    glm::mat4 floormodel_inverse_t = glm::mat4(1.0f);
    floormodel_inverse_t = glm::transpose(glm::inverse(floormodel));

    glm::mat4 lightView = lights[3].camera.GetView();
    glm::mat4 lightProjection = lights[3].camera.GetProjection();
    glm::mat4 model = glm::mat4(1.0f);
    posteffect->BindFrameBufferInit();
    glViewport(0, 0, width, height);



    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    ubotest->SetMatrix(projection);
    ubotest->SetMatrix(view);//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
    skyBoxShader->use();

    skyBoxShader->setMatrix("model", modelskyBox);
    skyBoxShader->setMatrix("projection", projection);
    glm::mat4 viewskyBox = glm::mat4(glm::mat3(view));
    skyBoxShader->setMatrix("view", viewskyBox);
    cubemap->Draw(*skyBoxShader);
    glDepthFunc(GL_LESS);

    myshader->use();
    for (int i = 0; i < animator->GetFinalBoneMatrices().size(); ++i)
        myshader->setMatrix("finalBonesMatrices[" + std::to_string(i) + "]", animator->GetFinalBoneMatrices()[i]);
    myshader->setMatrix("model", glm::scale(model,glm::vec3(5.0f)));
    myshader->setVec3("baseColor", glm::vec3(0.5, 0.5, 0.5));
    myshader->setVec3("cameraPos", camera.GetPos());
    myshader->setMatrix("lightMat", (lightProjection * lightView));
    for (unsigned int i = 0; i < 4; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        lights[i].SetLight(*myshader);
    }
    ourModel->Draw(*myshader);
    posteffect->BindFrameBufferOver();
   // posteffect->DebugDraw(posteffect->screenTexture);
    posteffect->Draw();

    if (showwindow)
    {
        lightshader->use();
        lightshader->setVec3("lightColor", lightColor);

        /*  if (ImGui::Combo("1", &select, "FFT\0WAVEParticles1\0WAVEParticles2\0"))
          {
              switch (select)
              {
              case 0: enableWaveParticle=false; break;
              case 1: enableWaveParticle = true; enableWaveParticle_hvfliter = false; break;
              case 2: enableWaveParticle = true; enableWaveParticle_hvfliter = true; break;
              }
          }*/
        ImGui::Begin("Dear ImGui Style Editor", &showwindow);
        for (unsigned int i = 0; i < 4; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 modellight = glm::mat4(1.0f);

            //if (lights[i].lightType == DIRLIGHT)
            {
                if (lights[i].lightType == POINTLIGHT) {
                    /*   if(lights[i].pos.x>0)
                           posteffect->DebugDraw(GaussianBlur_v->texes_output[0].id);
                       else
                           posteffect->DebugDraw(normalShader1->texes_output[1].id);*/
                    ImGui::SliderFloat3(("POINTLIGHT pos" + to_string(lights[i].pointLightCount)).c_str(), (float*)&(lights[i].pos), -5.0f, 5.0f);
                    ImGui::SliderFloat3("POINTLIGHT dir" + lights[i].lightType, (float*)&(lights[i].lightDir), -1.0f, 1.0f);
                    ImGui::ColorEdit4(("POINTLIGHT color" + to_string(lights[i].pointLightCount)).c_str(), (float*)&(lights[i].color));
                    lights[i].lightDir = normalize(lights[i].lightDir);
                }
                if (lights[i].lightType == DIRLIGHT)
                {
                    lights[i].lightDir = normalize(lights[i].lightDir);
                    ImGui::SliderFloat3("DIRLIGHT pos", (float*)&(lights[i].pos), -5.0f, 5.0f);
                    ImGui::SliderFloat3("DIRLIGHT dir", (float*)&(lights[i].lightDir), -1.0f, 1.0f);
                    ImGui::ColorEdit4("DIRLIGHT color", (float*)&(lights[i].color));
                    lights[i].lightDir = normalize(lights[i].lightDir);
                    ImGui::SliderFloat3("DIRLIGHT pos", (float*)&(lights[i].pos), -5.0f, 5.0f);
                    ImGui::SliderFloat3("DIRLIGHT dir", (float*)&(lights[i].lightDir), -1.0f, 1.0f);
                    ImGui::ColorEdit4("DIRLIGHT color", (float*)&(lights[i].color));
                }
                if (lights[i].lightType == SPOTLIGHT) {
                    ImGui::SliderFloat3("SPOTLIGHT pos", (float*)&(lights[i].pos), -5.0f, 5.0f);
                    ImGui::SliderFloat3("SPOTLIGHT dir", (float*)&(lights[i].lightDir), -1.0f, 1.0f);
                    ImGui::ColorEdit4("SPOTLIGHT color", (float*)&(lights[i].color));
                    lights[i].lightDir = normalize(lights[i].lightDir);

                }

                lights[i].camera.SetPos(lights[i].pos);
                modellight = glm::translate(modellight, lights[i].pos);
                modellight = glm::scale(modellight, glm::vec3(0.2f));
                lightshader->setMatrix("model", modellight);

                // lights[i].Draw();  //draw cube

            }
        }
        ImGui::End();
    }
}
void AnimeProcess::CalculateDeltaTime() {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

}
AnimeProcess::~AnimeProcess()
{
  
        std::cout << "delete Over";
        delete myshader;
        delete lightshader;
        delete screenShader;
        delete ourModel;
        delete depthShader;
        delete  skyBoxShader;
        delete blit2;
        delete blit1;
        delete blit;
        delete wavemap;
        delete temp1;
        delete temp2;
        delete temp3;
        delete temp4;
        delete posteffect;
        delete gammaCorrectionShader;
        delete light;
        delete pointLight;
        delete pointLight1;
        delete spotLight;
        delete cubemap;
        delete ubotest;
        delete depthMap;
        delete animation;
        delete animator;
        // delete bubbleNormal;

    
}
