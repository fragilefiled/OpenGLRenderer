#version 450 core
#extension GL_ARB_shader_image_load_store : require
out vec4 FragColor;

in GS_OUT{
 vec4 AABB;
 vec2 limitZ;
 vec4 projectPos;
 vec2 texCoord;
 vec4 worldTexCoord;
 vec3 normal;
  vec4 lightProjPos;
}fs_in;
layout(r32ui, binding = 0)uniform volatile coherent uimage3D voxelMap_diffuse;
layout(r32ui, binding = 1)uniform volatile coherent uimage3D voxelMap_normal;
//layout(r8ui, binding = 2) uniform uimage3D voxelMap_staticMark;
uniform sampler2D _depthTexture;
uniform sampler2D texture_Diffuse_0;
uniform sampler2D texture_Normal_0;
uniform sampler2D texture_Specular_0;
uniform float voxelResolution;
uniform int VoxelNormalBlend;
uniform int staticFlag;

vec4 convRGBA8ToVec4(uint val)
{
    return vec4(float((val & 0x000000FF)), 
    float((val & 0x0000FF00) >> 8U), 
    float((val & 0x00FF0000) >> 16U), 
    float((val & 0xFF000000) >> 24U));
}

uint convVec4ToRGBA8(vec4 val)
{
    return (uint(val.w) & 0x000000FF) << 24U | 
    (uint(val.z) & 0x000000FF) << 16U | 
    (uint(val.y) & 0x000000FF) << 8U | 
    (uint(val.x) & 0x000000FF);
}
vec3 EncodeNormal(vec3 normal)
{
    return normal * 0.5f + vec3(0.5f);
}

vec3 DecodeNormal(vec3 normal)
{
    return normal * 2.0f - vec3(1.0f);
}
void imageAtomicRGBA8Avg(layout(r32ui) volatile coherent uimage3D grid, ivec3 coords, vec4 value)
{
    value.rgb *= 255.0;                 // optimize following calculations
    uint newVal = convVec4ToRGBA8(value);
    uint prevStoredVal = 0;
    uint curStoredVal;
    uint numIterations = 0;

    while((curStoredVal = imageAtomicCompSwap(grid, coords, prevStoredVal, newVal)) 
            != prevStoredVal
            && numIterations < 255)
    {
        prevStoredVal = curStoredVal;
        vec4 rval = convRGBA8ToVec4(curStoredVal);
        rval.rgb = (rval.rgb * rval.a); // Denormalize
        vec4 curValF = rval + value;    // Add
        curValF.rgb /= (curValF.a);       // Renormalize
        newVal = convVec4ToRGBA8(curValF);

        ++numIterations;
    }
}

void imageAtomicRGBA8Avg_normal(layout(r32ui) volatile coherent uimage3D grid, ivec3 coords, vec4 value)
{
    value.rgb *= 255.0;                 // optimize following calculations
    uint newVal = convVec4ToRGBA8(value);
    uint prevStoredVal = 0;
    uint curStoredVal;
    uint numIterations = 0;

    
    while((curStoredVal = imageAtomicCompSwap(grid, coords, prevStoredVal, newVal)) 
            != prevStoredVal
            && numIterations < 255)
    {
        prevStoredVal = curStoredVal;
        vec4 rval = convRGBA8ToVec4(curStoredVal);
        rval.rgb = (rval.rgb * rval.a); // Denormalize
        vec3 normal_r=DecodeNormal(rval.rgb/255.0);
        vec3 normal=DecodeNormal(value.rgb/255.0);
        vec4 curValF = vec4(normal_r+normal,value.w+rval.w);    // Add
        curValF.rgb=EncodeNormal(curValF.rgb)*255.0;
        curValF.rgb /= curValF.a;       // Renormalize
        newVal = convVec4ToRGBA8(curValF);

        ++numIterations;
    }
}


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

void main(){
//FragColor=vec4(ourColor,1.0f);

vec3 projectxy=fs_in.projectPos.xyz;
vec2 limitZ=fs_in.limitZ;
vec4 AABB=fs_in.AABB;
vec3 normal=normalize(fs_in.normal);
//  if( !gl_FrontFacing )
//        normal=-normal;
//        else
//       normal=normal;
normal=EncodeNormal(normal);
ivec3 worldTexCoord=ivec3( (fs_in.worldTexCoord.x),(fs_in.worldTexCoord.y),(fs_in.worldTexCoord.z));
// uint staticFlagRead=imageLoad(voxelMap_staticMark,worldTexCoord).x;
// if(staticFlagRead==1)
//     return ;
//if enable conservative rasterization 
// if(projectxy.x<=AABB.x||projectxy.y<=AABB.y||projectxy.x>=AABB.z||projectxy.y>=AABB.w||projectxy.z<=limitZ.x||projectxy.z>=limitZ.y)
//     discard;
   
vec4 albedo=texture(texture_Diffuse_0,fs_in.texCoord);


imageAtomicRGBA8Avg(voxelMap_diffuse,worldTexCoord,vec4(albedo.xyz,1.0));
// imageAtomicRGBA8Avg(voxelMap_diffuse,worldTexCoord,vec4(vec3(fs_in.texCoord,0.0),1.0));
imageAtomicRGBA8Avg(voxelMap_normal,worldTexCoord,vec4(normal,1.0));
//imageStore(voxelMap_staticMark,worldTexCoord,ivec4(staticFlag,staticFlag,staticFlag,staticFlag));


    FragColor=albedo;
}