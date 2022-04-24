#version 450 core
out vec4 FragColor;
uniform vec3 baseColor;
uniform vec3 cameraPos;
uniform sampler2D _normalMap;
uniform sampler2D _bubbleMap;
in vec3 aPos_uv;
in VS_OUT
{
 vec3 worldPos;
  vec3 normal;
 vec2 texCoord;
}fs_in;
in vec4 modelPos;
uniform sampler2D _depthTexture;
uniform sampler2D _displacementMap;
uniform sampler2D _waveMap;
uniform sampler2D _reflectionMap;
uniform sampler2D _tangentMap;
uniform sampler2D _biotangentMap;
uniform sampler2D _bubbleDiffuseMap;
uniform sampler2D _bubbleDiffuseMask;
uniform samplerCube skybox;
uniform float Scale_subsurface;
uniform float _Glossiness;
uniform float _Metallic;
uniform float _bubbleMapPower;


struct DirLight
{
    vec3 lightDir;
    vec3 lightPos;
    vec4 lightColor;
    
};
#define PI 3.14159274f
uniform mat4 model_inverse_t;
uniform DirLight dirLight;
int mipmaplevel(vec2 uv){
  vec2 texsize=vec2(1.0/512,1.0/512);
  vec2 dx=dFdx(fs_in.texCoord*texsize.x );
  vec2 dy=dFdy(fs_in.texCoord*texsize.y );   
 float rho = max(sqrt(dot(dx, dx)), sqrt(dot(dy, dy)));
    float lambda = log2(rho);
    int lod = max(int(lambda + 0.5), 0);
  return lod;
}
float sqr(float x){
	return x*x; 
}
float pow5(float x){
  float a=x*x;
  
return a*a*x;
}
float GGXNormalDistribution(float roughness, float NdotH)
{
    float roughnessSqr = roughness*roughness;
    float NdotHSqr = NdotH*NdotH;
    float TanNdotHSqr = (1-NdotHSqr)/NdotHSqr;
 
  //  return sqr(1.0/(NdotHSqr * (roughnessSqr + TanNdotHSqr)));
    return (1.0/3.1415926535) * sqr(roughness/(NdotHSqr * (roughnessSqr + TanNdotHSqr)));
}
float WalterEtAlGeometricShadowingFunction (float NdotL, float NdotV, float alpha){
    float alphaSqr = alpha*alpha;
    float NdotLSqr = NdotL*NdotL;
    float NdotVSqr = NdotV*NdotV;

    float SmithL = 2.0/(1.0 + sqrt(1.0 + alphaSqr * (1.0-NdotLSqr)/(NdotLSqr)));
    float SmithV = 2.0/(1.0 + sqrt(1.0+ alphaSqr * (1.0-NdotVSqr)/(NdotVSqr)));


	float Gs =  (SmithL * SmithV);
	return Gs;
}
float smithG_GGX(float NdotV, float alphaG)
{
    float a = alphaG * alphaG;
    float b = NdotV * NdotV;
    return 1 / (NdotV + sqrt(a + b - a * b));
}

float MixFunction(float i, float j, float x) {
	 return  j * x + i * (1.0 - x);
}

float SchlickFresnel(float i){
    float x = clamp(1.0-i, 0.0, 1.0);
    float x2 = x*x;
    return x2*x2*x;
}

//normal incidence reflection calculation
float F0 (float NdotL, float NdotV, float LdotH, float roughness){
    float FresnelLight = SchlickFresnel(NdotL); 
    float FresnelView = SchlickFresnel(NdotV);
    float FresnelDiffuse90 = 0.5 + 2.0 * LdotH*LdotH * roughness;
    return  MixFunction(1, FresnelDiffuse90, FresnelLight) * MixFunction(1, FresnelDiffuse90, FresnelView);
}
void main(){



        vec3 tangent=texture(_tangentMap,fs_in.texCoord).xyz;
        vec3 biotangent=texture(_biotangentMap,fs_in.texCoord).xyz;


        float roughness = 1- (_Glossiness * _Glossiness);   // 1 - smoothness*smoothness
        roughness = roughness * roughness;

        // vec2 lightuv=(modelPos/modelPos.w).xy*0.5+0.5;
        // float depthz=(modelPos/modelPos.w).z*0.5+0.5;
        // float bias = max(0.05 , 0.005);
        
        // float shadow=depthz-0.002<texture(_depthTexture,lightuv).r?1.0:0.0;
        // if(depthz>1.0)
        // shadow=1.0;
      
        
       // FragColor=vec4(shadow,0,0,0);
        vec4 ambient=vec4(0.1,0.1,0.1,0.1)*0.5;
        vec4 normal= model_inverse_t*vec4(((texture(_normalMap,fs_in.texCoord).xyz*2.0-vec3(1.0,1.0,1.0))),0.0);
        vec3 real_normal=normalize(normal.xyz);
        // vec3 t0=vec3(tangent.x,biotangent.x,real_normal.x);
        // vec3 t1=vec3(tangent.y,biotangent.y,real_normal.y);
        // vec3 t2=vec3(tangent.z,biotangent.z,real_normal.z);
        // vec3 bubbleNormal=texture(_bubbleNormalMap,fs_in.texCoord).xyz;
        // bubbleNormal=vec3(dot(bubbleNormal,t0),dot(bubbleNormal,t1),dot(bubbleNormal,t2));
        vec3 viewDir=normalize(cameraPos-fs_in.worldPos);
        // float facing = max((dot(viewDir, real_normal)),0.0);    
       // vec4 _OceanColorDeep=pow(vec4(0,132,255,0.0)/255.0*0.5,vec4(2.2));//vec4(0.0738,0.1869,0.2673,0.0);
        vec4 _OceanColorShallow=vec4(32,178,170,0.0)/255.0;
       // vec4 _OceanColorReal=pow(vec4(11,45,100,0.0)/255.0*2.55,vec4(2.2));   
        vec4 _OceanColorReal=vec4(0.0077,0.172,1,0.0);     
         //vec4 oceanColor = mix(_OceanColorShallow, _OceanColorDeep, facing);
         // oceanColor=_OceanColorDeep;
       // vec4 diffuse=max(-dot(real_normal,normalize(dirLight.lightDir)),0.01)*_OceanColorDeep;//pow(vec4(78,119,140,0)/255.0,2.2);
      // vec4 spec=pow(max(dot((-dirLight.lightDir+normalize(cameraPos-fs_in.worldPos))/2,real_normal),0.0),100)*vec4(0.5,0.5,0.5,0);
        //FragColor=vec4(pow(dot((-dirLight.lightDir+normalize(cameraPos-fs_in.worldPos))/2,real_normal),1000));
       // FragColor=spec+diffuse+ambient;
        float bubble=texture(_bubbleDiffuseMap,fs_in.texCoord).x;
        float bubbleMask=texture(_bubbleDiffuseMask,fs_in.texCoord).x;
        vec4 bubblecolor=texture(_bubbleMap,fs_in.texCoord);
       bubbleMask=clamp(bubbleMask-bubblecolor.x,0,1);
        //  if(_bubbleMapPower>0.1)
        //  {
        //  // bubblecolor=vec4(bubbleMask)*bubble*dot(bubbleNormal,-normalize(dirLight.lightDir))+bubblecolor;
      
        //     bubblecolor=vec4(bubbleMask)*bubble+bubblecolor;
        //  }
         bubblecolor=vec4(bubbleMask)*bubble+bubblecolor;
        // if(_bubbleMapPower>0.5)
        // bubblecolor=texture(_bubbleMap,fs_in.texCoord)*vec4(bubble)*bubblecolor+bubblecolor;
        vec4 reflectionMap=texture(_reflectionMap,((aPos_uv+1.0)/2.0).xy);
       float facing = pow5(max((dot(viewDir, -dirLight.lightDir)),0.0));
        vec4 sss=(vec4(0.3,0.3,0.3,0.3)+vec4(0.3,0.3,0.3,0.3)*facing)*_OceanColorShallow;
        sss*=(1.0-abs(viewDir.y)*abs(viewDir.y))*texture(_displacementMap,fs_in.texCoord).y*10;
       vec3 dir=normalize(dirLight.lightDir);
      // float Fresnel=0.5+(1-0.5)*pow5(1-dot(viewDir,real_normal));

      vec3 reflectdir=reflect(-viewDir,real_normal);
  //    reflectdir=normalize(reflectdir+normalize(fs_in.worldPos));
        vec4 reflectcol=texture(skybox,reflectdir );
      
      FragColor=sss*Scale_subsurface+bubblecolor+FragColor+reflectcol*0.1;
 
        vec3 H=normalize(normalize(-dirLight.lightDir)+normalize(cameraPos-fs_in.worldPos));
       float NdotH=max(dot(real_normal,H),0.000001);
       float NdotL=max(-dot(real_normal,normalize(dirLight.lightDir)),0.000001);
       float LdotH=max(dot(-dirLight.lightDir,H),0.000000);
       float VdotH=max(dot(normalize(cameraPos-fs_in.worldPos),H),0.000001);
       float NdotV=max(dot(real_normal,normalize(cameraPos-fs_in.worldPos)),0.000001);
       float HdotX=max(dot(H,tangent),0.000001);
       float HdotY=max(dot(H,biotangent),0.000001);
       float NDF=GGXNormalDistribution(roughness,NdotH);
       float Gs=WalterEtAlGeometricShadowingFunction(NdotL,NdotV,roughness);
      // float Gs=smithG_GGX(NdotV,roughness);
       float F=F0(NdotL, NdotV,  LdotH, roughness);
       float brdf=NDF*Gs*F/(4*NdotL*NdotV);
      // FragColor=(vec4(brdf)*(vec4(0.1)+reflectcol))+sss*Scale_subsurface+bubblecolor+diffuse+ambient+reflectcol*0.1;
     // FragColor=diffuse+spec+ambient+sss*Scale_subsurface+bubblecolor+FragColor+0.1*reflectcol;
       //FragColor=sss*Scale_subsurface+bubblecolor+diffuse+spec+ambient+reflectcol*0.1;
       float FD90=0.5+2*sqr(LdotH)*roughness;
       //float FD90=0.5+2*sqr(VdotH)*roughness;
      vec4 diffuse_ds=_OceanColorReal/PI*(1+(FD90-1)*pow5(1-NdotL))*(1+(FD90-1)*pow5(1-NdotV));
    
    //   float bubblethrelod=clamp(0,1,bubblecolor.x);
    //   vec4 bubblerealColor=mix(texture(_bubbleDiffuseMap,fs_in.texCoord)*0.01,vec4(1.0),bubblethrelod);
    //   vec4 bubblediffusecolor=bubblerealColor*bubblecolor*max(dot(bubbleNormal,-dirLight.lightDir),0.0);
    //  vec4  bubblespeccolor=bubblecolor*pow(max(dot((-dirLight.lightDir+normalize(cameraPos-fs_in.worldPos))/2,bubbleNormal),0.0),10.0);

      FragColor=((vec4(brdf)*(vec4(0.5)+reflectcol*0.1))+sss*Scale_subsurface+bubblecolor+diffuse_ds+ambient+reflectcol*0.1);
     //FragColor=texture(_bubbleMap,fs_in.texCoord);
    // FragColor=vec4(texture(_bubbleDiffuseMask,fs_in.texCoord).x);
      //FragColor=texture(_bubbleDiffuseMap,fs_in.texCoord);
       // FragColor=sss*Scale_subsurface+bubblecolor+diffuse+spec+ambient+reflectcol*0.1;
      //FragColor=diffuse*5;
    // FragColor=vec4(real_normal,0.0);
     
    
}