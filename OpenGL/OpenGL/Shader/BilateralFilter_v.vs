#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;
out vec2 uv0[2];
out vec2 uv1[2];
out vec2 uv2[2];
out vec2 uv3;
uniform float texelsize_v;
out float depth;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    depth=aPos.z;
	  uv3=aTexcoord;
    uv0[0]=uv3+vec2(0,-1)*texelsize_v;
    uv0[1]=uv3+vec2(0,1)*texelsize_v;
    uv1[0]=uv3+vec2(0,-2)*texelsize_v;
    uv1[1]=uv3+vec2(0,2)*texelsize_v;
    uv2[0]=uv3+vec2(0,-3)*texelsize_v;
    uv2[1]=uv3+vec2(0,3)*texelsize_v;
}
	