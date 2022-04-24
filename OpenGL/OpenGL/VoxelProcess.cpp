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
    gBuffer = new Gbuffer(width,height);
    drawVoxel = new Shader(".//Shader//DrawVoxel.vs", ".//Shader//DrawVoxel.fs", ".//Shader//DrawVoxel.gs");
    voxelShaderRender= new Shader(".//Shader//MyVertexShader.vs", ".//Shader//VoxelModelRender.fs");
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
 // ourModel = new Model(".//Model//backpack//backpack.obj");
   //ourModel = new Model(".//Model//planet//planet.obj");
    //ourModel = new Model(".//Model//planet//planet.obj");
   // ourModel = new Model(".//Model//Suzanne//Suzanne.obj");
//ourModel = new Model(".//Model//Cornell-Box//ProRender.obj");
//   ourModel = new Model(".//Model//Cornell-Box//cornell_box.obj");
 ourModel= new Model(".//Model//sponza//sponza.obj");
    voxelModel = new VoxelModel(voxel_resolution * voxel_resolution * voxel_resolution);
    //delete roomModel;
    //Model sponzaModel(".//Model//sponza//sponza.obj");
    depthShader = new Shader(".//Shader//DepthShader.vs", ".//Shader//DepthShader.fs");
    depthVoxelShader=new Shader(".//Shader//DepthDrawVoxel.vs", ".//Shader//DepthShader.fs", ".//Shader//DepthDrawVoxel.gs");
    //  Model ourModel1(".//Model//rock//rock.obj",true,modelMatrices,amount);
     //Model ourModel1(".//Model//rock//rock.obj");
    //Model ourModel(".//Model//sponza//sponza.obj");
    AddShader= new Shader(".//Shader//PostEffect.vs", ".//Shader//AddInDirLight.fs");
    deferVoxelConeTracing = new Shader(".//Shader//PostEffect.vs", ".//Shader//deferVoxelConeTracing.fs");
    Gemetory_Pass= new Shader(".//Shader//Gemetory_Pass.vs", ".//Shader//Gemetory_Pass.fs");
    skyBoxShader = new Shader(".//Shader//CubeMap.vs", ".//Shader//CubeMap.fs");
    GaussianBlurShader_h = new Shader(".//Shader//PostEffect_gaussian_h.vs", ".//Shader//GaussionBlur_h.fs");
    GaussianBlurShader_v = new Shader(".//Shader//PostEffect_gaussian_v.vs", ".//Shader//GaussionBlur_h.fs");
    BilateralFilterShader_h= new Shader(".//Shader//BilateralFilter_h.vs", ".//Shader//BilateralFilter.fs");
    BilateralFilterShader_v = new Shader(".//Shader//BilateralFilter_v.vs", ".//Shader//BilateralFilter.fs");
    gammaCorrectionShader = new Shader(".//Shader//PostEffect.vs", ".//Shader//GammaCorrection.fs");
    Calcradiance = new ComputeShader(".//Shader//CalaRadiance.comp",glm::vec3(voxel_resolution/8.0f, voxel_resolution / 8.0f, voxel_resolution / 8.0f),glm::vec2(0,0), 0, 0, 4);
    ClearVoxelMap = new ComputeShader(".//Shader//ClearVoxelMap.comp", glm::vec3(voxel_resolution / 8.0f, voxel_resolution / 8.0f, voxel_resolution / 8.0f), glm::vec2(0, 0), 0, 0, 4);
    MipmapProduceFirst = new ComputeShader(".//Shader//MipmapProduceFirst.comp", glm::vec3(voxel_resolution / 16.0f, voxel_resolution / 16.0f, voxel_resolution / 16.0f), glm::vec2(0, 0), 0, 0, 7);
    MipmapProduceSecond = new ComputeShader(".//Shader//MipmapProduceSecond.comp", glm::vec3(voxel_resolution / 32.0f, voxel_resolution / 32.0f, voxel_resolution / 32.0f), glm::vec2(0, 0), 0, 0, 12,false,true,std::vector<int>(12,0));
    VoxelConeTracing=new ComputeShader(".//Shader//ConeTracing.comp", glm::vec3(voxel_resolution / 8.0f, voxel_resolution / 8.0f, voxel_resolution / 8.0f), glm::vec2(0, 0), 0, 0, 9);
    
   blit2 = new PostEffect(*BilateralFilterShader_h, width, height , false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    blit1 = new PostEffect(*BilateralFilterShader_v, width , height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
   // blit2 = new PostEffect(*GaussianBlurShader_h, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    //blit1 = new PostEffect(*GaussianBlurShader_v, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    blit = new PostEffect(*screenShader, CSwidth, CSheight, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    copy = new PostEffect(*screenShader, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    voxelization = new PostEffect(*screenShader, voxel_resolution, voxel_resolution, true, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    rasterization = new PostEffect(*rasterizationShader, voxel_resolution, voxel_resolution, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    posteffect = new PostEffect(*gammaCorrectionShader, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    lightPass = new PostEffect(*deferVoxelConeTracing,width,height,false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    AddInDirLight = new PostEffect(*AddShader, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    //voxel_diffuse = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution,true,GL_RGBA32F,GL_RGBA,GL_FLOAT);
    //voxel_normal = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    //voxel_radiance = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_RGBA32F, GL_RGBA, GL_FLOAT,2);
    voxel_diffuse = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    voxel_normal = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    voxel_radiance = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 2);
    voxel_static_mark = new Texture3D(voxel_resolution, voxel_resolution, voxel_resolution, true, GL_R8UI, GL_RED, GL_UNSIGNED_BYTE);
    shadowwidth = 2 * width;
    shadowheight = 2 * height;
    depthMap = new  FBO(shadowwidth, shadowheight, "_depthTexture", FBO::Depth);
    depthMapDefer = new FBO(shadowwidth, shadowheight, "_depthTexture", FBO::Depth);
    temp1 = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    temp2 = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    voxelRT = new FBO(width, height, "_voxelMap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT,true);
 
    





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
    voxel_static_mark->Bind();
    voxel_radiance->Bind();
    voxel_diffuse->Bind();
    voxel_normal->Bind();

    gBuffer->setupFBO();
    lightPass->BindQuad();
    posteffect->BindQuad();
    blit->BindQuad();
    blit1->BindQuad();
    blit2->BindQuad();
    copy->BindQuad();
    voxelization->BindQuad();
    rasterization->BindQuad();
    AddInDirLight->BindQuad();
    cubemap->Bind();



    // framebuffer configuration
    AddInDirLight->SetFrameBuffer();
    lightPass->SetFrameBuffer();
    rasterization->SetFrameBuffer();
    posteffect->SetFrameBuffer();
    blit->SetFrameBuffer();
    blit1->SetFrameBuffer();
    blit2->SetFrameBuffer();
    copy->SetFrameBuffer();
    voxelization->SetFrameBuffer();
    ubotest = new UBO(2 * sizeof(glm::mat4));
  
    shaders = vector<Shader>{ *lightshader, *myshader,*FloorShader,*myshader_gs,*drawVoxel,*Gemetory_Pass,*voxelShaderRender };
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
    voxel_rt[3] = *voxel_static_mark;
    voxel_rt[3].nameInShader = "voxelMap_staticMark";
    voxelWorldMinPoint = camera.getVoxelMinPoint(glm::vec3(0, 0, 0), voxel_width_world);
    voxelWorldMaxPoint = camera.getVoxelMaxPoint(glm::vec3(0, 0, 0), voxel_width_world);
    Calcradiance->texes3d = voxel_rt;
    VoxelConeTracing->texes3d[1]= *voxel_diffuse;
    VoxelConeTracing->texes3d[2] = *voxel_normal;
    VoxelConeTracing->texes3d[0] = *voxel_radiance;

    for (int i = 0; i < 6; i++) {
        voxel_anisotropicmipmap[i] = new Texture3D(voxel_resolution / 2.0, voxel_resolution / 2.0, voxel_resolution / 2.0, true, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
        voxel_anisotropicmipmap[i]->Bind();
        voxel_anisotropicmipmap[i]->nameInShader = "voxelMipMap[" + to_string(i) + "]";
        MipmapProduceSecond->texes3d[i] = *voxel_anisotropicmipmap[i];
        MipmapProduceSecond->lods[i] = 1;

    }
    for (int i = 0; i < 6; i++) {
      
      
        MipmapProduceFirst->texes3d[i] = *voxel_anisotropicmipmap[i];
        MipmapProduceSecond->texes3d[i+6] = *voxel_anisotropicmipmap[i];
        VoxelConeTracing->texes3d[i+3]= *voxel_anisotropicmipmap[i];
    }
    

    MipmapProduceFirst->texes3d[6] = *voxel_radiance;

   
   
}

void VoxelProcess::Process()
{
    /*   glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
    {
        //CalculateDeltaTime();
        if (EnableVoxelization) {
            ClearVoxelMap->texes3d = voxel_rt;
            ClearVoxelMap->use();
            ClearVoxelMap->dispatch();
            ClearVoxelMap->wait();
        }
        /*CalculateDeltaTime();
        cout << to_string( 1000 * deltaTime) + "Clear Voxel Map"<< endl;*/
    }

    ubotest->ClearSizeNow();
    float screenWidth = width;
    float screenHeight = height;
    glm::mat4 modelskyBox = glm::mat4(1.0f);
    modelskyBox = glm::scale(modelskyBox, glm::vec3(100.0f));
    glm::mat4 projection = glm::mat4(1.0f);;
    projection = glm::perspective(glm::radians(camera.GetFov()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    projection = camera.GetProjection();
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
   model = glm::translate(model, glm::vec3(ScaleUse.x, -ScaleUse.y, ScaleUse.z));
  ;
   // model = glm::scale(model, glm::vec3(0.8f));
  float tempScale = 0.008f;
  //tempScale = 0.04f;
    model = glm::scale(model, glm::vec3(tempScale));
   // model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   // model = glm::scale(model, glm::vec3(4.0f));
    glm::mat4 model_inverse_t = glm::mat4(1.0f);
    model_inverse_t = glm::transpose(glm::inverse(model));
    
    modelVoxel = glm::scale(modelVoxel, glm::vec3(Xscale));
    modelVoxel = glm::translate(modelVoxel, glm::vec3(-(voxel_resolution-1) * 0.5, -(voxel_resolution - 1) * 0.5, -(voxel_resolution - 1) * 0.5));

  /*  CalculateDeltaTime();*/
    camera.GenerateFrustum(projection*view);
    lights[3].camera.GenerateFrustum(lightProjection * lightView);
    ourModel->UpdateBoundingBox(glm::mat3(1.0),glm::vec3(ScaleUse.x, -ScaleUse.y, ScaleUse.z),glm::vec3(tempScale),true);
    VoxelCullCamera.GenerateFrustum(voxelViewProjction[0]);
   /* CalculateDeltaTime();
    cout << to_string(1000 * deltaTime) + "CPU" << endl;*/
    
    {
       
        glViewport(0, 0, width, height);

        
        ubotest->ClearSizeNow();

        gBuffer->BindFrameBufferInit();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT  | GL_STENCIL_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //glClear(GL_DEPTH_BUFFER_BIT);
        //glClearColor(1.0f,1.0f, 1.0f, 1.0f);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glDepthFunc(GL_LEQUAL);
        ubotest->SetMatrix(projection);
        ubotest->SetMatrix(view);//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
        glDepthFunc(GL_LESS);

        Gemetory_Pass->use();
        Gemetory_Pass->setMatrix("model", model);
        Gemetory_Pass->setMatrix("mvp", projection*view*model);
        Gemetory_Pass->setVec3("baseColor", glm::vec3(0.5, 0.5, 0.5));
        Gemetory_Pass->setVec3("cameraPos", camera.GetPos());
        Gemetory_Pass->setMatrix("lightMat", (lightProjection * lightView));
        Gemetory_Pass->setMatrix("model_inverse_t", (model_inverse_t));
        for (unsigned int i = 0; i < 4; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            lights[i].SetLight(*Gemetory_Pass);
        }
        //CalculateDeltaTime();
        ourModel->Draw(*Gemetory_Pass, vector<Texture>(0), nullptr, vector<Texture3D>(0),&camera);
        ubotest->ClearSizeNow();
        gBuffer->BindFrameBufferOver();
  /*      CalculateDeltaTime();
        cout << to_string(1000 * deltaTime) + "gBuffer" << endl;*/
        
    }//2ms
     
    
   // CalculateDeltaTime();
    if (EnableVoxelization) 
    {
        glViewport(0, 0, voxel_resolution, voxel_resolution);
        voxelization->BindFrameBufferInit();
        ubotest->SetMatrix(projection);
        ubotest->SetMatrix(view);//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
        myshader_gs->setInt("staticFlag", 1);
        for (unsigned int i = 0; i < 4; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            lights[i].SetLight(*myshader_gs);
        }
       //if(Time<2.0f)
        voxel_rt[0].format = GL_R32UI;
        voxel_rt[1].format = GL_R32UI;
  
            ourModel->Draw(*myshader_gs, rt, nullptr, voxel_rt, &VoxelCullCamera);
  
        voxel_rt[0].format = GL_RGBA8;
        voxel_rt[1].format = GL_RGBA8;
        //myshader_gs->setMatrix("model", floormodel);
        //roomModel->Draw(*myshader_gs, rt, nullptr, voxel_rt);
        ubotest->ClearSizeNow();
        voxelization->BindFrameBufferOver();//1.8ms
  
        /*CalculateDeltaTime();
        std::cout << to_string(1000 * deltaTime) + "voxelization" << endl;*/
       // EnableVoxelization = false;
    }
 
    //CalculateDeltaTime();
    glViewport(0, 0, shadowwidth, shadowheight);
    depthMapDefer->BindFrameBufferInit();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    glDisable(GL_CULL_FACE);
    ubotest->SetMatrix(projection);
    ubotest->SetMatrix(view);//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
    depthShader->use();
    depthShader->setMatrix("lightProjection", lightProjection);
    depthShader->setMatrix("lightView", lightView);
    depthShader->setMatrix("model", model);
    ourModel->Draw(*depthShader,std::vector<Texture>(0),nullptr, std::vector<Texture3D>(0),&lights[3].camera);

    depthMapDefer->BindFrameBufeerOver();
    ubotest->ClearSizeNow();
 /*   CalculateDeltaTime();
    cout << to_string(1000 * deltaTime) + "depthMap" << endl;*/
   

    rt[0] = depthMapDefer->texture;//0.832
 

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
    
    if (limit.x > 0.3) 
    {
        glViewport(0, 0, width, height);
        posteffect->BindFrameBufferInit();


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
    }
    

     
    
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


  /*  CalculateDeltaTime();*/
    if (updateRadianceNow == updateRadianceCount) {
        glm::mat4 modelVoxel_inverse_t = glm::transpose(glm::inverse(modelVoxel));
        Calcradiance->use();
        Calcradiance->setMatrix("lightMat", (lightProjection * lightView));
        Calcradiance->setMatrix("model", modelVoxel);
        Calcradiance->setMatrix("model_inverse_t", (modelVoxel_inverse_t));
        Calcradiance->setFloat("voxelResolution", voxel_resolution);
        Calcradiance->setInt("voxelResolutionI", voxel_resolution);
        Calcradiance->setFloat("voxelWidthWorld", voxel_width_world);
        Calcradiance->setVec3("worldMinPoint", voxelWorldMinPoint);
        glActiveTexture(GL_TEXTURE0 + 3);
        Calcradiance->setInt(rt[0].nameInShader, 3);
        Calcradiance->setInt("openNormalBlend", normalBlend);
        glBindTexture(GL_TEXTURE_2D, rt[0].id);
        for (unsigned int i = 0; i < 4; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            lights[i].SetLight(*Calcradiance);
        }

        Calcradiance->dispatch();
        Calcradiance->wait();//0.85ms
    }
    /*     CalculateDeltaTime();
    cout << to_string(1000 * deltaTime) + "Calcraidance" << endl;*/

    
    


   
    lightshader->use();
    lightshader->setVec3("lightColor", lightColor);
    glm::mat4 modellight = glm::mat4(1.0f);
    modellight = glm::scale(modellight, glm::vec3(1.0f));
    modellight = glm::translate(modellight, lights[3].pos);
 
    lightshader->setMatrix("model", modellight);
    lights[3].Draw();  //draw cube


    if (updateRadianceNow==updateRadianceCount) {
        GenerateMipmap();//0.2ms



        if (InjectFirstBounce)
        {
            VoxelConeTracing->use();
            VoxelConeTracing->setInt("voxelResolution", voxel_resolution);
            VoxelConeTracing->setFloat("IndirectLight", IndirectLight);
            VoxelConeTracing->setFloat("directLight", directLight);
            VoxelConeTracing->dispatch();
            VoxelConeTracing->wait();

            GenerateMipmap();
        }
        updateRadianceNow =0.0;
    }
    EnableVoxelization = false;
                
   if (limit.x > 0.3) 
   {
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
   }
    





    posteffect->BindFrameBufferOver();



    posteffect->Draw();

    

    if (limit.x < 0.3) 
    {   
       
        {
            CalculateDeltaTime();
            blit1->BindFrameBufferInit();
            glViewport(0, 0, width, height);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            lightPass->UseShader();
            for (int i = 0; i < 4; i++) {
                glActiveTexture(GL_TEXTURE0 + i + 1);
                lightPass->shader.setInt(gBuffer->gBufferTex[i].nameInShader, i + 1);
                glBindTexture(GL_TEXTURE_2D, gBuffer->gBufferTex[i].id);
            }
            for (unsigned int i = 0; i < 4; i++)
            {
                // calculate the model matrix for each object and pass it to shader before drawing
                lights[i].SetLight(lightPass->shader);
            }
            glActiveTexture(GL_TEXTURE0 + 5);
            voxel_radiance->nameInShader = "voxelMap_radiance";
            lightPass->shader.setInt(voxel_radiance->nameInShader, 5);
            glBindTexture(GL_TEXTURE_3D, voxel_radiance->id);

            glActiveTexture(GL_TEXTURE0 + 6);
            depthMapDefer->texture.nameInShader = "shadowMap";
            lightPass->shader.setInt(depthMapDefer->texture.nameInShader, 6);
            glBindTexture(GL_TEXTURE_2D, depthMapDefer->texture.id);
            for (int i = 0; i < 6; i++) {
                glActiveTexture(GL_TEXTURE0 + 7 + i);
                lightPass->shader.setInt(voxel_anisotropicmipmap[i]->nameInShader, 7 + i);
                glBindTexture(GL_TEXTURE_3D, voxel_anisotropicmipmap[i]->id);
            }
            glActiveTexture(GL_TEXTURE0 + 13);
            voxel_normal->nameInShader = "voxelMap_normal";
            lightPass->shader.setInt(voxel_normal->nameInShader, 13);
            glBindTexture(GL_TEXTURE_3D, voxel_normal->id);

            lightPass->shader.setMatrix("lightMat", (lightProjection * lightView));
            lightPass->shader.setInt("voxelResolutionI", voxel_resolution);
            lightPass->shader.setFloat("voxelWidthWorld", voxel_width_world);
            lightPass->shader.setVec3("worldMinPoint", voxelWorldMinPoint);
            lightPass->shader.setVec3("worldMaxPoint", voxelWorldMaxPoint);
            lightPass->shader.setVec3("cameraPos", camera.GetPos());
            lightPass->shader.setFloat("deferDirLight", deferDirLight);
            lightPass->shader.setFloat("deferInDirLight", deferInDirLight);
            lightPass->shader.setMatrix("inverseProjectionView", glm::inverse(projection * view));
            lightPass->shader.setFloat("maxDistance", maxdistance);
            lightPass->shader.setFloat("stepLength", stepLength);
            lightPass->shader.setFloat("Aperture", aperture);
            lightPass->shader.setFloat("occ_falloff", occ_falloff);
            lightPass->shader.setBool("enableAmbientOcc", EnableAmbientOcc);
            lightPass->DrawQuad(0, false);
            blit1->Blit(*temp1);
            blit1->BindFrameBufferOver();
        }//get Indir Light
       
        //{
        //    blit1->BindFrameBufferInit();
        //    glViewport(0, 0, width, height);
        //    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        //    glClear(GL_COLOR_BUFFER_BIT);
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //    lightPass->UseShader();
        //    for (int i = 0; i < 4; i++) {
        //        glActiveTexture(GL_TEXTURE0 + i + 1);
        //        lightPass->shader.setInt(gBuffer->gBufferTex[i].nameInShader, i + 1);
        //        glBindTexture(GL_TEXTURE_2D, gBuffer->gBufferTex[i].id);
        //    }
        //    for (unsigned int i = 0; i < 4; i++)
        //    {
        //        // calculate the model matrix for each object and pass it to shader before drawing
        //        lights[i].SetLight(lightPass->shader);
        //    }
        //    glActiveTexture(GL_TEXTURE0 + 5);
        //    voxel_radiance->nameInShader = "voxelMap_radiance";
        //    lightPass->shader.setInt(voxel_radiance->nameInShader, 5);
        //    glBindTexture(GL_TEXTURE_3D, voxel_radiance->id);

        //    glActiveTexture(GL_TEXTURE0 + 6);
        //    depthMapDefer->texture.nameInShader = "shadowMap";
        //    lightPass->shader.setInt(depthMapDefer->texture.nameInShader, 6);
        //    glBindTexture(GL_TEXTURE_2D, depthMapDefer->texture.id);
        //    for (int i = 0; i < 6; i++) {
        //        glActiveTexture(GL_TEXTURE0 + 7 + i);
        //        lightPass->shader.setInt(voxel_anisotropicmipmap[i]->nameInShader, 7 + i);
        //        glBindTexture(GL_TEXTURE_3D, voxel_anisotropicmipmap[i]->id);
        //    }
        //    lightPass->shader.setMatrix("lightMat", (lightProjection * lightView));
        //    lightPass->shader.setInt("voxelResolutionI", voxel_resolution);
        //    lightPass->shader.setFloat("voxelWidthWorld", voxel_width_world);
        //    lightPass->shader.setVec3("worldMinPoint", voxelWorldMinPoint);
        //    lightPass->shader.setVec3("worldMaxPoint", voxelWorldMaxPoint);
        //    lightPass->shader.setVec3("cameraPos", camera.GetPos());
        //    lightPass->shader.setFloat("deferDirLight", deferDirLight);
        //    lightPass->shader.setFloat("deferInDirLight", 0);
        //    lightPass->shader.setMatrix("inverseProjectionView", glm::inverse(projection * view));
        //    lightPass->shader.setFloat("maxDistance", maxdistance);
        //    lightPass->shader.setFloat("stepLength", stepLength);
        //    lightPass->shader.setFloat("Aperture", aperture);
        //    lightPass->shader.setFloat("occ_falloff", occ_falloff);
        //    lightPass->shader.setBool("enableAmbientOcc", EnableAmbientOcc);
        //    lightPass->DrawQuad(0, false);
        //    blit1->Blit(*temp2);
        //    blit1->BindFrameBufferOver();
        //  
        //} //get Dir Light
        //
        //if (limit.x > 0.2) 
        //{
        //    for (int i = 0; i < 0; i++)
        //    {
        //        blit1->BindFrameBufferInit();
        //        glViewport(0, 0, width, height);
        //        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        //       // glClear(GL_COLOR_BUFFER_BIT);
        //        glDepthFunc(GL_ALWAYS);
        //        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //        //glDepthFunc(GL_LEQUAL);
        //        blit1->UseShader();
        //        blit1->shader.setFloat("texelsize_v", (float)1 / (float)height);
        //        blit1->DrawQuad(temp1->texture.id, false);
        //        blit1->Blit(*temp1);
        //        blit1->BindFrameBufferOver();

        //        blit2->BindFrameBufferInit();
        //        glViewport(0, 0, width, height);
        //        glClearColor(0.0f, 0.0f, 0.0, 0.0f);
        //        glDepthFunc(GL_ALWAYS);
        //      //  glClear(GL_COLOR_BUFFER_BIT);
        //        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //        //glDepthFunc(GL_LEQUAL);
        //        blit2->UseShader();
        //        blit2->shader.setFloat("texelsize_h", (float)1 / (float)width);
        //        blit2->DrawQuad(temp1->texture.id, false);
        //        blit2->Blit(*temp1);
        //        blit2->BindFrameBufferOver();
        //        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        //    }
        //    AddInDirLight->BindFrameBufferInit();
        //    glViewport(0, 0, width, height);
        //    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        //    // glClear(GL_COLOR_BUFFER_BIT);
        //    glDepthFunc(GL_ALWAYS);
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //    AddInDirLight->UseShader();
        //    glActiveTexture(GL_TEXTURE0 + 1);
        //    temp1->texture.nameInShader = "IndirLightTexture";
        //    AddInDirLight->shader.setInt(temp1->texture.nameInShader, 1);
        //    glBindTexture(GL_TEXTURE_2D, temp1->texture.id);
        //    AddInDirLight->DrawQuad(temp2->texture.id, false);
        //    AddInDirLight->Blit(*temp1);
        //    blit1->BindFrameBufferOver();
        //  
        //}
     /*
        CalculateDeltaTime();
        cout << to_string(1000 * deltaTime) + "LightPass" << endl;*/
      posteffect->DebugDraw(temp1->texture.id);
      if(limit.x>0.25)
          posteffect->DebugDraw(depthMapDefer->texture.id);
       //copy->DebugDraw(gBuffer->gNormal);
       // posteffect->DebugDraw(depthMapDefer->fboTexture);
      // lights[3].Draw();
   
    }

  // posteffect->DebugDraw(voxelRT->texture.id);
  // posteffect->DebugDraw(voxelization->screenTexture);

    //posteffect->DebugDraw(normalShader->texes_output[3].id);
  ///posteffect->DebugDraw(normalShader->texes_output[4].id);
    //posteffect->DebugDraw(DisplacementShader->texes_output[3].id);
    //posteffect->DebugDraw(WaveParticleShader->texes_output[0].id);
    // glViewport(0, 0, width, height);
     //posteffect->DebugDraw(WaveParticleShader->texes_output[0].initid);
    updateRadianceNow++;
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
        ImGui::SliderFloat3(("ScaleUse"), (float*)&(ScaleUse), 0.0f, 20.0f);
        ImGui::SliderFloat("Aperture", &aperture, 0.5f, 2.0f);
        ImGui::SliderFloat("OccFallOff", &occ_falloff, 700, 900);
        ImGui::SliderFloat("Conventional Rasterization", &RasterScale, 0.0f, 20.0f);
        ImGui::SliderFloat("Xscale", &Xscale, 0.0f, 1.0f);
        ImGui::SliderInt("NormalBlend", &normalBlend, 0.0, 1.0);
        ImGui::SliderInt("VoxelNormalBlend", &voxelnormalBlend, 0.0, 1.0);
        ImGui::SliderFloat("IndirectLight", &IndirectLight, 0.0, 5.0);
        ImGui::SliderFloat("directLight", &directLight, 0.0, 5.0);
        ImGui::Checkbox("InjectFirstBounce", &InjectFirstBounce);
        ImGui::Checkbox("EnableAmbientOcc", &EnableAmbientOcc);
        ImGui::Checkbox("EnableVoxelization", &EnableVoxelization);
        ImGui::SliderFloat("deferDirLight", &deferDirLight,0.0,2.0);
        ImGui::SliderFloat("deferInDirLight", &deferInDirLight, 0.0, 10.0);
        ImGui::SliderFloat("maxdistance", &maxdistance, 0.0, 10.0);
        ImGui::SliderFloat("stepLength", &stepLength, 0.1, 2.0);
       
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

void VoxelProcess::GenerateMipmap()
{
    int lod=0;
    int resolution = voxel_resolution/4.0;
    MipmapProduceFirst->texes3d[6].enableTextureImage = false;
    MipmapProduceFirst->use();

    MipmapProduceFirst->dispatch();
    MipmapProduceFirst->wait();
    MipmapProduceFirst->texes3d[6].enableTextureImage = true;
    while (resolution >= 1) 
    {
        
        for (int i = 0; i < 6; i++) 
        {
            MipmapProduceSecond->lods[i] = lod+1;
            MipmapProduceSecond->lods[i+6] = lod;
        }
        
        int val = pow(2, lod+1);
        int size = glm::ceil( (float)voxel_resolution / (16.0 * (float)val));
        MipmapProduceSecond->work_size = glm::vec3(size,size,size);
        MipmapProduceSecond->use();
        MipmapProduceSecond->setInt("mipmaplevel", lod);
        MipmapProduceSecond->dispatch();
        MipmapProduceSecond->wait();
        resolution /= 2.0;
        lod++;
    }
    MipmapProduceSecond->work_size = glm::vec3(voxel_resolution / (16 * 2));
   
}


void VoxelProcess::CalculateDeltaTime() {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

}
VoxelProcess::~VoxelProcess()
{
    std::cout << "delete Over";
    delete deferVoxelConeTracing;
    delete Gemetory_Pass;
    delete drawVoxel;
    delete myshader;
    delete voxelShaderRender;
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
    delete BilateralFilterShader_h;
    delete BilateralFilterShader_v;
    delete Calcradiance;
    delete ClearVoxelMap;
    delete MipmapProduceFirst;
    delete MipmapProduceSecond;
    delete VoxelConeTracing;
    delete AddShader;
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
    delete gBuffer;
    delete gammaCorrectionShader;
    delete light;
    delete pointLight;
    delete pointLight1;
    delete spotLight;
    delete cubemap;
    delete ubotest;
    delete depthMapDefer;
    delete depthMap;
    delete pool;
    delete voxel_diffuse;
    delete voxel_normal;
    delete voxel_radiance;
    delete voxel_static_mark;
    delete lightPass;
    delete AddInDirLight;
    for (int i = 0; i < 6; i++) {
        delete voxel_anisotropicmipmap[i];
    
    }
    auto temp = std::vector<glm::mat4>(0);
    voxelViewProjction.swap(temp);
    voxelViewProjctionInverse.swap(temp);
    // delete bubbleNormal;

}
