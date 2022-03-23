# version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
uniform mat4 model;
//uniform mat4 view;
uniform mat4 lightView;
uniform mat4 lightProjection;
uniform sampler2D _displacementMap;
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
    vec3 displace4= texture(_displacementMap,vec2((aTexcoord))).xyz;
	gl_Position = lightProjection*lightView*model*vec4(aPos.x+displace4.x,aPos.y+displace4.y, aPos.z+displace4.z, 1.0);

}