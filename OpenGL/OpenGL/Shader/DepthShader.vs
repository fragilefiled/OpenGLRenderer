# version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
uniform mat4 model;
//uniform mat4 view;
uniform mat4 lightView;
uniform mat4 lightProjection;
//uniform mat4 projection;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT{
 vec3 worldPos;
  vec3 normal;
 vec2 texCoord;
 
}vs_out;

//out vec4 lastnormal;
void main()
{
	gl_Position = lightProjection*lightView*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);

}