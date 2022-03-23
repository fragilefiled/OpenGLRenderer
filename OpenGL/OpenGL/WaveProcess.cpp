#include"WaveProcess.h"
WaveProcess::WaveProcess(Camera& camera)
{
    this->camera = camera;
}
void WaveProcess::SetCameraAndTime(Camera& camera, float time)
{
    this->camera = camera;
    this->Time = time;
}
void WaveProcess::Init()
{
    showwindow = true;
   
    bubbleDiffuse = new Texture(".//Model//Foam//BubbleDiffuse2.jpg",Texture::Normal);
    //bubbleNormal = new Texture(".//Model//Foam//BubbleNormal1.jpg", Texture::Normal);
     myshader = new Shader(".//Shader//MyVertexShader.vs", ".//Shader//LightingCubeShader.fs");
    // Shader lightshader = Shader(".//Shader//MyVertexShader.vs", ".//Shader//lightshader->fs", ".//Shader//lightshader->gs");
   lightshader = new Shader(".//Shader//LightShader.vs", ".//Shader//LightShader.fs");
    screenShader = new Shader(".//Shader//PostEffect.vs", ".//Shader//PostEffect.fs");
    InstanceShader = new Shader(".//Shader//InstanceShader.vs", ".//Shader//LightingCubeShader.fs");
    FloorShader =new Shader(".//Shader//floorShader.vs", ".//Shader//floorShader.fs");
    //  Shader OceanShader = Shader(".//Shader//oceanShader.vs", ".//Shader//oceanShader.fs");
    roomModel = new Model(".//Model//floor//floor3.obj");
   ourModel=new Model(".//Model//nanosuit//nanosuit.obj");

    //delete roomModel;
    //Model sponzaModel(".//Model//sponza//sponza.obj");
    depthShader =new Shader(".//Shader//DepthShader.vs", ".//Shader//DepthShader.fs");
    depthOceanShader = new Shader(".//Shader//DepthOceanShader.vs", ".//Shader//DepthShader.fs");
    // Model ourModel(".//Model//planet//planet.obj");
    //  Model ourModel1(".//Model//rock//rock.obj",true,modelMatrices,amount);
     //Model ourModel1(".//Model//rock//rock.obj");
   skyBoxShader = new Shader(".//Shader//CubeMap.vs", ".//Shader//CubeMap.fs");
    GaussianShader = new ComputeShader(".//Shader//Test.comp", glm::vec3(CSwidth, CSheight,1.0));
   HeightSpectrumShader = new ComputeShader(".//Shader//Height.comp", glm::vec3(CSwidth, CSheight, 1.0));
    GaussianBlurShader_h = new Shader(".//Shader//PostEffect_gaussian_h.vs", ".//Shader//GaussionBlur_h.fs");
     GaussianBlurShader_v = new Shader(".//Shader//PostEffect_gaussian_v.vs", ".//Shader//GaussionBlur_h.fs");
    AddDisplacementShader = new  Shader(".//Shader//PostEffect.vs", ".//Shader//AddDisplacement.fs");
    WaveParticleShader_h = new Shader(".//Shader//WaveParticle_h.vs", ".//Shader//WaveParticle_h.fs");
     WaveParticleShader_v = new Shader(".//Shader//WaveParticle_h.vs", ".//Shader//WaveParticle_v.fs");
     gammaCorrectionShader = new Shader(".//Shader//PostEffect.vs", ".//Shader//GammaCorrection.fs");
    DrawPoint = new Shader(".//Shader//DrawPoint.vs", ".//Shader//DrawPoint.fs");
    //  Shader GroundShader = Shader(".//Shader//ground.vs", ".//Shader//ground.fs");
    OffsetSpectrumShader = new ComputeShader(".//Shader//Offset.comp", glm::vec3(CSwidth, CSheight, 1.0), 2);
    FftXShader = new ComputeShader(".//Shader//Fftx.comp", glm::vec3(CSwidth, CSheight, 1.0), 3, 3);
     FftYShader = new ComputeShader(".//Shader//Ffty.comp", glm::vec3(CSwidth, CSheight, 1.0), 3, 3);
    FftXendShader = new ComputeShader(".//Shader//Fftxend.comp", glm::vec3(CSwidth, CSheight, 1.0), 3, 3);
     FftYendShader = new ComputeShader(".//Shader//Fftyend.comp", glm::vec3(CSwidth, CSheight, 1.0), 3, 3);
     DisplacementShader = new ComputeShader(".//Shader//Displacement.comp", glm::vec3(CSwidth, CSheight, 1.0), 4, 3);
     normalShader = new ComputeShader(".//Shader//normalCalc.comp", glm::vec3(CSwidth, CSheight, 1.0), 5, 1);
     WaveParticleShader = new ComputeShader(".//Shader//WaveParticle.comp", glm::vec3(wave_particle_resolution, wave_particle_resolution, 1.0), 1, 2, 0, false);
     GaussianBlur_h = new ComputeShader(".//Shader//GaussianBlur_h.comp", glm::vec3(wave_particle_resolution, wave_particle_resolution, 1.0), 1, 1);
     GaussianBlur_v = new ComputeShader(".//Shader//GaussianBlur_v.comp", glm::vec3(wave_particle_resolution, wave_particle_resolution, 1.0), 1, 1);
     GaussianBlur_h1 = new ComputeShader(".//Shader//GaussianBlur_h.comp", glm::vec3(width, height, 1.0), 1, 1);
     GaussianBlur_v1 = new ComputeShader(".//Shader//GaussianBlur_v.comp", glm::vec3(width, height, 1.0), 1, 1);
     normalShader1 = new ComputeShader(".//Shader//normalCalc.comp", glm::vec3(width, height, 1.0), 5, 1);
     blit6 = new PostEffect(*WaveParticleShader_v, wave_particle_resolution_fliter, wave_particle_resolution_fliter, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     blit5 = new PostEffect(*WaveParticleShader_h, wave_particle_resolution_fliter, wave_particle_resolution_fliter, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     blit4 = new PostEffect(*screenShader, B_Spine_count, B_Spine_count, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     blit3 = new PostEffect(*AddDisplacementShader, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     blit2 = new PostEffect(*GaussianBlurShader_h, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     blit1 = new PostEffect(*GaussianBlurShader_v, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     blit = new PostEffect(*screenShader, wave_particle_resolution, wave_particle_resolution, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     copy = new PostEffect(*screenShader, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     copyWaveParticle= new PostEffect(*screenShader, wave_particle_resolution_fliter, wave_particle_resolution_fliter, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     posteffect = new PostEffect(*gammaCorrectionShader, width, height, true, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     

     shadowwidth = 2 * width;
     shadowheight = 2 * height;
     depthMap = new  FBO(shadowwidth, shadowheight, "_depthTexture", FBO::Depth);
     wavemap = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     temp1 = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     temp2 = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     temp3 = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     temp4 = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     temp5 = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     temp6 = new FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    temp7 = new FBO(wave_particle_resolution_fliter, wave_particle_resolution_fliter, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
     temp8 = new FBO(wave_particle_resolution_fliter, wave_particle_resolution_fliter, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);

    // vector<glm::vec2> control = vector<glm::vec2>({ glm::vec2(0,0),glm::vec2(11,0),glm::vec2(17,8),glm::vec2(24,10),glm::vec2(26,19),glm::vec2(26,0),glm::vec2(26,0),glm::vec2(26,0),glm::vec2(40,0),glm::vec2(45,0) });
    vector<glm::vec2> control = vector<glm::vec2>({ glm::vec2(0,0),glm::vec2(11,0),glm::vec2(17,8),glm::vec2(24,18),glm::vec2(26,19),glm::vec2(26.2,0),glm::vec2(30,0) });
    //  vector<glm::vec2> control = vector<glm::vec2>({ glm::vec2(0,5),glm::vec2(11,7),glm::vec2(17,8),glm::vec2(24,18),glm::vec2(26,19),glm::vec2(26.2,0),glm::vec2(30,0) });
    B_Spine b_spine = B_Spine(B_Spine_count, 4, control, vector<double>(0));
    b_spine.generate_u();
    b_spine.produce_height_array();


    particleImage =new TextureImage(Input_N, Input_N);
    particleImage->Bind();
    particleImage1 = new TextureImage(Input_N, Input_N);
    particleImage1->Bind();
     waveImage1 = new TextureImage(B_Spine_count, 1);
    waveImage1->Bind();
    waveImage1->setData(b_spine.y_array);
    auto temp = std::vector<float>(0);
    b_spine.y_array.swap(temp);





    glm::vec3 lightPos(0.0f, 10.0f, 4.0f);
    glm::vec3 lightPos1(0.0f, 3.0f, 2.0f);
    glm::vec3 lightDir = lightPos;
    glm::vec4 lightColor(1, 1, 1, 0);

    light=new Light(DIRLIGHT, glm::vec3(0.0, 15.0, 1.0), -glm::vec3(0.0, 0.4, 0.5), lightColor);
    pointLight= new Light(POINTLIGHT, lightPos, -lightDir, lightColor);
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

    if (enableWaveParticle) {

        jacobScale = 130.0f;
        BubblesThreshold = 1.0f;


    }


    //unsigned int pointVAO, pointVBO;

    
    bubbleDiffuse->Bind();
    posteffect->BindQuad();
    blit->BindQuad();
    blit1->BindQuad();
    blit2->BindQuad();
    blit3->BindQuad();
    blit4->BindQuad();
    blit5->BindQuad();
    blit6->BindQuad();
    copy->BindQuad();

    copyWaveParticle->BindQuad();
    cubemap->Bind();
    


    // framebuffer configuration
    posteffect->SetFrameBuffer();
    blit->SetFrameBuffer();
    blit1->SetFrameBuffer();
    blit2->SetFrameBuffer();
    blit3->SetFrameBuffer();
    blit4->SetFrameBuffer();
    blit5->SetFrameBuffer();
    blit6->SetFrameBuffer();
    copy->SetFrameBuffer();
    copyWaveParticle->SetFrameBuffer();
    ubotest = new UBO(2 * sizeof(glm::mat4));

    shaders=vector<Shader>{ *lightshader, *myshader,*FloorShader };
    ubotest->SetUnifromBlockIndexAndBind(shaders, "Matrices");
    ubotest->BindUniformBuffer();
    if (enableWaveParticle) {

        jacobScale = 391.f;
        if (!enableWaveParticle_hvfliter)
            scale_Wave = 0.9f;

    }
    
    if (enableWaveParticle_hvfliter)
        pool =new Wave_Particle_Pool(Input_N * Input_N, wave_particle_resolution_fliter,enableWaveParticle_hvfliter);
    else
        pool = new Wave_Particle_Pool(Input_N * Input_N, wave_particle_resolution);
    pointVAO = pool->pointVAO;
    pointVBO = pool->pointVBO;
    srand((int)time(0));
     particle_num = 700;
    if (!enableWaveParticle_hvfliter)
    {

        for (int i = 0; i < particle_num; i++)
        {
            int randomNum = rand() % wave_particle_resolution;;
            int randomNum3 = rand() % wave_particle_resolution;;
            float randomNum1 = rand() % 64 / 64.0f + 0.01f;;
            float randomNum2 = rand() % 64 / 64.0f + 0.01f;;
            float height = 0.05f;
            int a = rand() % 3; int b = rand() % 3;
            if (a > 1)
                a = 1;
            else
                a = -1;
            if (b > 1)
                b = 1;
            else
                b = -1;
            pool->PushToParticles(glm::vec2(randomNum, randomNum3), glm::vec2(randomNum, randomNum3), glm::vec2(a * randomNum1, b * randomNum2), height, 5.0f, 60, 0.0);

        }
      //  pool->PushToParticles(glm::vec2(32, 32), glm::vec2(32, 32), glm::vec2(0.1, 0.1), 0.5f, 5.0f, 60, 0.0);
    }
    if (enableWaveParticle_hvfliter)
    {
      //  pool->PushToParticles(glm::vec2(wave_particle_resolution_fliter / 2.0, wave_particle_resolution_fliter / 2.0), glm::vec2(wave_particle_resolution_fliter / 2.0, wave_particle_resolution_fliter / 2.0), glm::vec2(0.005 * wave_particle_resolution_fliter, 0.005 * wave_particle_resolution_fliter), 10.0f, 10.0f, 60, 0.0);

        for (int i = 0; i < particle_num; i++)
        {
            int randomNum = rand() % wave_particle_resolution_fliter;;
            int randomNum3 = rand() % wave_particle_resolution_fliter;;
            float randomNum1 = rand() % 64 / 64.0f + 0.01f;;
            float randomNum2 = rand() % 64 / 64.0f + 0.01f;;
            float height = 0.1f;
            int a = rand() % 3; int b = rand() % 3;
            if (a > 1)
                a = 1;
            else
                a = -1;
            if (b > 1)
                b = 1;
            else
                b = -1;
            if (wave_particle_resolution_fliter == 256)
                height = 12.0 * height;
            else if (wave_particle_resolution_fliter == 512)
                height = 4.0 * height;
            else if(wave_particle_resolution_fliter == 1024)
                height = 1.0 * height;
           
            pool->PushToParticles(glm::vec2(randomNum, randomNum3), glm::vec2(randomNum, randomNum3), 100.0f * glm::vec2(a * randomNum1, b * randomNum2)*(float)((wave_particle_resolution_fliter)/1024.0), height, 5.0f, 60, 0.0);

        }
    }
    /* pool->PushToParticles(glm::vec2(64, 64), glm::vec2(64, 64), glm::vec2(0.1, 0.1), 0.3f, 5.0f, 60, 0.0);*/
     //pool->particles.push_back(Wave_Particle( glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(0.1, 0.1), 1.0, 2.5, 30, 0.0));
  
}

void WaveProcess::Process()
{
    /*   glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
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

    //  model = glm::rotate(model, glm::radians(70.f*(float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));


    for (int i = 0; i < FftXShader->texes_output.size(); i++) {
        FftXShader->texes_output[i].InitId();
        FftYShader->texes_output[i].InitId();
    }
    for (int i = 0; i < GaussianBlur_h1->texes_input.size(); i++) {
        GaussianBlur_h1->texes_input[0].InitId();
        GaussianBlur_h->texes_input[0].InitId();
        GaussianBlur_v1->texes_input[0].InitId();
        GaussianBlur_v->texes_input[0].InitId();
        GaussianBlur_h1->texes_output[0].InitId();
        GaussianBlur_h->texes_output[0].InitId();
        GaussianBlur_v1->texes_output[0].InitId();
        GaussianBlur_v->texes_output[0].InitId();

    }
    for (int i = 0; i < normalShader->texes_output.size(); i++) {
        normalShader->texes_output[i].InitId();

    }
    WaveParticleShader->texes_output[0].InitId();
    DisplacementShader->texes_output[3].InitId();

    if (!enableWaveParticle)
    {

        GaussianShader->use();
        GaussianShader->setInt("N", CSwidth);
        GaussianShader->dispatch();
        GaussianShader->wait();


        HeightSpectrumShader->texes_input[0] = GaussianShader->texes_output[0];

        // HeightSpectrumShader->setFloat("Time", Time);

        HeightSpectrumShader->use();
        HeightSpectrumShader->setInt("N", CSwidth);
        HeightSpectrumShader->setFloat("Time", 2 * Time);
        HeightSpectrumShader->setFloat("A", A);
        HeightSpectrumShader->setVec4("WindAndSeed", windspeed);
        HeightSpectrumShader->dispatch();
        HeightSpectrumShader->wait();

        OffsetSpectrumShader->texes_input[0] = HeightSpectrumShader->texes_output[0];
        OffsetSpectrumShader->use();
        OffsetSpectrumShader->setInt("N", CSwidth);
        OffsetSpectrumShader->dispatch();
        OffsetSpectrumShader->wait();

        FftXShader->texes_input[0] = OffsetSpectrumShader->texes_output[0];
        FftXShader->texes_input[1] = OffsetSpectrumShader->texes_output[1];
        FftXShader->texes_input[2] = OffsetSpectrumShader->texes_input[0];


              for (int i = 0; i < log2(CSwidth); i++)
           {
               int a = log2(CSwidth);
               int Ns = pow(2, i);
               if (i == log2(CSwidth) - 1)
               {
                   FftXendShader->texes_input[0] = FftXShader->texes_input[0];
                   FftXendShader->texes_input[1] = FftXShader->texes_input[1];
                   FftXendShader->texes_input[2] = FftXShader->texes_input[2];
                   FftXendShader->use();
                   FftXendShader->setInt("N", CSwidth);
                   FftXendShader->setInt("Ns", Ns);
                   FftXendShader->dispatch();
                   FftXendShader->wait();

               }
               else
               {
                   FftXShader->use();
                   FftXShader->setInt("N", CSwidth);
                   FftXShader->setInt("Ns", Ns);
                   FftXShader->dispatch();
                   FftXShader->wait();
                   FftXShader->texes_input[0].Swap(FftXShader->texes_output[0]);
                   FftXShader->texes_input[1].Swap(FftXShader->texes_output[1]);
                   FftXShader->texes_input[2].Swap(FftXShader->texes_output[2]);
               }

           }
           FftYShader->texes_input[0] = FftXendShader->texes_output[0];
           FftYShader->texes_input[1] = FftXendShader->texes_output[1];
           FftYShader->texes_input[2] = FftXendShader->texes_output[2];
           for (int i = 0; i < log2(CSwidth); i++)
           {
               int a = log2(CSwidth);
               int Ns = pow(2, i);
               if (i == log2(CSwidth) - 1)
               {
                   FftYendShader->texes_input[0] = FftYShader->texes_input[0];
                   FftYendShader->texes_input[1] = FftYShader->texes_input[1];
                   FftYendShader->texes_input[2] = FftYShader->texes_input[2];
                   FftYendShader->use();
                   FftYendShader->setInt("N", CSwidth);
                   FftYendShader->setInt("Ns", Ns);
                   FftYendShader->dispatch();
                   FftYendShader->wait();

               }
               else
               {
                   FftYShader->use();
                   FftYShader->setInt("N", CSwidth);
                   FftYShader->setInt("Ns", Ns);
                   FftYShader->dispatch();
                   FftYShader->wait();
                   FftYShader->texes_input[0].Swap(FftYShader->texes_output[0]);
                   FftYShader->texes_input[1].Swap(FftYShader->texes_output[1]);
                   FftYShader->texes_input[2].Swap(FftYShader->texes_output[2]);
               }

           }
        DisplacementShader->texes_input[0] = FftYendShader->texes_output[0];
        DisplacementShader->texes_input[1] = FftYendShader->texes_output[1];
        DisplacementShader->texes_input[2] = FftYendShader->texes_output[2];
        DisplacementShader->use();
        DisplacementShader->setVec3("limit", limit);
        DisplacementShader->setInt("N", CSwidth);
        DisplacementShader->dispatch();
        DisplacementShader->wait();


        normalShader->texes_input[0] = DisplacementShader->texes_output[3];

        normalShader->use();
        normalShader->setFloat("BubblesScale", BubblesScale);
        normalShader->setFloat("BubblesThreshold", BubblesThreshold);
        normalShader->setFloat("jacobScale", jacobScale);
        normalShader->setFloat("jacobScaleEdge", jacobScaleEdge);
        normalShader->setFloat("resolutioninv", 2.0f * scale_Ocean / (float)CSwidth);//平面的长度为2
        normalShader->dispatch();
        normalShader->wait();

        for (int i = 0; i < 2; i++)
        {
            if (i == 0) {

                blit1->BindFrameBufferInit();
                glViewport(0, 0, blit1->width, blit1->height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit1->UseShader();
                blit1->shader.setFloat("texelsize_v", 1.0f / (float)height);
                blit1->DrawQuad(normalShader->texes_output[1].id, false);
                blit1->Blit(*temp1);
                blit1->BindFrameBufferOver();
            }
            else
            {
                blit1->BindFrameBufferInit();
                glViewport(0, 0, blit1->width, blit1->height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit1->UseShader();
                blit1->shader.setFloat("texelsize_v", 1.0f / (float)height);
                blit1->DrawQuad(temp2->texture.id, false);
                blit1->Blit(*temp1);
                blit1->BindFrameBufferOver();
            }

            blit2->BindFrameBufferInit();
            glViewport(0, 0, blit2->width, blit2->height);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDepthFunc(GL_LEQUAL);
            blit2->UseShader();
            blit2->shader.setFloat("texelsize_h", 1.0f / (float)width);
            blit2->DrawQuad(temp1->texture.id, false);
            blit2->Blit(*temp2);
            blit2->BindFrameBufferOver();

        }
        normalShader->texes_output[1].id = temp2->texture.id;
        for (int i = 0; i < 2; i++)
        {
            if (i == 0) {

                blit1->BindFrameBufferInit();
                glViewport(0, 0, blit1->width, blit1->height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit1->UseShader();
                blit1->shader.setFloat("texelsize_v", 1.0f / (float)height);
                blit1->DrawQuad(normalShader->texes_output[4].id, false);
                blit1->Blit(*temp1);
                blit1->BindFrameBufferOver();
            }
            else
            {
                blit1->BindFrameBufferInit();
                glViewport(0, 0, blit1->width, blit1->height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit1->UseShader();
                blit1->shader.setFloat("texelsize_v", 1.0f / (float)height);
                blit1->DrawQuad(temp3->texture.id, false);
                blit1->Blit(*temp1);
                blit1->BindFrameBufferOver();
            }

            blit2->BindFrameBufferInit();
            glViewport(0, 0, blit2->width, blit2->height);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDepthFunc(GL_LEQUAL);
            blit2->UseShader();
            blit2->shader.setFloat("texelsize_h", 1.0f / (float)width);
            blit2->DrawQuad(temp1->texture.id, false);
            blit2->Blit(*temp3);
            blit2->BindFrameBufferOver();

        }
        normalShader->texes_output[4].id = temp3->texture.id;
    }
    else
    {
        pool->ExPand();
        vector<float> all(Input_N * Input_N * 4, 0);
        vector<float> all3(wave_particle_resolution * wave_particle_resolution * 6, 0);
        vector<float> all1(Input_N * Input_N * 4, 0);
        vector<int> indexArray = pool->indexArray_out;//输入的indexArray
        for (int j = 0; j < indexArray.size(); j++)
        {
            int i = indexArray[j];
            glm::vec2 pos = pool->particles[i].pos;
            float a0 = pool->particles[i].amplitude;
            float r = pool->particles[i].radius;
            glm::vec2 dir = glm::normalize(pool->particles[i].wave_speed);
            int x = std::max(std::min((int)pos.x, wave_particle_resolution - 1), 0);
            int y = std::max(std::min((int)pos.y, wave_particle_resolution - 1), 0);
            all[j * 4 + 0] = pos.x;
            all[j * 4 + 1] = pos.y;
            all[j * 4 + 2] = a0;
            all[j * 4 + 3] = r;
            all1[j * 4 + 0] = dir.x;
            all1[j * 4 + 1] = dir.y;
            all1[j * 4 + 2] = 0;
            all1[j * 4 + 3] = 0;
            all3[j * 4 + 0] = (float)pos.x / (float)wave_particle_resolution_fliter * 2.0 - 1.0;
            all3[j * 4 + 1] = (float)pos.y / (float)wave_particle_resolution_fliter * 2.0 - 1.0;
            all3[j * 4 + 2] = a0;
            all3[j * 4 + 3] = r;
            all3[j * 4 + 4] = dir.x;
            all3[j * 4 + 5] = dir.y;
        }
        //cout << indexArray.size() << " ";

        WaveParticleShader->texes_input[0] = *particleImage;
        WaveParticleShader->texes_input[0].setData(all);
        WaveParticleShader->texes_input[1] = *particleImage1;
        WaveParticleShader->texes_input[1].setData(all1);
        if (enableWaveParticle_hvfliter)
            pool->UpdatePoints(all3);


        if (enableWaveParticle_hvfliter)
        {
            //将Wavemap拉伸到1024*1024
            copy->BindFrameBufferInit();
            glViewport(0, 0, copy->width, copy->height);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDepthFunc(GL_LEQUAL);
            //  blit->DrawQuad(WaveParticleShader->texes_output[0].id, true);
            DrawPoint->use();
            glPointSize(1);

            glBindVertexArray(pool->pointVAO);
            //绘制顶点 
            glDrawArrays(GL_POINTS, 0, particle_num);
            copy->Blit(*temp6);
            copy->BindFrameBufferOver();

            for (int i = 0; i < 5; i++)
            {


                {
                    blit1->BindFrameBufferInit();
                    glViewport(0, 0, blit1->width, blit1->height);
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                    glDepthFunc(GL_LEQUAL);
                    blit1->UseShader();
                    blit1->shader.setFloat("texelsize_v", 1.0f / (float)height);
                    blit1->DrawQuad(temp6->texture.id, false);
                    blit1->Blit(*temp5);
                    blit1->BindFrameBufferOver();
                }

                blit2->BindFrameBufferInit();
                glViewport(0, 0, blit2->width, blit2->height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit2->UseShader();
                blit2->shader.setFloat("texelsize_h", 1.0f / (float)width);
                blit2->DrawQuad(temp5->texture.id, false);
                blit2->Blit(*temp6);
                blit2->BindFrameBufferOver();

            }


            blit5->BindFrameBufferInit();
            glViewport(0, 0, blit5->width, blit5->height);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDepthFunc(GL_LEQUAL);
            blit5->UseShader();
            blit5->shader.setFloat("N",wave_particle_resolution_fliter);
            blit5->shader.setFloat("radius", radius);
            blit5->shader.setFloat("Scale", scale_Wave);
            blit5->DrawQuad(temp6->texture.id, false);
            blit5->Blit(*temp4);
            blit5->BindFrameBufferOver();
            blit6->BindFrameBufferInit();
            glViewport(0, 0, blit6->width, blit6->height);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDepthFunc(GL_LEQUAL);
            blit6->UseShader();
            glActiveTexture(GL_TEXTURE0 + 1);
            temp4->texture.nameInShader = "particle_map";
            blit6->shader.setInt(temp4->texture.nameInShader, 1);
            glBindTexture(GL_TEXTURE_2D, temp4->texture.id);
            blit6->shader.setFloat("N", wave_particle_resolution_fliter);
            blit6->shader.setFloat("radius", radius);
            blit6->shader.setFloat("Scale", scale_Wave);
            blit6->DrawQuad(temp6->texture.id, false);
            blit6->Blit(*temp5);
            blit6->BindFrameBufferOver();
            WaveParticleShader->texes_output[0].id = temp5->texture.id;
            // over
            // over


        }
        else
        {

            WaveParticleShader->use();
            WaveParticleShader->setInt("N", indexArray.size());
            WaveParticleShader->setInt("Input_N", Input_N);
            WaveParticleShader->setFloat("Scale", scale_Wave);
            WaveParticleShader->dispatch();
            WaveParticleShader->wait();
        }
        {
            //将Wavemap拉伸到1024*1024
            blit->BindFrameBufferInit();
            glViewport(0, 0, blit->width, blit->height);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDepthFunc(GL_LEQUAL);
            blit->DrawQuad(WaveParticleShader->texes_output[0].id, true);
            blit->Blit(*wavemap);
            blit->BindFrameBufferOver();


            blit4->BindFrameBufferInit();
            glViewport(0, 0, blit4->width, blit4->height);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDepthFunc(GL_LEQUAL);
            blit4->DrawQuad(waveImage1->id, true);
            blit4->Blit(*temp3);
            blit4->BindFrameBufferOver();
            blit4->Draw();


            blit3->BindFrameBufferInit();
            glViewport(0, 0, blit3->width, blit3->height);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDepthFunc(GL_LEQUAL);
            blit3->UseShader();
            glActiveTexture(GL_TEXTURE0 + 1);
            temp3->texture.nameInShader = "waveMap";
            blit3->shader.setInt(temp3->texture.nameInShader, 1);
            blit3->shader.setFloat("Time", Time);
            blit3->shader.setVec4("Scale_control", scale_Control_WaveParicle);
            glBindTexture(GL_TEXTURE_2D, temp3->texture.id);
            blit3->DrawQuad(wavemap->texture.id, false);
            blit3->Blit(*temp2);
            blit3->BindFrameBufferOver();

            for (int i = 0; i < 9; i++)
            {


                {
                    blit1->BindFrameBufferInit();
                    glViewport(0, 0, blit1->width, blit1->height);
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                    glDepthFunc(GL_LEQUAL);
                    blit1->UseShader();
                    blit1->shader.setFloat("texelsize_v", 2.0f / (float)height);
                    blit1->DrawQuad(temp2->texture.id, false);
                    blit1->Blit(*temp1);
                    blit1->BindFrameBufferOver();





                }

                blit2->BindFrameBufferInit();
                glViewport(0, 0, blit2->width, blit2->height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit2->UseShader();
                blit2->shader.setFloat("texelsize_h", 2.0f / (float)width);
                blit2->DrawQuad(temp1->texture.id, false);
                blit2->Blit(*temp2);
                blit2->BindFrameBufferOver();

            }

            WaveParticleShader->texes_output[0].id = temp2->texture.id;


        }





        normalShader1->texes_input[0] = WaveParticleShader->texes_output[0];
        normalShader1->use();
        normalShader1->setFloat("BubblesScale", BubblesScale);
        normalShader1->setFloat("BubblesThreshold", BubblesThreshold);
        normalShader1->setFloat("jacobScaleEdge", jacobScaleEdge);
        normalShader1->setFloat("jacobScale", jacobScale);
        normalShader1->setFloat("resolutioninv", 2.0f * scale_Ocean / (float)width);
        normalShader1->dispatch();
        normalShader1->wait();


    }




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
    depthShader->use();
    depthShader->setMatrix("lightProjection", lightProjection);
    depthShader->setMatrix("lightView", lightView);
    depthShader->setMatrix("model", model);
    ourModel->Draw(*depthShader);
    depthOceanShader->use();
    depthOceanShader->setMatrix("lightProjection", lightProjection);
    depthOceanShader->setMatrix("lightView", lightView);
    depthOceanShader->setMatrix("model", model);
    depthOceanShader->setMatrix("model", floormodel);
    if (enableWaveParticle) {
        //  WaveParticleShader->texes_output[0].id = waveImage1.id;
        WaveParticleShader->texes_output[0].nameInShader = "_displacementMap";
        rt[1] = WaveParticleShader->texes_output[0];

    }
    else {
        DisplacementShader->texes_output[3].nameInShader = "_displacementMap";
        rt[1] = DisplacementShader->texes_output[3];

    }
    glActiveTexture(GL_TEXTURE0);
    depthOceanShader->setInt("_displacementMap", 0);
    glBindTexture(GL_TEXTURE_2D, rt[1].id);
    roomModel->Draw(*depthOceanShader);
    depthMap->BindFrameBufeerOver();
    ubotest->ClearSizeNow();

    rt[0] = depthMap->texture;
    {

        //将Wavemap拉伸到1024*1024
        copy->BindFrameBufferInit();
        glViewport(0, 0, copy->width, copy->height);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDepthFunc(GL_LEQUAL);
        glDepthFunc(GL_LEQUAL);
        ubotest->SetMatrix(projection);
        ubotest->SetMatrix(view * camera.CaculateReflectMatrix(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0)));//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
        skyBoxShader->use();

        skyBoxShader->setMatrix("model", modelskyBox);
        skyBoxShader->setMatrix("projection", projection);
        glm::mat4 viewskyBox = glm::mat4(glm::mat3(view)) * camera.CaculateReflectMatrix(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
        skyBoxShader->setMatrix("view", viewskyBox);
        cubemap->Draw(*skyBoxShader);
        glDepthFunc(GL_LESS);
        myshader->use();
        myshader->setMatrix("model", model);
        myshader->setVec3("baseColor", glm::vec3(0.5, 0.5, 0.5));
        myshader->setVec3("cameraPos", camera.GetPos());
        myshader->setMatrix("lightMat", (lightProjection * lightView));
        for (unsigned int i = 0; i < 4; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            lights[i].SetLight(*myshader);
        }
        ourModel->Draw(*myshader, rt);
        copy->Blit(*temp6);
        copy->BindFrameBufferOver();
        ubotest->ClearSizeNow();
    }//draw reflect

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
    for (unsigned int i = 0; i < 4; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        lights[i].SetLight(*myshader);
    }
    /* glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
     glStencilMask(0xFF);

     glStencilFunc(GL_ALWAYS, 2, 0xFF);
     ourModel->Draw(myshader);
     glStencilMask(0x00);
     myshader->setMatrix("model", model);
     glStencilFunc(GL_EQUAL, 2, 0xFF);
     model = glm::translate(model,glm::vec3(0,0,3));

     lightshader->setMatrix("model", model);
     ourModel->Draw(lightshader);
     glStencilMask(0xFF);*/
   // ourModel->Draw(*myshader, rt);

       // glm::mat4 model1 = glm::mat4(1.0f);
       // model1 = glm::translate(model1, glm::vec3(0, 0, 1));
       // model1 = glm::scale(model1, glm::vec3(0.2f));

       // InstanceShader.use();
       // InstanceShader.setMatrix("model", model1);
       // InstanceShader.setMatrix("view", camera.GetView());
       // InstanceShader.setVec3("baseColor", glm::vec3(0.5, 0.5, 0.5));
       // InstanceShader.setVec3("cameraPos", camera.GetPos());
       // InstanceShader.setMatrix("projection", projection);
       // for (unsigned int i = 0; i < 4; i++)
       // {
       //     // calculate the model matrix for each object and pass it to shader before drawing
       //     lights[i].SetLight(InstanceShader);
       // }
       // for(int i=0;i<1;i++)
       //ourModel1.Draw(InstanceShader);
    FloorShader->use();
    for (unsigned int i = 0; i < 4; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        lights[i].SetLight(*FloorShader);
    }
    FloorShader->setMatrix("model", floormodel);
    FloorShader->setMatrix("model_inverse_t", floormodel_inverse_t);
    FloorShader->setFloat("Time", 2 * Time);
    FloorShader->setVec3("cameraPos", camera.GetPos());
    FloorShader->setMatrix("lightMat", (lightProjection * lightView));
    FloorShader->setFloat("Scale_subsurface", scale_subsurface);
    FloorShader->setFloat("_Glossiness", glossiness);
    FloorShader->setFloat("_Metallic", metallic);
    FloorShader->setFloat("_bubbleMapPower", scale_Control_WaveParicle.w);
    if (enableWaveParticle) {
        //  WaveParticleShader->texes_output[0].id = waveImage1.id;
        WaveParticleShader->texes_output[0].nameInShader = "_displacementMap";
        rt[1] = WaveParticleShader->texes_output[0];
        normalShader1->texes_output[0].nameInShader = "_normalMap";
        rt[2] = (normalShader1->texes_output[0]);
        normalShader1->texes_output[1].nameInShader = "_bubbleMap";
        rt[3] = (normalShader1->texes_output[1]);
        waveImage1->nameInShader = "_waveMap";
        rt[4] = *(waveImage1);
        normalShader1->texes_output[2].nameInShader = "_tangentMap";
        rt[5] = normalShader1->texes_output[2];
        normalShader1->texes_output[3].nameInShader = "_biotangentMap";
        rt[6] = normalShader1->texes_output[3];
        bubbleDiffuse->nameInShader = "_bubbleDiffuseMap";
        rt[7] = *bubbleDiffuse;
        normalShader1->texes_output[4].nameInShader = "_bubbleDiffuseMask";
        rt[8] = normalShader1->texes_output[4];
    }
    else 
    {
        DisplacementShader->texes_output[3].nameInShader = "_displacementMap";
        rt[1] = DisplacementShader->texes_output[3];
        normalShader->texes_output[0].nameInShader = "_normalMap";
        rt[2] = (normalShader->texes_output[0]);
        normalShader->texes_output[1].nameInShader = "_bubbleMap";
        rt[3] = (normalShader->texes_output[1]);
        temp6->texture.nameInShader = "_reflectionMap";
        rt[4] = (temp6->texture);
        normalShader->texes_output[2].nameInShader = "_tangentMap";
        rt[5] = normalShader->texes_output[2];
        normalShader->texes_output[3].nameInShader = "_biotangentMap";
        rt[6] = normalShader->texes_output[3];
        bubbleDiffuse->nameInShader = "_bubbleDiffuseMap";
        rt[7] = *bubbleDiffuse;
        normalShader->texes_output[4].nameInShader = "_bubbleDiffuseMask";
        rt[8] = normalShader->texes_output[4];
        

    }




    roomModel->Draw(*FloorShader, rt, cubemap);




    posteffect->BindFrameBufferOver();

    posteffect->Draw();
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
        ImGui::SliderFloat("A", &A, 0.1f, 100.0f);
        ImGui::SliderFloat("BubblesThreshold", &BubblesThreshold, 0.1f, 2.0f);
        ImGui::SliderFloat("BubblesScale", &BubblesScale, 0.0f, 100.0f);
        ImGui::SliderFloat("jacobScale", &jacobScale, 10.0f, 1000.0f);
        ImGui::SliderFloat("jacobScaleEdge", &jacobScaleEdge, 1.0f, 3.0f);
        ImGui::SliderFloat4("WindSeed", (float*)&(windspeed), 0.0f, 50.0f);
        ImGui::SliderFloat("WaveScale", (float*)&(scale_Wave), 0.0f, 2.0f);
        ImGui::SliderFloat4("Scale_control_WaveParticle", (float*)&(scale_Control_WaveParicle), 0.0f, 2.0f);
        ImGui::SliderFloat("Scale_subsurface", (float*)&(scale_subsurface), 0.0f, 1.0f);
        ImGui::SliderFloat("Glossness", (float*)&(glossiness), 0.0f, 1.0f);
        ImGui::SliderFloat("Metallic", (float*)&(metallic), 0.0f, 1.0f);
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

WaveProcess::~WaveProcess()
{
    std::cout << "delete Over";
    delete myshader;
    delete lightshader;
    delete screenShader;
    delete InstanceShader;
    delete FloorShader;
    delete ourModel;
    delete roomModel;
    delete depthShader;
    delete depthOceanShader;
    delete  skyBoxShader;
    delete  GaussianShader;
    delete  HeightSpectrumShader;
    delete  GaussianBlurShader_h;
    delete  GaussianBlurShader_v;
    delete  AddDisplacementShader;
    delete  WaveParticleShader_h;
    delete  WaveParticleShader_v;
    delete  DrawPoint;
    delete OffsetSpectrumShader;
    delete FftXShader;
    delete FftYShader;
    delete FftXendShader;
    delete FftYendShader;
    delete DisplacementShader;
    delete normalShader;
    delete WaveParticleShader;
    delete GaussianBlur_h;
    delete GaussianBlur_v;
    delete GaussianBlur_h1;
    delete GaussianBlur_v1;
    delete normalShader1;
    delete blit6;
    delete blit5;
    delete blit4;
    delete blit3;
    delete blit2;
    delete blit1;
    delete blit;
    delete copy;
    delete copyWaveParticle;
    delete wavemap;
    delete temp1;
    delete temp2;
    delete temp3;
    delete temp4;
    delete temp5;
    delete temp6;
    delete temp7;
    delete temp8;
    delete posteffect;
    delete gammaCorrectionShader;

    delete particleImage;
    delete particleImage1;
    delete waveImage1;
    delete light;
    delete pointLight;
    delete pointLight1;
    delete spotLight;
    delete cubemap;
    delete ubotest;
    delete depthMap;
    delete pool;
    delete bubbleDiffuse;
   // delete bubbleNormal;
    
}
