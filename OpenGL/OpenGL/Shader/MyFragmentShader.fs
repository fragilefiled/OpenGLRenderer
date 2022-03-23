#version 330 core
in vec3 ourColor;
out vec4 FragColor;
in  vec2 texCoord;
uniform sampler2D Texture;
uniform sampler2D Texture1;
void main(){
//FragColor=vec4(ourColor,1.0f);
FragColor=texture(Texture1, texCoord);
}