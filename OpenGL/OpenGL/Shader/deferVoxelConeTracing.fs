#version 450 core
in vec2 uv;
uniform sampler2D screen_RT;
out vec4 FragColor;
in float depth;
uniform sampler2D gDepth;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler3D voxelMap_radiance;
uniform sampler3D voxelMap_normal;
uniform sampler2D noiseMap;
uniform sampler2D noiseMap1;
uniform sampler2D gTangent;
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
uniform float voxelWidthWorld;
uniform int voxelResolutionI;
uniform sampler2D shadowMap;
uniform sampler3D voxelMipMap[6];
uniform mat4 lightMat;
uniform vec3 cameraPos;
#define PI 3.14159274f
const float HALF_PI = 1.57079f;
uniform float deferInDirLight;
uniform float deferDirLight;
uniform vec3 worldMinPoint;
uniform vec3 worldMaxPoint;
uniform mat4 inverseProjectionView;
uniform float maxDistance;
uniform float stepLength;
uniform float Aperture;
uniform float occ_falloff;
uniform bool enableAmbientOcc;
uniform bool showAmbientOcc;
uniform float specCone;
uniform float Time;
const vec3 diffuseConeDirections[] =
{
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.5f, 0.866025f),
    vec3(0.823639f, 0.5f, 0.267617f),
    vec3(0.509037f, 0.5f, -0.7006629f),
    vec3(-0.50937f, 0.5f, -0.7006629f),
    vec3(-0.823639f, 0.5f, 0.267617f)
};

const float diffuseConeWeights[] =
{
    PI / 4.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
};

// const vec3 diffuseConeDirections[] =
// {
//   vec3(0.0f, 1.0f, 0.0f),
//     vec3(0.0f, 0.5f, 0.866025f),
//     vec3(0.0, 0.5f, -0.866025f),
//     vec3(0.866025f, 0.5f, 0.0f),
//     vec3(-0.866025f, 0.5f, 0.0f),
//     vec3(0.61237243569579, 0.5f, 0.61237243569579),
//     vec3(-0.61237243569579, 0.5f, 0.61237243569579),
//     vec3(0.61237243569579, 0.5f, -0.61237243569579),
//     vec3(-0.61237243569579, 0.5f, -0.61237243569579)
// };


// const float diffuseConeWeights[] =
// {
//         PI / 4.0f,
//     3.0f * PI / 32.0f,
//     3.0f * PI / 32.0f,
//     3.0f * PI / 32.0f,
//     3.0f * PI / 32.0f,
//     3.0f * PI / 32.0f,
//     3.0f * PI / 32.0f,
//     3.0f * PI / 32.0f,
//     3.0f * PI / 32.0f,
// };
// vec4 ConeTracing(vec3 pos,vec3 dir,vec3 worldPos,vec3 normal)
// {
//   vec3 weight=dir*dir;
//   int x_select=dir.x>0?1:0;
//   int y_select=dir.y>0?3:2;
//   int z_select=dir.z>0?5:4;

//  float dst=0;
//   vec4 result=vec4(0.0);
//   vec3 samplePos=pos;
//     float voxelSize=(2.0/float(voxelResolutionI));
//   float stepCount=2*voxelSize;
// float mipMaxLevel = log2(float(voxelResolutionI)) - 1.0f;
//   while(result.a<1.0&&samplePos.x>0&&samplePos.x<1&&samplePos.y>0&&samplePos.y<1&&samplePos.z>0&&samplePos.z<1)
//   {
//    //  stepCount=voxelSize* 0.5f+stepCount;
//       samplePos=pos+dir*stepCount;
//     // float mipmaplevel=log2(stepCount*float(voxelResolutionI));
//     // mipmaplevel = min(max(mipmaplevel - 1.0f, 0.0f),mipMaxLevel);
    
//         float diameter = 2.0f * 0.57735f* stepCount;
//         float mipmaplevel=log2(diameter*float(voxelResolutionI));
//         mipmaplevel = min(max(mipmaplevel - 1.0f, 0.0f),mipMaxLevel);
//     vec4 sampleColor=textureLod(voxelMipMap[x_select],samplePos,mipmaplevel)*weight.x
//     +textureLod(voxelMipMap[y_select],samplePos,mipmaplevel)*weight.y
//     +textureLod(voxelMipMap[z_select],samplePos,mipmaplevel)*weight.z;
//     // vec3 samplePos1=vec3(0.5,0.5,0.5)+samplePos;
//     // vec4 sampleColor1=textureLod(voxelMipMap[x_select],samplePos,mipmaplevel)*weight.x
//     // +textureLod(voxelMipMap[y_select],samplePos,mipmaplevel)*weight.y
//     // +textureLod(voxelMipMap[z_select],samplePos,mipmaplevel)*weight.z;
//     // sampleColor=sampleColor*0.5+sampleColor1*0.5;
//     //vec4 sampleColor=vec4(1.0);
//     // if(mipmaplevel < 1.0f)
//     // {
//     //     vec4 baseColor = imageLoad(voxelMap_radiance,ivec3(samplePos*voxelResolution));
//     //     sampleColor = mix(baseColor, sampleColor, clamp(mipmaplevel, 0.0f, 1.0f));
//     // }
   
//     result+=(1-result.a)*sampleColor;
//     //stepCount+=max(stepCount,voxelSize);
//   // stepCount+=voxelSize;
//     stepCount+=diameter*0.5;
//   }

//  // result=textureLod(voxelMipMap[x_select],pos,3.0);
//   return result;


// }
ivec3 worldToVoxel(vec3 worldPos){
return ivec3((worldPos-worldMinPoint)/voxelWidthWorld*float(voxelResolutionI));
}
vec4 ConeTracing(vec3 pos,vec3 dir,vec3 worldPos,vec3 normal,float aperture,bool enabelAO)
{
  vec3 weight=dir*dir;
  int x_select=dir.x>0?1:0;
  int y_select=dir.y>0?3:2;
  int z_select=dir.z>0?5:4;

  float dst=0;
  vec4 result=vec4(0.0);
  float occlusion=0;
  float voxelSize=(2.0/float(voxelResolutionI)*voxelWidthWorld);
  worldPos=worldPos+normal*voxelSize;
  vec3 samplePos=worldPos ;
  float stepCount=voxelSize;
  float mipMaxLevel = log2(float(voxelResolutionI)) - 1.0f;
  float count=0;
  ivec3 samplePosVoxel=ivec3(0);
  float falloff = 0.5f * occ_falloff / voxelWidthWorld;
  while(result.a<1.0&&samplePos.x>worldMinPoint.x&&samplePos.x<worldMaxPoint.x&&samplePos.y>worldMinPoint.y&&samplePos.y<worldMaxPoint.y&&samplePos.z>worldMinPoint.z&&samplePos.z<worldMaxPoint.z&&stepCount<=voxelWidthWorld*maxDistance)
  {
   //  stepCount=voxelSize* 0.5f+stepCount;
      samplePos=worldPos+dir*stepCount;
    // float mipmaplevel=log2(stepCount*float(voxelResolutionI));
    // mipmaplevel = min(max(mipmaplevel - 1.0f, 0.0f),mipMaxLevel);
     vec3 samplePosVoxel=worldToVoxel(samplePos)/float(voxelResolutionI);
     //float aperture=0.577355f;
       // float diameter = 2.0f *Aperture * stepCount;
       float diameter = 2.0f *aperture * stepCount;
        float mipmaplevel=log2(diameter/voxelSize);
        mipmaplevel = min(max(mipmaplevel - 1.0f, 0.0f),mipMaxLevel);
    vec4 sampleColor=textureLod(voxelMipMap[x_select],samplePosVoxel,mipmaplevel)*weight.x
    +textureLod(voxelMipMap[y_select],samplePosVoxel,mipmaplevel)*weight.y
    +textureLod(voxelMipMap[z_select],samplePosVoxel,mipmaplevel)*weight.z;
    
    //   if(mipmaplevel < 1.0f)
    // {
    //     vec4 baseColor = texture(voxelMap_radiance, samplePosVoxel);
    //     sampleColor = mix(baseColor, sampleColor, clamp(mipmaplevel, 0.0f, 1.0f));
    // }
    if(enabelAO)
    occlusion=occlusion+(1-occlusion)*sampleColor.a/(1+diameter*falloff);
    
    result+=(1-result.a)*sampleColor;
   
    //stepCount+=max(stepCount,voxelSize);
  // stepCount+=voxelSize;
    stepCount+=diameter*stepLength;
    count++;
  }

 // result=textureLod(voxelMipMap[x_select],pos,3.0);
  //return vec4(worldPos,1.0);
  if(!enabelAO)
  occlusion=0.0f;

  occlusion=min(occlusion,1.0f);
  return vec4(result.rgb,occlusion);

}




float CalcShadow(vec4 worldPos,vec3 normal){

    vec4 lightSpacePos=lightMat*worldPos;
    vec2 lightuv=(lightSpacePos/lightSpacePos.w).xy*0.5+0.5;
        float depthz=(lightSpacePos/lightSpacePos.w).z*0.5+0.5;
        float bias = max(0.05 * (1.0 - dot(-normal, dirLight.lightDir)), 0.0001);
        float shadow=depthz-bias<texture(shadowMap,lightuv).r?1.0:0.0;
        return shadow;

}

vec4 CalcIndirectLighting(vec3 pos,vec3 normal,vec4 albedo)
{

    vec3 updir=vec3(0,1,0);
    vec3 viewDir=normalize(pos-cameraPos);
    vec3 dir=normalize(reflect(viewDir,normal));
    vec4 result=vec4(0.0);
        float noise=texture(noiseMap,uv+10.0*vec2(Time)).x;
     float angle=texture(noiseMap1,uv+10.5*vec2(Time)).x;
   //  vec3  real_noise=vec3(noise*0.5,sqrt(0.75-noise),sqrt(1.0-0.25*noise*noise));
     vec3  real_noise=vec3(noise*cos(2*PI*angle),sqrt(1-noise*noise),noise*sin(2*PI*angle));
    // real_noise=vec3(real_noise)*2.0-vec3(1.0);
     vec3 tangent=normalize(texture(gTangent,uv)).xyz;
     vec3 biotangent=normalize(cross(tangent,normal));
     vec3 randomDir=real_noise.x*tangent*0.5+real_noise.y*normal*1.0+real_noise.z*biotangent*0.5;
      if(occ_falloff>850||(abs(tangent.x)<=1e-6&&abs(tangent.y)<=1e-6&&abs(tangent.z)<=1e-6))
      randomDir=vec3(real_noise);

     vec3 origin=worldToVoxel(pos);
     if(albedo.a>0.0)
     {
     float aperture = clamp(tan(HALF_PI * (1.0f - albedo.a)), 0.0174533f, PI);
 
 
    //  else
    //  randomDir=(real_noise.x*2.0-1.0)*tangent+real_noise.y*normal+(real_noise.z*2.0-1.0)*biotangent;
    result+=1.0*ConeTracing(origin/float(voxelResolutionI),dir,pos+specCone*randomDir,normal,aperture,false);
     }
    //cost  a lot
    // if(albedo.a>0.99f)
    // return vec4(result.rgb*vec3(0.8),1.0);

  
    if(abs(dot(updir,normal))==1)
      updir=vec3(0,0,1);
      vec3 x_axis=normalize(cross(updir,normal));
     vec3 z_axis=normalize(cross(x_axis,normal));

      // vec3 x_axis=normalize(updir - dot(normal,updir) * normal);
      // vec3 z_axis=cross(x_axis,normal);
      
      for(int i=0;i<6;i++)
      {
        vec3 coneDir=diffuseConeDirections[i].x*x_axis+normal+diffuseConeDirections[i].z*z_axis;
        coneDir=normalize(coneDir);
        result+=ConeTracing(origin/float(voxelResolutionI),coneDir,pos+specCone*randomDir,normal,Aperture,true)*diffuseConeWeights[i];


      }

      if(enableAmbientOcc)
    return vec4((result*albedo).rgb,clamp(1.0f - result.a , 0.0f, 1.0f) );
    else
      return vec4((result*albedo).rgb,1.0f );
    
}
vec4 brdf(vec3 pos,vec3 normal,vec4 albedo)
{
    vec3 diffuse=max(dot(-dirLight.lightDir,normal),0.0)*albedo.xyz;
    vec3 viewDir=normalize(cameraPos-pos.xyz);
    vec3 spec=vec3(pow(max(dot(normal,normalize(normalize(-dirLight.lightDir)+(viewDir))),0.0),100));
    return vec4(diffuse+spec,1.0);



}
vec4 CalcdirectLighting(vec3 pos,vec3 normal,vec4 albedo)
{
    
    return brdf(pos,normal,albedo)*CalcShadow(vec4(pos,1.0),normal);


}

vec3 PositionFromDepth()
{
    float z = texture(gDepth, uv).x * 2.0f - 1.0f;
    vec4 projected = vec4(uv * 2.0f - 1.0f, z, 1.0f);
    projected = inverseProjectionView * projected;
    return projected.xyz / projected.w;
}



void main(){
//FragColor=vec4(ourColor,1.0f);
 vec4 worldPos=texture(gPosition,uv);
 worldPos/=worldPos.w;
//vec4 worldPos=vec4(PositionFromDepth(),1.0);
vec3 normal=normalize(texture(gNormal,uv).xyz);
vec4 albedo=texture(gAlbedoSpec,uv);

    ivec3 voxelpos=worldToVoxel(worldPos.xyz);
    FragColor=albedo*dot(normal,-dirLight.lightDir)*10*CalcShadow(worldPos,normal);
    vec3 viewDir=normalize(worldPos.xyz-cameraPos);
    vec3 dir=reflect(viewDir,normal);
    vec4 indirectLighting=CalcIndirectLighting(worldPos.xyz,normal,albedo);
    vec4 directLighting=CalcdirectLighting(worldPos.xyz,normal,albedo);

FragColor=(directLighting*deferDirLight+indirectLighting*deferInDirLight)*indirectLighting.a;
if(showAmbientOcc)
FragColor=vec4(indirectLighting.a);

//FragColor=vec4(indirectLighting.a);
//FragColor=vec4(normal,1.0);
//FragColor=texture(voxelMap_radiance,worldToVoxel(worldPos.xyz)/float(voxelResolutionI));
//FragColor=textureLod(voxelMipMap[0],worldToVoxel(worldPos.xyz)/float(voxelResolutionI),0);
//FragColor=texelFetch(voxelMap_radiance,worldToVoxel(worldPos.xyz),0);
//FragColor=textureLod(voxelMipMap[0],worldToVoxel(worldPos.xyz)/float(voxelResolutionI),0)*100;
//FragColor=pow(texture(screen_RT, uv),vec4(1.0/2.2));
//FragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z)/gl_FragCoord.w;
//  float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//     FragColor = vec4(average, average, average, 1.0);
//FragColor=vec4(1.0,0.0,1.0,1.0);
}