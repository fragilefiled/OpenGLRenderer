#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 attri;
out vec2 uv;
out vec4 attri_use;
void main()
{
	gl_Position = vec4(aPos, 0.0, 1.0);
    uv=aPos;
	attri_use=attri;
}