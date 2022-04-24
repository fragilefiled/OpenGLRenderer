

#pragma once
#include<iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "TextureImage.h"
#include<vector>
#include"Texture3D.h"
class Shader
{
    
public:
    static std::vector<int> allShaders;
    unsigned int ID;
    Shader();
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
            if (geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if (geometryPath != nullptr)
        {
            const char* gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        
        // shader Program
        ID = glCreateProgram();
        allShaders.push_back(ID);
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
            glDeleteShader(geometry);

    }
    ~Shader() {

        
    }
public:
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setMatrix(const std::string& name, glm::mat4 value) const
    {
        unsigned int matrixLoc = glGetUniformLocation(ID, name.c_str());
        glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(value));
        
    }
    void setMatrixArray(const std::string& name, std::vector<glm::mat4> matArray) const
    {
        for (int i = 0; i < matArray.size(); i++) {
            std::string realname = (name + "[" + std::to_string(i) + "]");
            unsigned int matrixLoc = glGetUniformLocation(ID,realname.c_str());
            glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(matArray[i]));
        }

    }
    void setMatrix3(const std::string& name, glm::mat3 value) const
    {
        unsigned int matrixLoc = glGetUniformLocation(ID, name.c_str());
        glUniformMatrix3fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(value));

    }
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
  
protected:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

class ComputeShader:public Shader
{
public:

    unsigned int TextureID;
    std::vector<TextureImage>texes_input= std::vector<TextureImage>(1);
    std::vector<TextureImage>texes_output = std::vector<TextureImage>(1);
    std::vector<Texture3D>texes3d = std::vector<Texture3D>(0);
    std::vector<int>lods=std::vector<int>(0);
    int tex_output_num = 0;
    int tex_input_num = 0;
    int tex3d_num = 0;
    bool enableMipmap = false;
    bool input_auto = false;
    glm::vec3 work_size;
    ComputeShader(const char* computePath, glm::vec3 work_size, glm::vec2 tex_size,unsigned int tex_output_num=1, unsigned int tex_input_num = 1,unsigned int tex3d_num=0,bool input_auto=false,bool enableMipmap=false,std::vector<int> lods=std::vector<int>(0)) :work_size(work_size),input_auto(input_auto),enableMipmap(enableMipmap)
    {
        
        // 1. retrieve the vertex/fragment source code from filePath
        std::string computeCode;
        std::ifstream cShaderFile;

        // ensure ifstream objects can throw exceptions:
        cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            cShaderFile.open(computePath);
            std::stringstream cShaderStream;
            // read file's buffer contents into streams
            cShaderStream << cShaderFile.rdbuf();

            // close file handlers
            cShaderFile.close();

            // convert stream into string
            computeCode = cShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
        
   
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* cShaderCode = computeCode.c_str();
        // 2. compile shaders
        unsigned int compute;
        // 2. compile shaders
        compute = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compute, 1, &cShaderCode, NULL);
        glCompileShader(compute);
        ID = glCreateProgram();
        allShaders.push_back(ID);
        glAttachShader(ID, compute);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        glDeleteShader(compute);

 
        int tex_w = tex_size.x, tex_h = tex_size.y;
       // TextureImage output=ne TextureImage()
       // tex_output=
        texes_output = std::vector<TextureImage>(tex_output_num);
        texes_input = std::vector<TextureImage>(tex_input_num);
        texes3d = std::vector<Texture3D>(tex3d_num);
        for (int i = 0; i < texes_output.size(); i++) 
        {
            texes_output[i] = TextureImage(tex_w, tex_h);
            glActiveTexture(GL_TEXTURE+i);
            texes_output[i].id = texes_output[i].Bind();
            //glBindImageTexture(0, texes_output[i].id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
        }
        if (input_auto) 
        {
            for (int i = 0; i < texes_input.size(); i++)
            {
                texes_input[i] = TextureImage(tex_w, tex_h);
                glActiveTexture(GL_TEXTURE + i + texes_output.size());
                texes_input[i].id = texes_input[i].Bind();
               // glBindImageTexture(0, texes_input[i].id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
            }
        }
        this->tex_output_num = tex_output_num;
        this->tex_input_num = tex_input_num;
        this->tex3d_num = tex3d_num;
        if (!enableMipmap)
            this->lods = std::vector<int>(tex_output_num + tex_input_num + tex3d_num, 0);
        else
            this->lods = lods;
    }
    void use() {
        glUseProgram(ID);
    /*    glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);*/
  /*      glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_output_id);
        glBindImageTexture(0, tex_output_id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);*/
        int bindImageNum = 0;
        for (int i = 0; i < texes_output.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texes_output[i].id);
            glBindImageTexture(i , texes_output[i].id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
          //  glGenerateMipmap(GL_TEXTURE_2D);
            bindImageNum++;
        }
        for (int i = 0; i < texes_input.size(); i++) {
            glActiveTexture(GL_TEXTURE0+i+ texes_output.size());
            glBindTexture(GL_TEXTURE_2D, texes_input[i].id);
            glBindImageTexture(i+ texes_output.size(), texes_input[i].id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
           // glGenerateMipmap(GL_TEXTURE_2D);
            bindImageNum++;
        }
        
        for (int i = 0; i < texes3d.size(); i++) 
        {
            glActiveTexture(GL_TEXTURE0 + i + texes_input.size() + texes_output.size());
            setInt(texes3d[i].nameInShader, i + texes_input.size() + texes_output.size());
            glBindTexture(GL_TEXTURE_3D, texes3d[i].id);
            int level = 0;
            if (enableMipmap)
                level = lods[i];
            if (texes3d[i].enableTextureImage) {
                if (texes3d[i].rw == 3)
                    glBindImageTexture(bindImageNum, texes3d[i].id, level, GL_FALSE, 0, GL_READ_WRITE, texes3d[i].format);
                else if (texes3d[i].rw == 2)
                    glBindImageTexture(bindImageNum, texes3d[i].id, level, GL_FALSE, 0, GL_WRITE_ONLY, texes3d[i].format);
                else
                    glBindImageTexture(bindImageNum, texes3d[i].id, level, GL_FALSE, 0, GL_READ_ONLY, texes3d[i].format);
                bindImageNum++;
                /*  if (i == texes3d.size() - 1) {
                      glGenerateMipmap(GL_TEXTURE_3D);
                  }*/
            }
        }
    }
    void dispatch() {
        // just keep it simple, 2d work group
        glDispatchCompute(work_size.x, work_size.y, work_size.z);
    }
    void setTextureID(unsigned textureID) {
        TextureID = textureID;
    }
    void wait() {
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
    ~ComputeShader() {

        
    }
private:
   



};
