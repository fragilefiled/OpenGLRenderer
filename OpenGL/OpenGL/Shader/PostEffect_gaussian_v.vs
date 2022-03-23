#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;
out vec2 uv0;
out vec2 uv1;
out vec2 uv2;
out vec2 uv3;
out vec2 uv4;
uniform float texelsize_v;
out float depth;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    depth=aPos.z;
	//ourColor = aColor;
    uv2=aTexcoord;
    uv0=uv2+vec2(0,-2)*texelsize_v;
    uv1=uv2+vec2(0,-1)*texelsize_v;
    uv3=uv2+vec2(0,1)*texelsize_v;
    uv4=uv2+vec2(0,2)*texelsize_v;

	
}