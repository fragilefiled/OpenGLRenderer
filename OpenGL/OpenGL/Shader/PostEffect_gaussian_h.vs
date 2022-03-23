#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;
out vec2 uv0;
out vec2 uv1;
out vec2 uv2;
out vec2 uv3;
out vec2 uv4;
uniform float texelsize_h;
out float depth;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    depth=aPos.z;
	//ourColor = aColor;
    uv2=aTexcoord;
    uv0=uv2+vec2(-2,0)*texelsize_h;
    uv1=uv2+vec2(-1,0)*texelsize_h;
    uv3=uv2+vec2(1,0)*texelsize_h;
    uv4=uv2+vec2(2,0)*texelsize_h;

	
}