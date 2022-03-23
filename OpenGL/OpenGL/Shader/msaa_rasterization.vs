# version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;
out vec2 uv;
out float depth;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    depth=aPos.z;
	//ourColor = aColor;
    uv=aTexcoord;
	
}