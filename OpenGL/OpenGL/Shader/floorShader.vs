# version 450 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
uniform sampler2D _displacementMap;

//uniform mat4 view;
uniform sampler2D _normalMap;
//uniform mat4 projection;
uniform mat4 model;
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
out vec3 aPos_uv;
uniform float Time;
//out vec4 lastnormal;
float mipmapLevel(vec2 dxy, vec2 textureSize) 
{    
  
 float d = max(dot(dxy.x, dxy.x), dot(dxy.y, dxy.y));
 return 0.5 * log2(d);//0.5是技巧，本来是d的平方。
} 
void main()
{
	
	vec3 displace=texture(_displacementMap,aPos.xy).xyz;
	vec3 displace1=texture(_displacementMap,aPos.yz).xyz;
	vec3 displace2=texture(_displacementMap,aPos.zx).xyz;
	vec3 displace3=texture(_displacementMap,0.01*aPos.xz).xyz;
	//float x=sin(Time*Time+aPos.x);
	float a=0.5;
	// vec2 uv_world=vec2(0,0);
	// float x_now=(aPos.x+1.0)/2.0;
	
	float uv_world=(aPos.x+1.0)*2.0;

	float x_now=uv_world-floor(uv_world);
	float c=0.5;

	//  if(x_now>0.5)
	//  uv_world=x_now-(x_now-1)*(x_now-1);
	//  else
	//  uv_world=x_now+(x_now-0)*(x_now-0);



	// if(x_now>c)
	// uv_world=uv_world-(x_now-c)*(x_now-c);
	// if(x_now<c)
	// uv_world=uv_world+(x_now-c)*(x_now-c);
	//uv_world=abs(sin(x_now*3.14/2.0));
	uv_world=x_now;
	//uv_world=4*x_now*x_now*x_now-6*x_now*x_now+3*x_now;
	vec3 displace4= texture(_displacementMap,vec2((aTexcoord))).xyz;

	gl_Position = projection*view*model*vec4(aPos.x+displace4.x,aPos.y+displace4.y, aPos.z+displace4.z, 1.0);
	aPos_uv=gl_Position.xyz/gl_Position.w;
	//gl_Position = projection*view*model*vec4(aPos,1.0);
	//gl_Position = projection*view*model*(vec4(aPos+displace4, 1.0));
	//ourColor = aColor;
	
	modelPos=lightMat*model*vec4(aPos.x+displace4.x,aPos.y+displace4.y, aPos.z+displace4.z, 1.0);
	vs_out.worldPos =vec3(model * vec4(aPos.x+displace4.x,aPos.y+displace4.y, aPos.z+displace4.z, 1.0));
	vs_out.normal=normalize(mat3(transpose(inverse(model)))*aNormal);
	
	//vs_out.normal=normalize(mat3(transpose(inverse(model)))*texture(_normalMap,aPos.xz).xyz);
	//lastnormal=projection*view*model*vec4(aPos.x+aNormal.x,aPos.y+aNormal.y,aPos.z+aNormal.z,1.0);
	vs_out.texCoord=aTexcoord;

}