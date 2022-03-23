#include"VoxelProcess.h"
VoxelProcess::VoxelProcess(Camera& camera)
{
    this->camera = camera;
}
void VoxelProcess::SetCameraAndTime(Camera& camera, float time)
{
    this->camera = camera;
    this->Time = time;
}
void VoxelProcess::Init()
{
    showwindow = true;

    drawVoxel = new Shader(".//Shader//DrawVoxel.vs", ".//Shader//DrawVoxel.fs", ".//Shader//DrawVoxel.gs");
    myshader = new Shader(".//Shader//MyVertexShader.vs", ".//Shader//LightingCubeShader.fs");
    myshader_gs = new Shader(".//Shader//MyVertexShader.vs", ".//Shader//gsShader.fs", ".//Shader//voxelization.gs");
    // Shader lightshader = Shader(".//Shader//MyVertexShader.vs", ".//Shader//lightshader->fs", ".//Shader//lightshader->gs");
    lightshader = new Shader(".//Shader//LightShader.vs", ".//Shader//LightShader.fs");
    screenShader = new Shader(".//Shader//PostEffect.vs", ".//Shader//PostEffect.fs");
    rasterizationShader = new Shader(".//Shader//msaa_rasterization.vs", ".//Shader//msaa_rasterization.fs");
    InstanceShader = new Shader(".//Shader//InstanceShader.vs", ".//Shader//LightingCubeShader.fs");
    FloorShader = new Shader(".//Shader//floorShader.vs", ".//Shader//floorShader.fs");
    //  Shader OceanShader = Shader(".//Shader//oceanShader.vs", ".//Shader//oceanShader.fs");
    roomModel = new Model(".//Model//floor//floor1.obj");
  //ourModel = new Model(".//Model//nanosuit//nanosuit.obj");
   //ourModel = new Model(".//Model//planet//planet.obj");
    //ourModel = new Model(".//Model//planet//planet.obj");
   // ourModel = new Model(".//Model//Suzanne//Suzanne.obj");
 // ourModel = new Model(".//Model//Cornell-Box//ProRender.obj");
 ourModel= new Model(".//Model//sponza//sponza.obj");
    voxelModel = new VoxelModel(voxel_resolution * voxel_resolution * voxel_resolution);
    //delete roomModel;
    //Model sponzaModel(".//Model//sponza//sponza.obj");
    depthShader = new Shader(".//Shader//DepthShader.vs", ".//Shader//DepthShader.fs");
    depthVoxelShader=new Shader(".//Shader//DepthDrawVoxel.vs", ".//Shader//DepthShader.fs", ".//Shader//DepthDrawVoxel.gs");
    //  Model ourModel1(".//Model//rock//rock.obj",true,modelMatrices,amount);
     //Model ourModel1(".//Model//rock//rock.obj");
    //Model ourModel(".//Model//sponza//sponza.obj");
    skyBoxShader = new Shader(".//Shader//CubeMap.vs", ".//Shader//CubeMap.fs");
    GaussianBlurShader_h = new Shader(".//Shader//PostEffect_gaussian_h.vs", ".//Shader//GaussionBlur_h.fs");
    GaussianBlurShader_v = new Shader(".//Shader//PostEffect_gaussian_v.vs", ".//Shader//GaussionBlur_h.fs");
    gammaCorrectionShader = new Shader(".//Shader//PostEffect.vs", ".//Shader//GammaCorrection.fs");
    Calcradiance = new ComputeShader(".//Shader//CalaRadiance.comp",glm::vec3(voxel_resolution/8.0f, voxel_resolution / 8.0f, voxel_resolution / 8.0f), 0, 0, 3);
    ClearVoxelMap = new ComputeShader(".//Shader//ClearVoxelMap.comp", glm::vec3(voxel_resolution / 8.0f, voxel_resolution / 8.0f, voxel_resolution / 8.0f), 0, 0, 3);
    blit2 = new PostEffect(*GaussianBlurShader_h, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    blit1 = new PostEffect(*GaussianBlurShader_v, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    blit = new PostEffect(*screenShader, CSwidth, CSheight, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    copy = new PostEffect(*screenShader, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    voxelization = new PostEffect(*screenShader, voxel_resolution, voxel_resolution, true, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    rasterization = new PostEffect(*rasterizationShader, voxel_resolution, voxel_resolution, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    posteffect = new PostEffect(*gammaCorrectionShader, width, height, true, GL_RGBA32F, GL_RGBA, GL_FLOAT);

    //voxel_diffuse = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution,true,GL_RGBA32F,GL_RGBA,GL_FLOAT);
    //voxel_normal = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    //voxel_radiance = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_RGBA32F, GL_RGBA, GL_FLOAT,2);
    voxel_diffuse = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    voxel_normal = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    voxel_radiance = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 2);
    shadowwidth = 2 * width;
    shadowheight = 2 * height;
    depthMap = new  FBO(shadowwidth, shadowheight, "_depthTexture", FBO::Depth);
    temp1 = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    temp2 = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    voxelRT = new FBO(voxel_resolution, voxel_resolution, "_voxelMap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT,true);
 






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

    /*  CubeMap cubemap = CubeMap(vector<string>{".//Model//skybox//right.jpg",
           ".//Model//skybox//left.jpg",
           ".//Model//skybox//top.jpg",
           ".//Model//skybox//bottom.jpg",
           ".//Model//skybox//front.jpg",
           ".//Model//skybox//back.jpg"});*/

    cubemap = new CubeMap(vector<string>{".//Model//skybox//FS000_Day_04.png_Rig.jpg",
        ".//Model//skybox//FS000_Day_04.png_Lef.jpg",
        ".//Model//skybox//FS000_Day_04.png_Top.jpg",
        ".//Model//skybox//FS000_Day_04.png_Bot.jpg",
        ".//Model//skybox//FS000_Day_04.png_Fro.jpg",
        ".//Model//skybox//FS000_Day_04.png_Bak.jpg"});
    for (int i = 0; i < 4; i++)
        lights[i].BindModel();



    //unsigned int pointVAO, pointVBO;

   // std::vector<float> data = std::vector<float>(voxel_resolution * voxel_resolution * voxel_resolution * 4, 0.0);
    voxel_radiance->Bind();
    voxel_diffuse->Bind();
    voxel_normal->Bind();
    posteffect->BindQuad();
    blit->BindQuad();
    blit1->BindQuad();
    blit2->BindQuad();
    copy->BindQuad();
    voxelization->BindQuad();
    rasterization->BindQuad();

    cubemap->Bind();



    // framebuffer configuration
    rasterization->SetFrameBuffer();
    posteffect->SetFrameBuffer();
    blit->SetFrameBuffer();
    blit1->SetFrameBuffer();
    blit2->SetFrameBuffer();
    copy->SetFrameBuffer();
    voxelization->SetFrameBuffer();
    ubotest = new UBO(2 * sizeof(glm::mat4));
  
    shaders = vector<Shader>{ *lightshader, *myshader,*FloorShader,*myshader_gs,*drawVoxel };
    ubotest->SetUnifromBlockIndexAndBind(shaders, "Matrices");
    ubotest->BindUniformBuffer();

    srand((int)time(0));
    particle_num = 700;
   
    voxelViewProjction = camera.GetVoxelViewProjection(glm::vec3(0, 0, 0),voxel_width_world);
    voxelViewProjctionInverse = camera.GetVoxelViewProjectionInverse(glm::vec3(0, 0, 0),voxel_width_world);
    /* pool->PushToParticles(glm::vec2(64, 64), glm::vec2(64, 64), glm::vec2(0.1, 0.1), 0.3f, 5.0f, 60, 0.0);*/
     //pool->particles.push_back(Wave_Particle( glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(0.1, 0.1), 1.0, 2.5, 30, 0.0));
   // voxel->setData(data);
    voxel_rt[0] = *voxel_diffuse;
    voxel_rt[0].nameInShader = "voxelMap_diffuse";
    voxel_rt[1] = *voxel_normal;
    voxel_rt[1].nameInShader = "voxelMap_normal";
    voxel_rt[2] = *voxel_radiance;
    voxel_rt[2].nameInShader = "voxelMap_radiance";
    voxelWorldMinPoint = camera.getVoxelMinPoint(glm::vec3(0, 0, 0), voxel_width_world);
    Calcradiance->texes3d = voxel_rt;
}

void VoxelProcess::Process()
{
    /*   glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
    ClearVoxelMap->texes3d = voxel_rt;
    ClearVoxelMap->use();
    ClearVoxelMap->dispatch();
    ClearVoxelMap->wait();

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
    glm::mat4 modelVoxel = glm::mat4(1.0f);
    //  model = glm::rotate(model, glm::radians(70.f*(float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
   model = glm::scale(model, glm::vec3(0.004f));
   // model = glm::scale(model, glm::vec3(0.3f));
    glm::mat4 model_inverse_t = glm::mat4(1.0f);
    model_inverse_t = glm::transpose(glm::inverse(model));
    
    modelVoxel = glm::scale(modelVoxel, glm::vec3(Xscale));
    modelVoxel = glm::translate(modelVoxel, glm::vec3(-(voxel_resolution-1) * 0.5, -(voxel_resolution - 1) * 0.5, -(voxel_resolution - 1) * 0.5));

    glViewport(0, 0, voxel_resolution, voxel_resolution);
    voxelization->BindFrameBufferInit();
    ubotest->SetMatrix(projection);
    ubotest->SetMatrix(view);//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDepthFunc(GL_LESS);

    glDisable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    //glDisable(GL_MULTISAMPLE);
    myshader_gs->use();
    myshader_gs->setMatrix("model", model);
    myshader_gs->setVec3("baseColor", glm::vec3(0.5, 0.5, 0.5));
    myshader_gs->setVec3("cameraPos", camera.GetPos());
    myshader_gs->setMatrix("lightMat", (lightProjection * lightView));
    myshader_gs->setMatrix("model_inverse_t", (model_inverse_t));
    myshader_gs->setMatrixArray("voxelViewProjection", voxelViewProjction);
    myshader_gs->setFloat("voxelResolution", voxel_resolution);
    myshader_gs->setFloat("voxelWidthWorld", voxel_width_world);
    myshader_gs->setMatrixArray("voxelViewProjectionInverse", voxelViewProjctionInverse);
    myshader_gs->setVec3("worldMinPoint", voxelWorldMinPoint);
    myshader_gs->setFloat("RasterScale", RasterScale);
    myshader_gs->setInt("VoxelNormalBlend", voxelnormalBlend);
    for (unsigned int i = 0; i < 4; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        lights[i].SetLight(*myshader_gs);
    }
   //if(Time<2.0f)
    voxel_rt[0].format = GL_R32UI;
    voxel_rt[1].format = GL_R32UI;
    ourModel->Draw(*myshader_gs, rt, nullptr, voxel_rt);
    voxel_rt[0].format = GL_RGBA8;
    voxel_rt[1].format = GL_RGBA8;
    //myshader_gs->setMatrix("model", floormodel);
    //roomModel->Draw(*myshader_gs, rt, nullptr, voxel_rt);
    ubotest->ClearSizeNow();
    voxelization->BindFrameBufferOver();



    glViewport(0, 0, shadowwidth, shadowheight);
    depthMap->BindFrameBufferInit();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    glDisable(GL_CULL_FACE);
    ubotest->SetMatrix(projection);
    ubotest->SetMatrix(view);//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
  /*  depthShader->use();
    depthShader->setMatrix("lightProjection", lightProjection);
    depthShader->setMatrix("lightView", lightView);
    depthShader->setMatrix("model", model);
    ourModel->Draw(*depthShader);*/
    
    
    depthVoxelShader->use();
    depthVoxelShader->setMatrix("model", modelVoxel);
    depthVoxelShader->setVec3("cameraPos", camera.GetPos());
    depthVoxelShader->setMatrix("lightMat", (lightProjection * lightView));
    depthVoxelShader->setMatrixArray("voxelViewProjection", voxelViewProjction);
    depthVoxelShader->setFloat("voxelResolution", voxel_resolution);
    depthVoxelShader->setInt("voxelResolutionI", voxel_resolution);
    depthVoxelShader->setFloat("voxelWidthWorld", voxel_width_world);
    depthVoxelShader->setMatrixArray("voxelViewProjectionInverse", voxelViewProjctionInverse);
    depthVoxelShader->setVec3("worldMinPoint", voxelWorldMinPoint);
    voxelModel->Draw(*depthVoxelShader,voxel_rt);
    depthMap->BindFrameBufeerOver();
    ubotest->ClearSizeNow();

    rt[0] = depthMap->texture;




 

   // glViewport(0, 0, voxel_resolution, voxel_resolution);
   // rasterization->BindFrameBufferInit();
   // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   // glClear(GL_COLOR_BUFFER_BIT);
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   // glDepthFunc(GL_LESS);
   // glDisable(GL_CULL_FACE);
   // rasterization->UseShader();
   // 
   // rasterization->DrawQuadMS(voxelization->textureColorbuffer,false);
   //// glActiveTexture(GL_TEXTURE0 + 1);
   // //BindTexture(GL_TEXTURE_2D, voxelization->textureColorbuffer);
   // rasterization->Blit(*voxelRT);
   // rasterization->BindFrameBufferOver();





    //glViewport(0, 0, shadowwidth, shadowheight);
    //depthMap->BindFrameBufferInit();
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_STENCIL_TEST);

    //glDisable(GL_CULL_FACE);
    //ubotest->SetMatrix(projection);
    //ubotest->SetMatrix(view);//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
    //depthShader->use();
    //depthShader->setMatrix("lightProjection", lightProjection);
    //depthShader->setMatrix("lightView", lightView);
    //depthShader->setMatrix("model", model);
    //ourModel->Draw(*depthShader);
    //depthMap->BindFrameBufeerOver();
    //ubotest->ClearSizeNow();

    //rt[0] = depthMap->texture;
    

    glViewport(0, 0, width, height);
    posteffect->BindFrameBufferInit();


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



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
    myshader->setMatrix("model", model);
    myshader->setVec3("baseColor", glm::vec3(0.5, 0.5, 0.5));
    myshader->setVec3("cameraPos", camera.GetPos());
    myshader->setMatrix("lightMat", (lightProjection * lightView));
    myshader->setMatrix("model_inverse_t", (model_inverse_t));
    for (unsigned int i = 0; i < 4; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        lights[i].SetLight(*myshader);
    }
    
  // ourModel->Draw(*myshader, rt,nullptr,voxel_rt);
    //myshader_gs->use();
    //myshader_gs->setMatrix("model", model);
    //myshader_gs->setVec3("baseColor", glm::vec3(0.5, 0.5, 0.5));
    //myshader_gs->setVec3("cameraPos", camera.GetPos());
    //myshader_gs->setMatrix("lightMat", (lightProjection * lightView));
    //myshader_gs->setMatrix("model_inverse_t", (model_inverse_t));
    //myshader_gs->setFloat("voxelResolution", 32.0);
    //myshader_gs->setMatrixArray("voxelViewProjection", voxelViewProjction);
    //myshader_gs->setMatrixArray("voxelViewProjectionInverse", voxelViewProjctionInverse);
    //ourModel->Draw(*myshader_gs, rt, nullptr, voxel_rt);
     
    
    //FloorShader->use();
    //for (unsigned int i = 0; i < 4; i++)
    //{
    //    // calculate the model matrix for each object and pass it to shader before drawing
    //    lights[i].SetLight(*FloorShader);
    //}
    //FloorShader->setMatrix("model", floormodel);
    //FloorShader->setMatrix("model_inverse_t", floormodel_inverse_t);
    //FloorShader->setFloat("Time", 2 * Time);
    //FloorShader->setVec3("cameraPos", camera.GetPos());
    //FloorShader->setMatrix("lightMat", (lightProjection * lightView));

   
    //glDepthFunc(GL_LESS);
    //ubotest->SetMatrix(projection);
    //ubotest->SetMatrix(view);//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))


    //roomModel->Draw(*FloorShader, rt, cubemap);
  
  
 
    
    //modelVoxel = glm::translate(modelVoxel, glm::vec3(-voxel_resolution*0.5 , -voxel_resolution * 0.5, -voxel_resolution * 0.5));
  /*  drawVoxel->use();
    drawVoxel->setMatrix("model", modelVoxel);
    drawVoxel->setVec3("cameraPos", camera.GetPos());
    drawVoxel->setMatrix("lightMat", (lightProjection * lightView));
    drawVoxel->setMatrix("model_inverse_t", (model_inverse_t));
    drawVoxel->setMatrixArray("voxelViewProjection", voxelViewProjction);
    drawVoxel->setFloat("voxelResolution", voxel_resolution);
    drawVoxel->setInt("voxelResolutionI", voxel_resolution);
    drawVoxel->setFloat("voxelWidthWorld", voxel_width_world);
    drawVoxel->setMatrixArray("voxelViewProjectionInverse", voxelViewProjctionInverse);
    drawVoxel->setVec3("worldMinPoint", voxelWorldMinPoint);
    voxelModel->Draw(*drawVoxel, voxel_rt);*/


 
    glm::mat4 modelVoxel_inverse_t = glm::transpose(glm::inverse(modelVoxel));
    Calcradiance->use();
    Calcradiance->setMatrix("lightMat", (lightProjection * lightView));
    Calcradiance->setMatrix("model", modelVoxel);
    Calcradiance->setMatrix("model_inverse_t", (modelVoxel_inverse_t));
    glActiveTexture(GL_TEXTURE0 +3);
    Calcradiance->setInt(rt[0].nameInShader, 3);
    Calcradiance->setInt("openNormalBlend", normalBlend);
    glBindTexture(GL_TEXTURE_2D, rt[0].id);
    for (unsigned int i = 0; i < 4; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        lights[i].SetLight(*Calcradiance);
    }
   
    Calcradiance->dispatch();
    Calcradiance->wait();
    
    drawVoxel->use();
    drawVoxel->setMatrix("model", modelVoxel);
    drawVoxel->setVec3("cameraPos", camera.GetPos());
    drawVoxel->setMatrix("lightMat", (lightProjection * lightView));
    drawVoxel->setMatrix("model_inverse_t", (model_inverse_t));
    drawVoxel->setMatrixArray("voxelViewProjection", voxelViewProjction);
    drawVoxel->setFloat("voxelResolution", voxel_resolution);
    drawVoxel->setInt("voxelResolutionI", voxel_resolution);
    drawVoxel->setFloat("voxelWidthWorld", voxel_width_world);
    drawVoxel->setMatrixArray("voxelViewProjectionInverse", voxelViewProjctionInverse);
    drawVoxel->setVec3("worldMinPoint", voxelWorldMinPoint);
    voxelModel->Draw(*drawVoxel, voxel_rt);



    lightshader->use();
    lightshader->setVec3("lightColor", lightColor);
    glm::mat4 modellight = glm::mat4(1.0f);
    modellight = glm::scale(modellight, glm::vec3(1.0f));
    modellight = glm::translate(modellight, lights[3].pos);
 
    lightshader->setMatrix("model", modellight);
    lights[3].Draw();  //draw cube

    posteffect->BindFrameBufferOver();



    posteffect->Draw();

    if (limit.x < 0.3)
        posteffect->DebugDraw(depthMap->texture.id);

  // posteffect->DebugDraw(voxelRT->texture.id);
  // posteffect->DebugDraw(voxelization->screenTexture);

    //posteffect->DebugDraw(normalShader->texes_output[3].id);
  ///posteffect->DebugDraw(normalShader->texes_output[4].id);
    //posteffect->DebugDraw(DisplacementShader->texes_output[3].id);
    //posteffect->DebugDraw(WaveParticleShader->texes_output[0].id);
    // glViewport(0, 0, width, height);
     //posteffect->DebugDraw(WaveParticleShader->texes_output[0].initid);
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
        ImGui::SliderFloat3(("limit"), (float*)&(limit), 0.0f, 1.0f);
        ImGui::SliderFloat("Conventional Rasterization", &RasterScale, 1.0f, 4.0f);
        ImGui::SliderFloat("Xscale", &Xscale, 0.0f, 1.0f);
        ImGui::SliderInt("NormalBlend", &normalBlend, 0.0, 1.0);
        ImGui::SliderInt("VoxelNormalBlend", &voxelnormalBlend, 0.0, 1.0);
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
                    ImGui::SliderFloat3("DIRLIGHT pos", (float*)&(lights[i].pos), -16.0f, 16.0f);
                    ImGui::SliderFloat3("DIRLIGHT dir", (float*)&(lights[i].lightDir), -1.0f, 1.0f);
                    ImGui::ColorEdit4("DIRLIGHT color", (float*)&(lights[i].color));
                    lights[i].lightDir = normalize(lights[i].lightDir);
                    ImGui::SliderFloat3("DIRLIGHT pos", (float*)&(lights[i].pos), -16.0f, 16.0f);
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
                modellight = glm::scale(modellight, glm::vec3(1.0f));
                lightshader->setMatrix("model", modellight);

                //if((lights[i].lightType == DIRLIGHT))
                // lights[i].Draw();  //draw cube

            }
        }
        ImGui::End();
    }

}

VoxelProcess::~VoxelProcess()
{
    std::cout << "delete Over";
    delete drawVoxel;
    delete myshader;
    delete myshader_gs;
    delete lightshader;
    delete rasterizationShader;
    delete screenShader;
    delete InstanceShader;
    delete depthVoxelShader;
    delete FloorShader;
    delete ourModel;
    delete voxelModel;
    delete roomModel;
    delete depthShader;
    delete  skyBoxShader;
    delete  GaussianBlurShader_h;
    delete  GaussianBlurShader_v;
    delete Calcradiance;
    delete ClearVoxelMap;
    delete blit2;
    delete blit1;
    delete blit;
    delete copy;
    delete voxelization;
    delete rasterization;
    delete temp1;
    delete temp2;
    delete voxelRT;
    delete posteffect;
    delete gammaCorrectionShader;
    delete light;
    delete pointLight;
    delete pointLight1;
    delete spotLight;
    delete cubemap;
    delete ubotest;
    delete depthMap;
    delete pool;
    delete voxel_diffuse;
    delete voxel_normal;
    delete voxel_radiance;
    auto temp = std::vector<glm::mat4>(0);
    voxelViewProjction.swap(temp);
    voxelViewProjctionInverse.swap(temp);
    // delete bubbleNormal;

}
