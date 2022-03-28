# version 430 core


in vec4 voxelColor;
out vec4 FragColor;
in vec4 worldPos;
void main()
{

	FragColor=voxelColor*4.0;

}