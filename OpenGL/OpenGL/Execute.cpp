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
#include "B_Spline.h"
#include"WaveProcess.h"
#include"VoxelProcess.h"

static bool showwindow = true;
glm::vec3 pos = glm::vec3(0.0f, 15.0f, -3.0f);
//glm::vec3 pos = glm::vec3(0.0f, 6.0f, -3.0f);
glm::vec3 frontDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 upDir = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera = Camera(pos, frontDir, upDir);
glm::mat4 view;
glm::vec4 windspeed = glm::vec4(11.0f, 23.0f, 28.0f, 0.0f);
glm::vec3 limit = glm::vec3(0.2f, 0.4f, 1.0f);
float cameraSpeed = 2.0f;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
float deltaTimeOptimazation = 0.0f; // 当前帧与上一帧的时间差
float lastFrameOptimazation = 0.0f; // 上一帧的时间
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
float B_Spline_count = 1024;
float scale_Wave = 0.2f;
float scale_Ocean = 10.0f;
float scale_subsurface = 1.0f;
glm::vec4 scale_Control_WaveParicle = glm::vec4(0.3f, 1.0f, 0.8, 0.0f);
//glm::vec4 scale_Control_WaveParicle = glm::vec4(0.0f, 1.0f, 0.0, 0.0f);
int wave_particle_resolution = 64;
int wave_particle_resolution_fliter = 2048;
float radius = 100;
bool enableWaveParticle = true;
bool enableWaveParticle_hvfliter = true;
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

        camera.Move(LEFT, deltaTime,cameraSpeed);

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        camera.Move(RIGHT, deltaTime, cameraSpeed);

    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {


        camera.Move(FORWARD, deltaTime, cameraSpeed);

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

        camera.Move(BACK, deltaTime, cameraSpeed);

    }

}

void CalculateDeltaTime() {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

}
void CalculateDeltaTimeOP() {

    float currentFrame = glfwGetTime();
    deltaTimeOptimazation = currentFrame - lastFrameOptimazation;
    lastFrameOptimazation= currentFrame;

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
    glfwSwapInterval(0); // Enable vsync (Before screen swaps Buffer,it should wait the num of frame)
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


 //   std::vector<Texture> rt = std::vector<Texture>(7);
 /*   WaveProcess* process = new WaveProcess(camera);
    process->Init();*/
       VoxelProcess* process = new VoxelProcess(camera);
       process->Init();
    //int coutFrame = 0;
    
       auto  vendor = glGetString(GL_VENDOR);
       auto  renderer = glGetString(GL_RENDERER);
       int countFrame = 0;
       float timeFrame = 0;
       float timeFrameOptimazation = 0;
       float avgfps = 0;
    while (!glfwWindowShouldClose(window))
    {
       
        /*timeFrame++;
        timeFrame += deltaTime;
        if (timeFrame > 1.0f) {
            cout << timeFrame << endl;
            timeFrame = 0.0f;
            timeFrame = 0.0f;

        }*/
       /* CalculateDeltaTime();
        Time += deltaTime;*/
       // process1->SetCameraAndTime(camera, Time*1.00);
        CalculateDeltaTime();
        Time += deltaTime;
        process->SetCameraAndTime(camera, Time*1.0);
        // Start the Dear ImGui frame
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //glEnable(GL_MULTISAMPLE);

      
        //输入
        processInput(window);
       
      //  process1->Process();
        
      process->Process();
        
      
      
        
        
          
        
      CalculateDeltaTimeOP();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       // CalculateDeltaTimeOP();
        glfwPollEvents();
     
        glfwSwapBuffers(window);//swapBuffer Cost Lots of time
        //CalculateDeltaTimeOP();
        CalculateDeltaTimeOP();

        timeFrameOptimazation += deltaTimeOptimazation;
        timeFrame += deltaTime;
        countFrame++;
       
        if (countFrame == 60) {
            timeFrameOptimazation /= 60.0;
            timeFrame /= 60.0;
             avgfps = 1 / timeFrameOptimazation;
             cout << "Real: "<<1000*timeFrame<<" Optimazation: " <<1000* timeFrameOptimazation << endl;
             
             timeFrameOptimazation = 0.0;
             timeFrame = 0.0;
             countFrame = 0;
             
           
        }//need to Calc UI
        
        
    }
   
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  
      
        for (int i = 0; i < Texture::allTextures.size(); i++) {
            const GLuint a = Texture::allTextures[i];
            glDeleteTextures(1, &a);
        }
        for (int i = 0; i < Shader::allShaders.size(); i++) {
            const GLuint a = Shader::allShaders[i];
            glDeleteProgram(a);
        }
        for (int i = 0; i < PostEffect::allVAO.size(); i++)
        {
            glDeleteVertexArrays(1, &PostEffect::allVAO[i]);
            glDeleteBuffers(1, &PostEffect::allVBO[i]);
        }
        Texture::allTextures.clear();
        Shader::allShaders.clear();
        PostEffect::allVBO.clear();
        PostEffect::allVAO.clear();
        delete process;
       // delete process;
        

    

   
    glfwTerminate();

    
    return 0;
}