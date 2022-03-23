# version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

uniform mat4 model;
//uniform mat4 view;

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
uniform mat4 lightMat;
out vec4 modelPos;
out vec4 lastnormal;
uniform mat4 model_inverse_t;
void main()
{
	
	gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//ourColor = aColor;
	modelPos=lightMat*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vs_out.worldPos =vec3(model * vec4(aPos, 1.0));
	//vs_out.normal=normalize(mat3(transpose(inverse(model)))*aNormal);
	vs_out.normal=normalize(mat3(model_inverse_t)*aNormal);
	lastnormal=projection*view*model*vec4(aPos.x+aNormal.x,aPos.y+aNormal.y,aPos.z+aNormal.z,1.0);
	
	vs_out.texCoord=aTexcoord;
}