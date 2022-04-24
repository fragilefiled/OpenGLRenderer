#include"PostEffect.h"

 std::vector<unsigned int> PostEffect::allVAO;
 std::vector<unsigned int> PostEffect::allVBO;
PostEffect::PostEffect(Shader& shader,int width,int height,bool msaaEnable, GLenum format, GLenum format1, GLenum storeform,bool noRepeat):width(width),height(height),shader(shader),msaaEnable(msaaEnable),format(format), format1(format1),storeform(storeform),noRepeat(noRepeat)
{
   
}

void PostEffect::BindQuad()
{
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    allVAO.push_back(quadVAO);
    allVBO.push_back(quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  
}
void PostEffect::SetFrameBuffer() 
{


    if (!msaaEnable) {
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // create a color attachment texture

        glGenTextures(1, &textureColorbuffer);
        Texture::allTextures.push_back(textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format1, storeform, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    else
    {
        
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // create a multisampled color attachment texture

        glGenTextures(1, &textureColorbuffer);
        Texture::allTextures.push_back(textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer, 0);
        // create a (also multisampled) renderbuffer object for depth and stencil attachments
        
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // configure second post-processing framebuffer
        

        glGenFramebuffers(1, &intermediateFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
        // create a color attachment texture
        
        glGenTextures(1, &screenTexture);
        Texture::allTextures.push_back(screenTexture);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
void PostEffect::Draw() 
{

    glActiveTexture(GL_TEXTURE0);
    shader.use();
    shader.setInt("screen_RT", 0);
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(quadVAO);
    if(!msaaEnable)
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
    else
      glBindTexture(GL_TEXTURE_2D, screenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

 
  


}
void PostEffect::DebugDraw(unsigned int test_RT) 
{

    glActiveTexture(GL_TEXTURE0);
    shader.use();
    shader.setInt("screen_RT", 0);
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, test_RT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    
};
void PostEffect::BindFrameBufferOver()
{
    if(msaaEnable)
    {

        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
        glBlitFramebuffer(0, 0, width ,height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, intermediateFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        
    }
    else
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
   
}
void PostEffect::BindFrameBufferInit() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}
PostEffect::~PostEffect()
{
    glDeleteFramebuffers(1, &framebuffer);
    if(msaaEnable)
        glDeleteFramebuffers(1, &intermediateFBO);
    glDeleteRenderbuffers(1, &rbo);
}

void PostEffect::Blit(FBO& fbo)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.fboBuffer);
      glBlitFramebuffer(0, 0, width, height, 0, 0, fbo.width, fbo.height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
      {
          GLenum glError = glGetError();
          if (glError != GL_NO_ERROR)
          {
             // std::cout << glError<<std::endl;

          }
      }
}
void PostEffect::UseShader() 
{

    shader.use();


}
void PostEffect::DrawQuad(unsigned int test_RT,bool use_shader) {

    glActiveTexture(GL_TEXTURE0);
    if (use_shader) {
        shader.use();
    }
    shader.setInt("screen_RT", 0);
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    //glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, test_RT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}
void PostEffect::DrawQuadMS(unsigned int test_RT, bool use_shader) {

    glActiveTexture(GL_TEXTURE0);
    if (use_shader) {
        shader.use();
    }
    shader.setInt("screen_RT", 0);
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, test_RT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

