#version 450 core

in VS_OUT
{
 vec3 worldPos;
  vec3 normal;
 vec2 texCoord;
}fs_in;

layout(location = 0) out vec3 gWorldPos;
layout(location = 1) out vec3 gNomral;
layout(location = 2) out vec4 gAlbedoSpec;
layout(location = 3) out float gDepth;

uniform sampler2D texture_Diffuse_0;
uniform sampler2D texture_Normal_0;
uniform sampler2D texture_Specular_0;
void main(){

      vec3 diffuse=texture(texture_Diffuse_0,fs_in.texCoord).rgb;
      vec3 spec=texture(texture_Specular_0,fs_in.texCoord).rgb;
      gWorldPos=fs_in.worldPos;
       if( !gl_FrontFacing )
       gNomral=-fs_in.normal;
       else
      gNomral=fs_in.normal;
      gAlbedoSpec=vec4(diffuse,spec.x);
      gDepth=0.1;
  
   
}