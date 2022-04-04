#version 450 core
in vec2 uv;
uniform sampler2D screen_RT;
out vec4 FragColor;
in float depth;
uniform float Time;
uniform sampler2D IndirLightTexture;
void main(){

    FragColor=texture(IndirLightTexture,uv)+texture(screen_RT,uv);

}