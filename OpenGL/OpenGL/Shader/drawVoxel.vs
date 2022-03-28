# version 430 core



out VS_OUT{
 vec4 albedo;

}vs_out;
layout(rgba8, binding = 0)uniform  readonly image3D voxelMap_diffuse;
layout(rgba8, binding = 1)uniform readonly image3D voxelMap_normal;
layout(rgba8, binding = 2)uniform image3D voxelMap_radiance;
uniform int voxelResolutionI;
uniform float voxelResolution;

void main()
{
    
	vec3 pos=ivec3(gl_VertexID % voxelResolutionI,
		(gl_VertexID / voxelResolutionI) % voxelResolutionI,
		gl_VertexID / (voxelResolutionI * voxelResolutionI));

	gl_Position =vec4(pos,1.0);

	vs_out.albedo=imageLoad(voxelMap_radiance, ivec3(pos));;
	if(vs_out.albedo.x+vs_out.albedo.y+vs_out.albedo.z>0.0001)//可能是msaa导致的w插值过后为0,但可以通过x,y,z来让w不为0,说实话不太明白
	vs_out.albedo=vec4(vs_out.albedo.xyz,vs_out.albedo.w);

}