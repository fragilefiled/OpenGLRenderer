#version 330 core
out vec4 FragColor;
in vec4 voxelColor;
void main(){
//FragColor=vec4(ourColor,1.0f);

FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z);
}