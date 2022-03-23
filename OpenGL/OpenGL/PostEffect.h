#pragma once
#include "ShaderCompile.h"
#include "FBO.h"
class  PostEffect
{
public:
    PostEffect(Shader &shader, int width, int height, bool msaaEnable = false, GLenum format = GL_RGB, GLenum format1 = GL_RGB, GLenum storeform=GL_UNSIGNED_BYTE,bool noRepeat=true);
     void BindQuad();
     void SetFrameBuffer();
     void Draw();
     void DebugDraw(unsigned int test_RT);
     void BindFrameBufferOver();
     void BindFrameBufferInit();
     ~ PostEffect();
     void Blit(FBO &fbo);

     void UseShader();

     void DrawQuad(unsigned int test_RT, bool use_shader=true);
     void DrawQuadMS(unsigned int test_RT, bool use_shader=true);
     //void DrawQuad(unsigned int test_RT);
 
    unsigned int quadVAO, quadVBO;
    unsigned int framebuffer, intermediateFBO;
    unsigned int textureColorbuffer,screenTexture;
    unsigned int rbo;
    static std::vector<unsigned int> allVAO;
    static std::vector<unsigned int> allVBO;
    GLenum format;
    GLenum format1;
    GLenum storeform;
    bool msaaEnable;
    int width;
    int height;
    Shader shader;
    bool noRepeat;
   static constexpr float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
       // positions   // texCoords
       -1.0f,  1.0f,  0.0f, 1.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

       -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
private:

};


