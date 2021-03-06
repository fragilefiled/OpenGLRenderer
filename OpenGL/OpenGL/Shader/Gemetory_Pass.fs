#version 450 core
layout(early_fragment_tests) in;
in VS_OUT
{
 vec3 worldPos;
  vec3 normal;
 vec2 texCoord;
 vec3 tangent;
}fs_in;

layout(location = 0) out vec3 gWorldPos;
layout(location = 1) out vec3 gNomral;
layout(location = 2) out vec4 gAlbedoSpec;
layout(location = 3) out vec2 gVelocity;
layout(location = 4) out vec3 gTangent;
in vec3 TBN1;
in vec3 TBN2;
in vec3 TBN3;
in vec4 nowScreenPos;
in vec4 preScreenPos;
uniform sampler2D texture_Diffuse_0;
uniform sampler2D texture_Normal_0;
uniform sampler2D texture_Specular_0;
uniform vec3 limit;
uniform bool enableNormalMap;

void main(){

      vec3 diffuse=texture(texture_Diffuse_0,fs_in.texCoord).rgb;
      vec3 spec=texture(texture_Specular_0,fs_in.texCoord).rgb;
      gWorldPos=fs_in.worldPos;
      vec3 bump=texture(texture_Normal_0,fs_in.texCoord).xyz;
      bump=bump*2.0-vec3(1.0);
       bump=normalize(vec3(dot(TBN1, bump), dot(TBN2, bump), dot(TBN3, bump)));
       if(!enableNormalMap||limit.z<0.5)
       bump=fs_in.normal;
       if( !gl_FrontFacing )
       gNomral=-bump;
       else
      gNomral=bump;

      gTangent=normalize(fs_in.tangent);
     

      gAlbedoSpec=vec4(diffuse,spec.x);
      vec2 nowPos=(nowScreenPos.xy/nowScreenPos.w)*0.5+0.5;
      vec2 prePos=(preScreenPos.xy/preScreenPos.w)*0.5+0.5;
      gVelocity=nowPos-prePos;
   
}