# version 460 core
layout(points) in;
// outputs voxels as cubes
layout(triangle_strip, max_vertices = 24) out;


in VS_OUT{
 vec4 albedo;

}gs_in[];
uniform mat4 model;
//uniform mat4 view;

//uniform mat4 projection;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform float voxelResolution;
uniform vec3 worldMinPoint;
uniform float voxelWidthWorld;
out vec4 voxelColor;
out vec4 worldPos;

vec3 VoxelToWorld(vec3 pos)
{
	vec3 result = pos;
	result = result*voxelWidthWorld/(voxelResolution);

	return result + worldMinPoint;
}
// bool VoxelInFrustum(vec3 center, vec3 extent)
// {
// 	vec4 plane;

// 	for(int i = 0; i < 6; i++)
// 	{
// 		plane = frustumPlanes[i];
// 		float d = dot(extent, abs(plane.xyz));
// 		float r = dot(center, plane.xyz) + plane.w;

// 		if(d + r > 0.0f == false)
// 		{
// 			return false;
// 		}
// 	}

// 	return true;
// }
void main()
{

	const vec4 cubeVertices[8] = vec4[8] 
	(
		vec4( 0.5f,  0.5f,  0.5f, 0.0f),
		vec4( 0.5f,  0.5f, -0.5f, 0.0f),
		vec4( 0.5f, -0.5f,  0.5f, 0.0f),
		vec4( 0.5f, -0.5f, -0.5f, 0.0f),
		vec4(-0.5f,  0.5f,  0.5f, 0.0f),
		vec4(-0.5f,  0.5f, -0.5f, 0.0f),
		vec4(-0.5f, -0.5f,  0.5f, 0.0f),
		vec4(-0.5f, -0.5f, -0.5f, 0.0f)
	);

	const int cubeIndices[24]  = int[24] 
	(
		0, 2, 1, 3, // right
		6, 4, 7, 5, // left
		5, 4, 1, 0, // up
		6, 7, 2, 3, // down
		4, 6, 0, 2, // front
		1, 3, 5, 7  // back
	);

    
	vec3 center = VoxelToWorld(gl_in[0].gl_Position.xyz);
	vec3 extent = vec3(voxelWidthWorld/(voxelResolution));
	if(gs_in[0].albedo.a==0.0)
	return;
	vec4 projectedVertices[8];
	vec4 worldVertices[8];
	for(int i = 0; i < 8; ++i)
	{
		vec4 vertex = gl_in[0].gl_Position + cubeVertices[i];
		projectedVertices[i] = projection*view*model*vertex;
		worldVertices[i]=model*vertex;
	}

	for(int face = 0; face < 6; ++face)
	{
		for(int vertex = 0; vertex < 4; ++vertex)
		{
			gl_Position = projectedVertices[cubeIndices[face * 4 + vertex]];
			
			// multply per color channel, 0 = delete channel, 1 = see channel
			// on alpha zero if the colors channels are positive, alpha will be passed as one
			// with alpha enabled and color channels > 1,  the albedo alpha will be passed.
			voxelColor = gs_in[0].albedo;
			worldPos=worldVertices[cubeIndices[face * 4 + vertex]];
			EmitVertex();
		}

		EndPrimitive();
	}

}