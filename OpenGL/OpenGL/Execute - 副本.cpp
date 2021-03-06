#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
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
#include"WaveProcess.h"

static bool showwindow = true;
glm::vec3 pos = glm::vec3(0.0f, 15.0f, -3.0f);
glm::vec3 frontDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 upDir = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera = Camera(pos, frontDir, upDir);
glm::mat4 view;
glm::vec4 windspeed = glm::vec4(11.0f, 23.0f, 28.0f, 0.0f);
glm::vec3 limit = glm::vec3(0.2f, 0.4f, 1.0f);
float cameraSpeed = 2.0f;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
int width = 1024;
int height = 1024;
int CSwidth = 256;
int CSheight = 256;
float Time = 0.0f;
double lastX = width / 2;
double lastY = height / 2;
float yaw = -90.0f;
float pitch = 0;
float A = 32.0f;
bool firstEnter = true;
bool init = false;
float BubblesThreshold = 0.7f;
float BubblesScale = 1.0f;
float jacobScale = 100.0f;
float Input_N = 64;
float B_Spine_count = 1024;
float scale_Wave = 0.2f;
float scale_Ocean = 10.0f;
float scale_subsurface = 1.0f;
glm::vec4 scale_Control_WaveParicle = glm::vec4(0.3f, 1.0f, 0.8, 0.0f);
//glm::vec4 scale_Control_WaveParicle = glm::vec4(0.0f, 1.0f, 0.0, 0.0f);
int wave_particle_resolution = 64;
int wave_particle_resolution_fliter = 2048;
float radius = 100;
bool enableWaveParticle = true;
bool enableWaveParticle_hvfliter = false;
int select = 0;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if (state == GLFW_PRESS) {

        if (firstEnter) {
            lastX = xpos;
            lastY = ypos;
            firstEnter = false;

        }
        float sensitivity = 0.06f;
        double xOffset = (xpos - lastX) * sensitivity;
        double yOffset = -(ypos - lastY) * sensitivity;
        lastX = xpos;
        lastY = ypos;
        yaw += xOffset;
        pitch += yOffset;
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        camera.SetFrontDir(glm::normalize(front));
    }
    if (state == GLFW_RELEASE)
        firstEnter = true;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

    float fov = camera.GetFov();
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
    camera.SetFov(fov);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    frontDir = camera.GetFrontDir();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

        camera.Move(LEFT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        camera.Move(RIGHT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {


        camera.Move(FORWARD, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

        camera.Move(BACK, deltaTime);

    }

}

void CalculateDeltaTime() {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

}
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}



int main(int, char**)
{

    //float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    //   // positions   // texCoords
    //   -1.0f,  1.0f,  0.0f, 1.0f,
    //   -1.0f, -1.0f,  0.0f, 0.0f,
    //    1.0f, -1.0f,  1.0f, 0.0f,

    //   -1.0f,  1.0f,  0.0f, 1.0f,
    //    1.0f, -1.0f,  1.0f, 0.0f,
    //    1.0f,  1.0f,  1.0f, 1.0f
    //};

    glfwInit();
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
        // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //unsigned int amount = 1000;
    //vector<glm::mat4> modelMatrices;
    //modelMatrices = vector<glm::mat4>();
    //srand(glfwGetTime()); // initialize random seed 
    //float radius = 50.0;
    //float offset = 2.5f;
    //for (unsigned int i = 0; i < amount; i++)
    //{
    //    glm::mat4 model = glm::mat4(1.0f);
    //    // 1. translation: displace along circle with 'radius' in range [-offset, offset]
    //    float angle = (float)i / (float)amount * 360.0f;
    //    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    //    float x = sin(angle) * radius + displacement;
    //    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    //    float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
    //    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    //    float z = cos(angle) * radius + displacement;
    //    model = glm::translate(model, glm::vec3(x, y, z));

    //    // 2. scale: scale between 0.05 and 0.25f
    //    float scale = (rand() % 20) / 100.0f + 0.05;
    //    model = glm::scale(model, glm::vec3(scale));

    //    // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
    //    float rotAngle = (rand() % 360);
    //    model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    //    // 4. now add to list of matrices
    //    modelMatrices.push_back( model);
    //}

    Shader myshader = Shader(".//Shader//MyVertexShader.vs", ".//Shader//LightingCubeShader.fs");
    // Shader lightshader = Shader(".//Shader//MyVertexShader.vs", ".//Shader//LightShader.fs", ".//Shader//LightShader.gs");
    Shader lightshader = Shader(".//Shader//LightShader.vs", ".//Shader//LightShader.fs");
    Shader screenShader = Shader(".//Shader//PostEffect.vs", ".//Shader//PostEffect.fs");
    Shader InstanceShader = Shader(".//Shader//InstanceShader.vs", ".//Shader//LightingCubeShader.fs");
    Shader FloorShader = Shader(".//Shader//floorShader.vs", ".//Shader//floorShader.fs");
    //  Shader OceanShader = Shader(".//Shader//oceanShader.vs", ".//Shader//oceanShader.fs");
    Model ourModel(".//Model//nanosuit//nanosuit.obj");
    Model roomModel(".//Model//floor//floor3.obj");

    //Model sponzaModel(".//Model//sponza//sponza.obj");
    Shader depthShader = Shader(".//Shader//DepthShader.vs", ".//Shader//DepthShader.fs");
    Shader depthOceanShader = Shader(".//Shader//DepthOceanShader.vs", ".//Shader//DepthShader.fs");
    // Model ourModel(".//Model//planet//planet.obj");
    //  Model ourModel1(".//Model//rock//rock.obj",true,modelMatrices,amount);
     //Model ourModel1(".//Model//rock//rock.obj");
    Shader skyBoxShader = Shader(".//Shader//CubeMap.vs", ".//Shader//CubeMap.fs");
    ComputeShader GaussianShader = ComputeShader(".//Shader//Test.comp", glm::vec2(CSwidth, CSheight));
    ComputeShader HeightSpectrumShader = ComputeShader(".//Shader//Height.comp", glm::vec2(CSwidth, CSheight));
    Shader GaussianBlurShader_h = Shader(".//Shader//PostEffect_gaussian_h.vs", ".//Shader//GaussionBlur_h.fs");
    Shader GaussianBlurShader_v = Shader(".//Shader//PostEffect_gaussian_v.vs", ".//Shader//GaussionBlur_h.fs");
    Shader AddDisplacementShader = Shader(".//Shader//PostEffect.vs", ".//Shader//AddDisplacement.fs");
    Shader WaveParticleShader_h = Shader(".//Shader//WaveParticle_h.vs", ".//Shader//WaveParticle_h.fs");
    Shader WaveParticleShader_v = Shader(".//Shader//WaveParticle_h.vs", ".//Shader//WaveParticle_v.fs");
    Shader DrawPoint = Shader(".//Shader//DrawPoint.vs", ".//Shader//DrawPoint.fs");
    //  Shader GroundShader = Shader(".//Shader//ground.vs", ".//Shader//ground.fs");
    ComputeShader OffsetSpectrumShader = ComputeShader(".//Shader//Offset.comp", glm::vec2(CSwidth, CSheight), 2);
    ComputeShader FftXShader = ComputeShader(".//Shader//Fftx.comp", glm::vec2(CSwidth, CSheight), 3, 3);
    ComputeShader FftYShader = ComputeShader(".//Shader//Ffty.comp", glm::vec2(CSwidth, CSheight), 3, 3);
    ComputeShader FftXendShader = ComputeShader(".//Shader//Fftxend.comp", glm::vec2(CSwidth, CSheight), 3, 3);
    ComputeShader FftYendShader = ComputeShader(".//Shader//Fftyend.comp", glm::vec2(CSwidth, CSheight), 3, 3);
    ComputeShader DisplacementShader = ComputeShader(".//Shader//Displacement.comp", glm::vec2(CSwidth, CSheight), 4, 3);
    ComputeShader normalShader = ComputeShader(".//Shader//normalCalc.comp", glm::vec2(CSwidth, CSheight), 2, 1);
    ComputeShader WaveParticleShader = ComputeShader(".//Shader//WaveParticle.comp", glm::vec2(wave_particle_resolution, wave_particle_resolution), 1, 2, 0, false);
    ComputeShader GaussianBlur_h = ComputeShader(".//Shader//GaussianBlur_h.comp", glm::vec2(wave_particle_resolution, wave_particle_resolution), 1, 1);
    ComputeShader GaussianBlur_v = ComputeShader(".//Shader//GaussianBlur_v.comp", glm::vec2(wave_particle_resolution, wave_particle_resolution), 1, 1);
    ComputeShader GaussianBlur_h1 = ComputeShader(".//Shader//GaussianBlur_h.comp", glm::vec2(width, height), 1, 1);
    ComputeShader GaussianBlur_v1 = ComputeShader(".//Shader//GaussianBlur_v.comp", glm::vec2(width, height), 1, 1);
    ComputeShader normalShader1 = ComputeShader(".//Shader//normalCalc.comp", glm::vec2(width, height), 2, 1);
    PostEffect blit6 = PostEffect(WaveParticleShader_v, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    PostEffect blit5 = PostEffect(WaveParticleShader_h, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    PostEffect blit4 = PostEffect(screenShader, B_Spine_count, B_Spine_count, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    PostEffect blit3 = PostEffect(AddDisplacementShader, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    PostEffect blit2 = PostEffect(GaussianBlurShader_h, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    PostEffect blit1 = PostEffect(GaussianBlurShader_v, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    PostEffect blit = PostEffect(screenShader, wave_particle_resolution, wave_particle_resolution, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    PostEffect copy = PostEffect(screenShader, width, height, false, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    FBO wavemap = FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    FBO temp1 = FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    FBO temp2 = FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    FBO temp3 = FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    FBO temp4 = FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    FBO temp5 = FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    FBO temp6 = FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    FBO temp7 = FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    FBO temp8 = FBO(width, height, "_wavemap", FBO::Color, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    PostEffect posteffect = PostEffect(screenShader, width, height, true);
    PostEffect reflectBuffer = PostEffect(screenShader, width, height, true);
    // vector<glm::vec2> control = vector<glm::vec2>({ glm::vec2(0,0),glm::vec2(11,0),glm::vec2(17,8),glm::vec2(24,10),glm::vec2(26,19),glm::vec2(26,0),glm::vec2(26,0),glm::vec2(26,0),glm::vec2(40,0),glm::vec2(45,0) });
    vector<glm::vec2> control = vector<glm::vec2>({ glm::vec2(0,0),glm::vec2(11,0),glm::vec2(17,8),glm::vec2(24,18),glm::vec2(26,19),glm::vec2(26.2,0),glm::vec2(30,0) });
    //  vector<glm::vec2> control = vector<glm::vec2>({ glm::vec2(0,5),glm::vec2(11,7),glm::vec2(17,8),glm::vec2(24,18),glm::vec2(26,19),glm::vec2(26.2,0),glm::vec2(30,0) });
    B_Spine b_spine = B_Spine(B_Spine_count, 4, control, vector<double>(0));
    b_spine.generate_u();
    b_spine.produce_height_array();
    unsigned int texture;
    unsigned int texture1;

    TextureImage particleImage = TextureImage(Input_N, Input_N);
    particleImage.Bind();
    TextureImage particleImage1 = TextureImage(Input_N, Input_N);
    particleImage1.Bind();
    TextureImage waveImage1 = TextureImage(B_Spine_count, 1);
    waveImage1.Bind();
    waveImage1.setData(b_spine.y_array);
    auto a = waveImage1.getData();
    myshader.use();


    glm::vec4 vec(0.0f, 0.0f, 0.0f, 0.0f);



    glm::vec3 lightPos(0.0f, 10.0f, 4.0f);
    glm::vec3 lightPos1(0.0f, 3.0f, 2.0f);
    glm::vec3 lightDir = lightPos;
    glm::vec4 lightColor(1, 1, 1, 0);

    Light light(DIRLIGHT, glm::vec3(0.0, 15.0, 1.0), -glm::vec3(0.0, 0.4, 0.5), lightColor);
    Light pointLight(POINTLIGHT, lightPos, -lightDir, lightColor);
    Light pointLight1(POINTLIGHT, lightPos1, -lightDir, lightColor);
    pointLight.lightProperty = glm::vec3(1.0f, 0.9f, 0.032f);
    pointLight1.lightProperty = glm::vec3(1.0f, 0.9f, 0.032f);
    Light spotLight(SPOTLIGHT, glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 0.0, -1.0), lightColor);
    spotLight.cutOffAngle = glm::cos(glm::radians(12.5));
    Light lights[4] = { pointLight,pointLight1,spotLight,light };
    /*   CubeMap cubemap = CubeMap(vector<string>{".//Model//skybox//right.jpg",
           ".//Model//skybox//left.jpg",
           ".//Model//skybox//top.jpg",
           ".//Model//skybox//bottom.jpg",
           ".//Model//skybox//front.jpg",
           ".//Model//skybox//back.jpg"});*/
    CubeMap cubemap = CubeMap(vector<string>{".//Model//skybox//FS000_Day_04.png_Rig.jpg",
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


    unsigned int pointVAO, pointVBO;




    posteffect.BindQuad();
    blit.BindQuad();
    blit1.BindQuad();
    blit2.BindQuad();
    blit3.BindQuad();
    blit4.BindQuad();
    blit5.BindQuad();
    blit6.BindQuad();
    copy.BindQuad();
    reflectBuffer.BindQuad();
    cubemap.Bind();



    // framebuffer configuration
    posteffect.SetFrameBuffer();
    blit.SetFrameBuffer();
    blit1.SetFrameBuffer();
    blit2.SetFrameBuffer();
    blit3.SetFrameBuffer();
    blit4.SetFrameBuffer();
    blit5.SetFrameBuffer();
    blit6.SetFrameBuffer();
    copy.SetFrameBuffer();
    reflectBuffer.SetFrameBuffer();
    UBO ubotest = UBO(2 * sizeof(glm::mat4));

    vector<Shader> shaders{ lightshader, myshader,FloorShader };
    ubotest.SetUnifromBlockIndexAndBind(shaders, "Matrices");
    ubotest.BindUniformBuffer();
    int shadowwidth = 2 * width;
    int shadowheight = 2 * height;
    FBO depthMap = FBO(shadowwidth, shadowheight, "_depthTexture", FBO::Depth);
    Wave_Particle_Pool pool = Wave_Particle_Pool(Input_N * Input_N, wave_particle_resolution);
    if (enableWaveParticle_hvfliter)
        pool = Wave_Particle_Pool(Input_N * Input_N, wave_particle_resolution_fliter);
    pointVAO = pool.pointVAO;
    pointVBO = pool.pointVBO;
    srand((int)time(0));
    int particle_num = 700;
    if (!enableWaveParticle_hvfliter)
    {

        for (int i = 0; i < particle_num; i++)
        {
            int randomNum = rand() % wave_particle_resolution;;
            int randomNum3 = rand() % wave_particle_resolution;;
            float randomNum1 = rand() % 64 / 64.0f + 0.01f;;
            float randomNum2 = rand() % 64 / 64.0f + 0.01f;;
            float height = 0.03f;
            int a = rand() % 3; int b = rand() % 3;
            if (a > 1)
                a = 1;
            else
                a = -1;
            if (b > 1)
                b = 1;
            else
                b = -1;
            pool.PushToParticles(glm::vec2(randomNum, randomNum3), glm::vec2(randomNum, randomNum3), glm::vec2(a * randomNum1, b * randomNum2), height, 5.0f, 60, 0.0);

        }
        //pool.PushToParticles(glm::vec2(32, 32), glm::vec2(32, 32), glm::vec2(0.1, 0.1), 0.5f, 2.0f, 60, 0.0);
    }
    if (enableWaveParticle_hvfliter)
    {
        //pool.PushToParticles(glm::vec2(wave_particle_resolution_fliter / 2.0, wave_particle_resolution_fliter / 2.0), glm::vec2(wave_particle_resolution_fliter / 2.0, wave_particle_resolution_fliter / 2.0), glm::vec2(0.005 * wave_particle_resolution_fliter, 0.005 * wave_particle_resolution_fliter), 3.0f, 10.0f, 60, 0.0);

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
            pool.PushToParticles(glm::vec2(randomNum, randomNum3), glm::vec2(randomNum, randomNum3), 100.0f * glm::vec2(a * randomNum1, b * randomNum2), height, 5.0f, 60, 0.0);

        }
    }
    /* pool.PushToParticles(glm::vec2(64, 64), glm::vec2(64, 64), glm::vec2(0.1, 0.1), 0.3f, 5.0f, 60, 0.0);*/
     //pool.particles.push_back(Wave_Particle( glm::vec2(16, 16), glm::vec2(16, 16), glm::vec2(0.1, 0.1), 1.0, 2.5, 30, 0.0));
    std::vector<Texture> rt = std::vector<Texture>(7);

    while (!glfwWindowShouldClose(window))
    {


        //  waveImage1.setData(b_spine.y_array);
        ubotest.ClearSizeNow();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //glEnable(GL_MULTISAMPLE);

        CalculateDeltaTime();
        //输入
        processInput(window);
        //渲染指令
        //posteffect.BindFrameBuffer();


        /*   glEnable(GL_BLEND);
           glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
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


        for (int i = 0; i < FftXShader.texes_output.size(); i++) {
            FftXShader.texes_output[i].InitId();
            FftYShader.texes_output[i].InitId();
        }
        for (int i = 0; i < GaussianBlur_h1.texes_input.size(); i++) {
            GaussianBlur_h1.texes_input[0].InitId();
            GaussianBlur_h.texes_input[0].InitId();
            GaussianBlur_v1.texes_input[0].InitId();
            GaussianBlur_v.texes_input[0].InitId();
            GaussianBlur_h1.texes_output[0].InitId();
            GaussianBlur_h.texes_output[0].InitId();
            GaussianBlur_v1.texes_output[0].InitId();
            GaussianBlur_v.texes_output[0].InitId();

        }
        for (int i = 0; i < normalShader.texes_output.size(); i++) {
            normalShader.texes_output[i].InitId();

        }
        WaveParticleShader.texes_output[0].InitId();
        DisplacementShader.texes_output[3].InitId();

        Time += deltaTime;
        if (!enableWaveParticle)
        {

            GaussianShader.use();
            GaussianShader.setInt("N", CSwidth);
            GaussianShader.dispatch();
            GaussianShader.wait();


            HeightSpectrumShader.texes_input[0] = GaussianShader.texes_output[0];

            // HeightSpectrumShader.setFloat("Time", Time);

            HeightSpectrumShader.use();
            HeightSpectrumShader.setInt("N", CSwidth);
            HeightSpectrumShader.setFloat("Time", 2 * Time);
            HeightSpectrumShader.setFloat("A", A);
            HeightSpectrumShader.setVec4("WindAndSeed", windspeed);
            HeightSpectrumShader.dispatch();
            HeightSpectrumShader.wait();

            OffsetSpectrumShader.texes_input[0] = HeightSpectrumShader.texes_output[0];
            OffsetSpectrumShader.use();
            OffsetSpectrumShader.setInt("N", CSwidth);
            OffsetSpectrumShader.dispatch();
            OffsetSpectrumShader.wait();

            FftXShader.texes_input[0] = OffsetSpectrumShader.texes_output[0];
            FftXShader.texes_input[1] = OffsetSpectrumShader.texes_output[1];
            FftXShader.texes_input[2] = OffsetSpectrumShader.texes_input[0];


            for (int i = 0; i < log2(CSwidth); i++)
            {
                int a = log2(CSwidth);
                int Ns = pow(2, i);
                if (i == log2(CSwidth) - 1)
                {
                    FftXendShader.texes_input[0] = FftXShader.texes_input[0];
                    FftXendShader.texes_input[1] = FftXShader.texes_input[1];
                    FftXendShader.texes_input[2] = FftXShader.texes_input[2];
                    FftXendShader.use();
                    FftXendShader.setInt("N", CSwidth);
                    FftXendShader.setInt("Ns", Ns);
                    FftXendShader.dispatch();
                    FftXendShader.wait();

                }
                else
                {
                    FftXShader.use();
                    FftXShader.setInt("N", CSwidth);
                    FftXShader.setInt("Ns", Ns);
                    FftXShader.dispatch();
                    FftXShader.wait();
                    FftXShader.texes_input[0].Swap(FftXShader.texes_output[0]);
                    FftXShader.texes_input[1].Swap(FftXShader.texes_output[1]);
                    FftXShader.texes_input[2].Swap(FftXShader.texes_output[2]);
                }

            }
            FftYShader.texes_input[0] = FftXendShader.texes_output[0];
            FftYShader.texes_input[1] = FftXendShader.texes_output[1];
            FftYShader.texes_input[2] = FftXendShader.texes_output[2];
            for (int i = 0; i < log2(CSwidth); i++)
            {
                int a = log2(CSwidth);
                int Ns = pow(2, i);
                if (i == log2(CSwidth) - 1)
                {
                    FftYendShader.texes_input[0] = FftYShader.texes_input[0];
                    FftYendShader.texes_input[1] = FftYShader.texes_input[1];
                    FftYendShader.texes_input[2] = FftYShader.texes_input[2];
                    FftYendShader.use();
                    FftYendShader.setInt("N", CSwidth);
                    FftYendShader.setInt("Ns", Ns);
                    FftYendShader.dispatch();
                    FftYendShader.wait();

                }
                else
                {
                    FftYShader.use();
                    FftYShader.setInt("N", CSwidth);
                    FftYShader.setInt("Ns", Ns);
                    FftYShader.dispatch();
                    FftYShader.wait();
                    FftYShader.texes_input[0].Swap(FftYShader.texes_output[0]);
                    FftYShader.texes_input[1].Swap(FftYShader.texes_output[1]);
                    FftYShader.texes_input[2].Swap(FftYShader.texes_output[2]);
                }

            }
            DisplacementShader.texes_input[0] = FftYendShader.texes_output[0];
            DisplacementShader.texes_input[1] = FftYendShader.texes_output[1];
            DisplacementShader.texes_input[2] = FftYendShader.texes_output[2];
            DisplacementShader.use();
            DisplacementShader.setVec3("limit", limit);
            DisplacementShader.setInt("N", CSwidth);
            DisplacementShader.dispatch();
            DisplacementShader.wait();


            normalShader.texes_input[0] = DisplacementShader.texes_output[3];

            normalShader.use();
            normalShader.setFloat("BubblesScale", BubblesScale);
            normalShader.setFloat("BubblesThreshold", BubblesThreshold);
            normalShader.setFloat("jacobScale", jacobScale);
            normalShader.setFloat("resolutioninv", 2.0f * scale_Ocean / (float)CSwidth);//平面的长度为2
            normalShader.dispatch();
            normalShader.wait();

            for (int i = 0; i < 2; i++)
            {
                if (i == 0) {

                    blit1.BindFrameBufferInit();
                    glViewport(0, 0, blit1.width, blit1.height);
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                    glDepthFunc(GL_LEQUAL);
                    blit1.UseShader();
                    blit1.shader.setFloat("texelsize_v", 1.0f / (float)height);
                    blit1.DrawQuad(normalShader.texes_output[1].id, false);
                    blit1.Blit(temp1);
                    blit1.BindFrameBufferOver();
                }
                else
                {
                    blit1.BindFrameBufferInit();
                    glViewport(0, 0, blit1.width, blit1.height);
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                    glDepthFunc(GL_LEQUAL);
                    blit1.UseShader();
                    blit1.shader.setFloat("texelsize_v", 1.0f / (float)height);
                    blit1.DrawQuad(temp2.texture.id, false);
                    blit1.Blit(temp1);
                    blit1.BindFrameBufferOver();
                }

                blit2.BindFrameBufferInit();
                glViewport(0, 0, blit2.width, blit2.height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit2.UseShader();
                blit2.shader.setFloat("texelsize_h", 1.0f / (float)width);
                blit2.DrawQuad(temp1.texture.id, false);
                blit2.Blit(temp2);
                blit2.BindFrameBufferOver();

            }
            normalShader.texes_output[1].id = temp2.texture.id;

        }
        else
        {
            pool.ExPand();
            vector<float> all(Input_N * Input_N * 4, 0);
            vector<float> all3(wave_particle_resolution * wave_particle_resolution * 6, 0);
            vector<float> all1(Input_N * Input_N * 4, 0);
            vector<int> indexArray = pool.indexArray_out;//输入的indexArray
            for (int j = 0; j < indexArray.size(); j++)
            {
                int i = indexArray[j];
                glm::vec2 pos = pool.particles[i].pos;
                float a0 = pool.particles[i].amplitude;
                float r = pool.particles[i].radius;
                glm::vec2 dir = glm::normalize(pool.particles[i].wave_speed);
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

            WaveParticleShader.texes_input[0] = particleImage;
            WaveParticleShader.texes_input[0].setData(all);
            WaveParticleShader.texes_input[1] = particleImage1;
            WaveParticleShader.texes_input[1].setData(all1);
            if (enableWaveParticle_hvfliter)
                pool.UpdatePoints(all3);


            if (enableWaveParticle_hvfliter)
            {
                //将Wavemap拉伸到1024*1024
                copy.BindFrameBufferInit();
                glViewport(0, 0, copy.width, copy.height);
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                //  blit.DrawQuad(WaveParticleShader.texes_output[0].id, true);
                DrawPoint.use();
                glPointSize(1);
                glBindVertexArray(pointVAO);
                //绘制顶点 
                glDrawArrays(GL_POINTS, 0, particle_num);
                copy.Blit(temp6);
                copy.BindFrameBufferOver();

                for (int i = 0; i < 2; i++)
                {


                    {
                        blit1.BindFrameBufferInit();
                        glViewport(0, 0, blit1.width, blit1.height);
                        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                        glClear(GL_COLOR_BUFFER_BIT);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                        glDepthFunc(GL_LEQUAL);
                        blit1.UseShader();
                        blit1.shader.setFloat("texelsize_v", 1.0f / (float)height);
                        blit1.DrawQuad(temp6.texture.id, false);
                        blit1.Blit(temp5);
                        blit1.BindFrameBufferOver();
                    }

                    blit2.BindFrameBufferInit();
                    glViewport(0, 0, blit2.width, blit2.height);
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                    glDepthFunc(GL_LEQUAL);
                    blit2.UseShader();
                    blit2.shader.setFloat("texelsize_h", 1.0f / (float)width);
                    blit2.DrawQuad(temp5.texture.id, false);
                    blit2.Blit(temp6);
                    blit2.BindFrameBufferOver();

                }


                blit5.BindFrameBufferInit();
                glViewport(0, 0, blit5.width, blit5.height);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit5.UseShader();
                blit5.shader.setFloat("N", width);
                blit5.shader.setFloat("radius", radius);
                blit5.shader.setFloat("Scale", scale_Wave);
                blit5.DrawQuad(temp6.texture.id, false);
                blit5.Blit(temp4);
                blit5.BindFrameBufferOver();
                blit6.BindFrameBufferInit();
                glViewport(0, 0, blit6.width, blit6.height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit6.UseShader();
                glActiveTexture(GL_TEXTURE0 + 1);
                temp4.texture.nameInShader = "particle_map";
                blit6.shader.setInt(temp4.texture.nameInShader, 1);
                glBindTexture(GL_TEXTURE_2D, temp4.texture.id);
                blit6.shader.setFloat("N", width);
                blit6.shader.setFloat("radius", radius);
                blit6.shader.setFloat("Scale", scale_Wave);
                blit6.DrawQuad(temp6.texture.id, false);
                blit6.Blit(temp5);
                blit6.BindFrameBufferOver();
                WaveParticleShader.texes_output[0].id = temp5.texture.id;
                // over


            }
            else
            {

                WaveParticleShader.use();
                WaveParticleShader.setInt("N", indexArray.size());
                WaveParticleShader.setInt("Input_N", Input_N);
                WaveParticleShader.setFloat("Scale", scale_Wave);
                WaveParticleShader.dispatch();
                WaveParticleShader.wait();
            }
            {
                //将Wavemap拉伸到1024*1024
                blit.BindFrameBufferInit();
                glViewport(0, 0, blit.width, blit.height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit.DrawQuad(WaveParticleShader.texes_output[0].id, true);
                blit.Blit(wavemap);
                blit.BindFrameBufferOver();


                blit4.BindFrameBufferInit();
                glViewport(0, 0, blit4.width, blit4.height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit4.DrawQuad(waveImage1.id, true);
                blit4.Blit(temp3);
                blit4.BindFrameBufferOver();
                blit4.Draw();


                blit3.BindFrameBufferInit();
                glViewport(0, 0, blit3.width, blit3.height);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glDepthFunc(GL_LEQUAL);
                blit3.UseShader();
                glActiveTexture(GL_TEXTURE0 + 1);
                temp3.texture.nameInShader = "waveMap";
                blit3.shader.setInt(temp3.texture.nameInShader, 1);
                blit3.shader.setFloat("Time", Time);
                blit3.shader.setVec4("Scale_control", scale_Control_WaveParicle);
                glBindTexture(GL_TEXTURE_2D, temp3.texture.id);
                blit3.DrawQuad(wavemap.texture.id, false);
                blit3.Blit(temp2);
                blit3.BindFrameBufferOver();

                for (int i = 0; i < 9; i++)
                {


                    {
                        blit1.BindFrameBufferInit();
                        glViewport(0, 0, blit1.width, blit1.height);
                        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                        glClear(GL_COLOR_BUFFER_BIT);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                        glDepthFunc(GL_LEQUAL);
                        blit1.UseShader();
                        blit1.shader.setFloat("texelsize_v", 2.0f / (float)height);
                        blit1.DrawQuad(temp2.texture.id, false);
                        blit1.Blit(temp1);
                        blit1.BindFrameBufferOver();





                    }

                    blit2.BindFrameBufferInit();
                    glViewport(0, 0, blit2.width, blit2.height);
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                    glDepthFunc(GL_LEQUAL);
                    blit2.UseShader();
                    blit2.shader.setFloat("texelsize_h", 2.0f / (float)width);
                    blit2.DrawQuad(temp1.texture.id, false);
                    blit2.Blit(temp2);
                    blit2.BindFrameBufferOver();

                }

                WaveParticleShader.texes_output[0].id = temp2.texture.id;


            }





            normalShader1.texes_input[0] = WaveParticleShader.texes_output[0];
            normalShader1.use();
            normalShader1.setFloat("BubblesScale", BubblesScale);
            normalShader1.setFloat("BubblesThreshold", BubblesThreshold);
            normalShader1.setFloat("jacobScale", jacobScale);
            normalShader1.setFloat("resolutioninv", 2.0f * scale_Ocean / (float)width);
            normalShader1.dispatch();
            normalShader1.wait();


        }




        glViewport(0, 0, shadowwidth, shadowheight);
        depthMap.BindFrameBufferInit();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);

        glDisable(GL_CULL_FACE);
        ubotest.SetMatrix(projection);
        ubotest.SetMatrix(view);//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
        depthShader.use();
        depthShader.setMatrix("lightProjection", lightProjection);
        depthShader.setMatrix("lightView", lightView);
        depthShader.setMatrix("model", model);
        ourModel.Draw(depthShader);
        depthOceanShader.use();
        depthOceanShader.setMatrix("lightProjection", lightProjection);
        depthOceanShader.setMatrix("lightView", lightView);
        depthOceanShader.setMatrix("model", model);
        depthOceanShader.setMatrix("model", floormodel);
        if (enableWaveParticle) {
            //  WaveParticleShader.texes_output[0].id = waveImage1.id;
            WaveParticleShader.texes_output[0].nameInShader = "_displacementMap";
            rt[1] = WaveParticleShader.texes_output[0];

        }
        else {
            DisplacementShader.texes_output[3].nameInShader = "_displacementMap";
            rt[1] = DisplacementShader.texes_output[3];

        }
        glActiveTexture(GL_TEXTURE0);
        depthOceanShader.setInt("_displacementMap", 0);
        glBindTexture(GL_TEXTURE_2D, rt[1].id);
        roomModel.Draw(depthOceanShader);
        depthMap.BindFrameBufeerOver();
        ubotest.ClearSizeNow();

        rt[0] = depthMap.texture;
        {

            //将Wavemap拉伸到1024*1024
            copy.BindFrameBufferInit();
            glViewport(0, 0, copy.width, copy.height);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDepthFunc(GL_LEQUAL);
            glDepthFunc(GL_LEQUAL);
            ubotest.SetMatrix(projection);
            ubotest.SetMatrix(view * camera.CaculateReflectMatrix(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0)));//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
            skyBoxShader.use();

            skyBoxShader.setMatrix("model", modelskyBox);
            skyBoxShader.setMatrix("projection", projection);
            glm::mat4 viewskyBox = glm::mat4(glm::mat3(view)) * camera.CaculateReflectMatrix(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
            skyBoxShader.setMatrix("view", viewskyBox);
            cubemap.Draw(skyBoxShader);
            glDepthFunc(GL_LESS);
            myshader.use();
            myshader.setMatrix("model", model);
            myshader.setVec3("baseColor", glm::vec3(0.5, 0.5, 0.5));
            myshader.setVec3("cameraPos", camera.GetPos());
            myshader.setMatrix("lightMat", (lightProjection * lightView));
            for (unsigned int i = 0; i < 4; i++)
            {
                // calculate the model matrix for each object and pass it to shader before drawing
                lights[i].SetLight(myshader);
            }
            ourModel.Draw(myshader, rt);
            copy.Blit(temp6);
            copy.BindFrameBufferOver();
            ubotest.ClearSizeNow();
        }//draw reflect

        glViewport(0, 0, width, height);
        posteffect.BindFrameBufferInit();


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



        glDepthFunc(GL_LEQUAL);
        ubotest.SetMatrix(projection);
        ubotest.SetMatrix(view);//* camera.CaculateReflectMatrix(glm::vec3(0,1,0),glm::vec3(0,0,0))
        skyBoxShader.use();

        skyBoxShader.setMatrix("model", modelskyBox);
        skyBoxShader.setMatrix("projection", projection);
        glm::mat4 viewskyBox = glm::mat4(glm::mat3(view));
        skyBoxShader.setMatrix("view", viewskyBox);
        cubemap.Draw(skyBoxShader);
        glDepthFunc(GL_LESS);
        myshader.use();
        myshader.setMatrix("model", model);
        myshader.setVec3("baseColor", glm::vec3(0.5, 0.5, 0.5));
        myshader.setVec3("cameraPos", camera.GetPos());
        myshader.setMatrix("lightMat", (lightProjection * lightView));
        for (unsigned int i = 0; i < 4; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            lights[i].SetLight(myshader);
        }
        /* glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
         glStencilMask(0xFF);

         glStencilFunc(GL_ALWAYS, 2, 0xFF);
         ourModel.Draw(myshader);
         glStencilMask(0x00);
         myshader.setMatrix("model", model);
         glStencilFunc(GL_EQUAL, 2, 0xFF);
         model = glm::translate(model,glm::vec3(0,0,3));

         lightshader.setMatrix("model", model);
         ourModel.Draw(lightshader);
         glStencilMask(0xFF);*/
         //ourModel.Draw(myshader, rt);

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
        FloorShader.use();
        for (unsigned int i = 0; i < 4; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            lights[i].SetLight(FloorShader);
        }
        FloorShader.setMatrix("model", floormodel);
        FloorShader.setMatrix("model_inverse_t", floormodel_inverse_t);
        FloorShader.setFloat("Time", 2 * Time);
        FloorShader.setVec3("cameraPos", camera.GetPos());
        FloorShader.setMatrix("lightMat", (lightProjection * lightView));
        FloorShader.setFloat("Scale_subsurface", scale_subsurface);

        if (enableWaveParticle) {
            //  WaveParticleShader.texes_output[0].id = waveImage1.id;
            WaveParticleShader.texes_output[0].nameInShader = "_displacementMap";
            rt[1] = WaveParticleShader.texes_output[0];
            normalShader1.texes_output[0].nameInShader = "_normalMap";
            rt[2] = (normalShader1.texes_output[0]);
            normalShader1.texes_output[1].nameInShader = "_bubbleMap";
            rt[3] = (normalShader1.texes_output[1]);
            waveImage1.nameInShader = "_waveMap";
            rt[4] = (waveImage1);
        }
        else {
            DisplacementShader.texes_output[3].nameInShader = "_displacementMap";
            rt[1] = DisplacementShader.texes_output[3];
            normalShader.texes_output[0].nameInShader = "_normalMap";
            rt[2] = (normalShader.texes_output[0]);
            normalShader.texes_output[1].nameInShader = "_bubbleMap";
            rt[3] = (normalShader.texes_output[1]);
            temp6.texture.nameInShader = "_reflectionMap";
            rt[4] = (temp6.texture);

        }




        roomModel.Draw(FloorShader, rt, &cubemap);




        posteffect.BindFrameBufferOver();

        posteffect.Draw();
        //posteffect.DebugDraw(DisplacementShader.texes_output[3].id);
        //posteffect.DebugDraw(WaveParticleShader.texes_output[0].id);
        // glViewport(0, 0, width, height);
         //posteffect.DebugDraw(WaveParticleShader.texes_output[0].initid);
        if (showwindow)
        {
            lightshader.use();
            lightshader.setVec3("lightColor", lightColor);

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
            ImGui::SliderFloat4("WindSeed", (float*)&(windspeed), 0.0f, 50.0f);
            ImGui::SliderFloat("WaveScale", (float*)&(scale_Wave), 0.0f, 2.0f);
            ImGui::SliderFloat4("Scale_control_WaveParticle", (float*)&(scale_Control_WaveParicle), 0.0f, 2.0f);
            ImGui::SliderFloat("Scale_subsurface", (float*)&(scale_subsurface), 0.0f, 1.0f);
            for (unsigned int i = 0; i < 4; i++)
            {
                // calculate the model matrix for each object and pass it to shader before drawing
                glm::mat4 modellight = glm::mat4(1.0f);

                //if (lights[i].lightType == DIRLIGHT)
                {
                    if (lights[i].lightType == POINTLIGHT) {
                        /*   if(lights[i].pos.x>0)
                               posteffect.DebugDraw(GaussianBlur_v.texes_output[0].id);
                           else
                               posteffect.DebugDraw(normalShader1.texes_output[1].id);*/
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
                    lightshader.setMatrix("model", modellight);

                    // lights[i].Draw();  //draw cube

                }
            }
            ImGui::End();
        }



        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwPollEvents();
        glfwSwapBuffers(window);

    }

    /*   glDeleteVertexArrays(1, &VAO);
       glDeleteVertexArrays(1, &LightVAO);
       glDeleteBuffers(1, &VBO);*/
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}