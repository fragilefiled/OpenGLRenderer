#version 450 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
layout (location = 3) in vec3 aTangent;
uniform mat4 model;
//uniform mat4 view;

//uniform mat4 projection;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
	mat4 preprojection;
	mat4 preview;
};

out VS_OUT{
 vec3 worldPos;
  vec3 normal;
 vec2 texCoord;
vec3 tangent;
}vs_out;
out vec3 TBN1;
out vec3 TBN2;
out vec3 TBN3;
out vec4 nowScreenPos;
out vec4 preScreenPos;

uniform mat4 jitterProjection;
uniform mat4 mvp;
uniform mat4 model_inverse_t;
uniform int frameNow;
uniform mat4 preModel;
void main()
{
	//gl_Position = mvp*vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position = jitterProjection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
	nowScreenPos=projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
	preScreenPos=preprojection*preview*preModel*vec4(aPos.x, aPos.y, aPos.z, 1.0);

	//ourColor = aColor;
	vs_out.worldPos =vec3(model * vec4(aPos, 1.0));
	//vs_out.normal=normalize(mat3(transpose(inverse(model)))*aNormal);
	vec3 normal=normalize(aNormal);
	vs_out.normal=normal;//normalize(mat4(model_inverse_t)*vec4(normal,0.0)).xyz;
	
	vec3 worldnormal=normalize(mat4(model_inverse_t)*vec4(normal,0.0)).xyz;
	vec3 worldtangent=normalize(model*vec4(aTangent,0.0)).xyz;
	vec3 worldbiotangent=normalize(cross(worldnormal,worldtangent));
   TBN1=vec3(worldtangent.x,worldbiotangent.x,worldnormal.x);
    TBN2=vec3(worldtangent.y,worldbiotangent.y,worldnormal.y);
    TBN3=vec3(worldtangent.z,worldbiotangent.z,worldnormal.z);
	vs_out.texCoord=aTexcoord;
	vs_out.tangent=worldtangent;
}