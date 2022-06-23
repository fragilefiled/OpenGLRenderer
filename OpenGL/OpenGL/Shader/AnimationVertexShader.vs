# version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;
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
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
void main()
{
	
	//gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//ourColor = aColor;
	modelPos=lightMat*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vs_out.worldPos =vec3(model * vec4(aPos, 1.0));
	//vs_out.normal=normalize(mat3(transpose(inverse(model)))*aNormal);
	vec4 totalPosition = vec4(0.0f);
	for(int i=0;i<MAX_BONE_INFLUENCE;i++)
	{
		if(boneIds[i]==-1)
			continue;
		if(boneIds[i]>MAX_BONES){
			totalPosition = vec4(aPos,1.0f);
            break;
		}
		totalPosition+=weights[i]*(finalBonesMatrices[boneIds[i]]*vec4(aPos,1.0f));

	};
	gl_Position=projection*view*model*totalPosition;
	vec3 normal=normalize(aNormal);
	vs_out.normal=normalize(mat4(model_inverse_t)*vec4(normal,0.0)).xyz;
	lastnormal=projection*view*model*vec4(aPos.x+aNormal.x,aPos.y+aNormal.y,aPos.z+aNormal.z,1.0);
	
	vs_out.texCoord=aTexcoord;
}