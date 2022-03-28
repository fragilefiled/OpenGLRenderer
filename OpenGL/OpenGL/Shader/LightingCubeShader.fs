#version 330 core
out vec4 FragColor;
uniform vec3 baseColor;
uniform vec3 cameraPos;

in VS_OUT
{
 vec3 worldPos;
  vec3 normal;
 vec2 texCoord;
}fs_in;
uniform sampler2D texture_Diffuse_0;
uniform sampler2D texture_Normal_0;
uniform sampler2D texture_Specular_0;
uniform sampler2D _depthTexture;
uniform sampler3D voxemMap;
in vec4 modelPos;

struct DirLight
{
    vec3 lightDir;
    vec3 lightPos;
    vec4 lightColor;
    
};
struct SpotLight 
{
    vec3 lightDir;
    vec3 lightPos;
    vec4 lightColor;
    float cutOff;
    
};
struct PointLight
{
    vec3 lightDir;
    vec3 lightPos;
    vec4 lightColor;
    vec3 lightproperty;
    
};

uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[4];
#define NR_POINT_LIGHTS 4
vec3 calcDirLight(DirLight dirLight,vec3 normal,vec3 spec_color,vec3 diffuse_color,vec3 worldPos)
{
    vec3 finalColor=vec3(0);
    vec3 viewDir=normalize(cameraPos-worldPos);
    vec3 diffuse=max(dot(normal,(-dirLight.lightDir)),0.0)*dirLight.lightColor.rgb*diffuse_color;
    vec3 spec=pow(max(dot(normal,normalize(normalize(-dirLight.lightDir)+(viewDir))),0.0),100.0)*dirLight.lightColor.rgb*diffuse_color;
    return spec+diffuse;
} 
vec3 calcPointLight(PointLight pointLight,vec3 normal,vec3 spec_color,vec3 diffuse_color,vec3 worldPos)
{
        float distance=length(pointLight.lightPos-worldPos);
    float attenuation = 1.0 / (pointLight.lightproperty.x + pointLight.lightproperty.y * distance + 
            pointLight.lightproperty.z * (distance * distance));
    vec3 finalColor=vec3(0);
    vec3 pointLightDir=pointLight.lightPos-worldPos;
    vec3 viewDir=normalize(cameraPos-worldPos);
    vec3 diffuse=max(dot(normal,normalize(pointLightDir)),0.0)*pointLight.lightColor.rgb*diffuse_color;
    vec3 spec=pow(max(dot(normal,normalize(normalize(pointLightDir)+(viewDir))),0.0),100.0)*pointLight.lightColor.rgb*spec_color;
   
    return (diffuse+spec)*attenuation;
} 
vec3 calcSpotLight(SpotLight spotLight,vec3 normal,vec3 spec_color,vec3 diffuse_color,vec3 worldPos)
{
 vec3 finalColor=vec3(0);
    vec3 lightviewDir=normalize(worldPos-spotLight.lightPos);
    vec3 pointLightDir=spotLight.lightPos-worldPos;
    vec3 viewDir=normalize(worldPos-cameraPos);
    vec3 diffuse=vec3(0);
    vec3 spec=vec3(0);
    if(max(dot(lightviewDir,spotLight.lightDir),0.0)-spotLight.cutOff>0.0)
    {
        diffuse= max(dot(normal,-spotLight.lightDir),0.0)*diffuse_color*spotLight.lightColor.rgb;;
        spec=pow(max(dot(normal,-normalize(normalize(spotLight.lightDir)+(viewDir))),0.0),100.0)*spotLight.lightColor.rgb*spec_color;
    }

    return diffuse+spec;
}  

void main(){
//FragColor=vec4(ourColor,1.0f);
    // vec3 viewDir=normalize(cameraPos-worldPos);
    // float distance=length(lightPos-worldPos);
    // float attenuation = 1.0 / (lightproperty.x + lightproperty.y * distance + 
    //             lightproperty.z * (distance * distance));
    // vec3 ambient = lightColor.rgb*0.1;

   

      vec3 diffuse_helmet=texture(texture_Diffuse_0,fs_in.texCoord).rgb;
      vec3 spec_helmet=texture(texture_Specular_0,fs_in.texCoord).rgb;
   
    // vec3 pointlightDir=normalize( dirLight.lightPos-worldPos);
    // vec3 diffuse=max(dot(normal,(pointlightDir)),0)*dirLight.lightColor.rgb;
    // vec3 spec=pow(max(dot(normal,normalize(normalize(lightDir)+(viewDir))),0),100)*lightColor.rgb*spec_wood;
    
    //  vec3 lightviewDir=normalize(worldPos-spotLight.lightPos);
    // diffuse= step(spotLight.cutOff,max(dot(lightviewDir,-lightDir),0))*diffuse_wood;;
        // FragColor=vec4(spec_wood,0.0);
        FragColor=vec4(0.0);
        FragColor+=vec4(calcDirLight(dirLight,fs_in.normal,spec_helmet,diffuse_helmet,fs_in.worldPos),1.0f);
        FragColor+=vec4(calcSpotLight(spotLight,fs_in.normal,spec_helmet,diffuse_helmet,fs_in.worldPos),1.0f);
        // FragColor+=vec4(calcPointLight(pointLights[0],normal,spec_wood,diffuse_wood),1.0f);
        // FragColor+=vec4(calcPointLight(pointLights[1],normal,spec_wood,diffuse_wood),1.0f);
        FragColor+=vec4(calcPointLight(pointLights[0],fs_in.normal,spec_helmet,diffuse_helmet,fs_in.worldPos),1.0f);
        FragColor+=vec4(calcPointLight(pointLights[1],fs_in.normal,spec_helmet,diffuse_helmet,fs_in.worldPos),1.0f);
       // FragColor=pow(FragColor,vec4(1.0/2.2));

        vec2 lightuv=(modelPos/modelPos.w).xy*0.5+0.5;
        float depthz=(modelPos/modelPos.w).z*0.5+0.5;
        float bias = max(0.05 * (1.0 - dot(-fs_in.normal, dirLight.lightDir)), 0.005);
        float shadow=depthz-bias<texture(_depthTexture,lightuv).r?1.0:0.0;
        FragColor=vec4(fs_in.texCoord,0.0,0.0);
      //  FragColor=vec4(diffuse_helmet,0.0);
       // FragColor=vec4(pow(FragColor.xyz*shadow,vec3(1.0/2.2)),1.0);
       // FragColor=vec4(1.0,0.0,0.0,0.0);
     //   FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
         //FragColor= vec4(1,1,1,1);
        // FragColor=vec4(diffuse_helmet,1.0)*2.0;
        //  if(texture(texture_Normal_0,texCoord).a==1)
        //     FragColor=vec4(0.5);
}