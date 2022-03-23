# version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
layout (location = 7) in mat4 instanceMatrix;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
 vec3 worldPos;
  vec3 normal;
 vec2 texCoord;
 
}vs_out;
out mat4 matout;
//out vec4 lastnormal;
void main()
{
	gl_Position = projection*view*instanceMatrix*vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//ourColor = aColor;

	vs_out.worldPos =vec3(model * vec4(aPos, 1.0));
	vs_out.normal=normalize(mat3(transpose(inverse(model)))*aNormal);
	//lastnormal=projection*view*model*vec4(aPos.x+aNormal.x,aPos.y+aNormal.y,aPos.z+aNormal.z,1.0);
	vs_out.texCoord=aTexcoord;
}