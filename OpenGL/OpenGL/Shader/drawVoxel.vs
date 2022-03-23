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
    
	ivec3 pos=ivec3(gl_VertexID % voxelResolutionI,
		(gl_VertexID / voxelResolutionI) % voxelResolutionI,
		gl_VertexID / (voxelResolutionI * voxelResolutionI));

	gl_Position =vec4(pos,1.0);

	vs_out.albedo=imageLoad(voxelMap_radiance, pos);;


}