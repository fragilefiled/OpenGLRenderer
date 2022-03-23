#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;
out vec2 uv;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);

	//ourColor = aColor;
    uv=aTexcoord;

	
}