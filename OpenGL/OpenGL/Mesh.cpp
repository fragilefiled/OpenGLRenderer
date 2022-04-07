#include"Mesh.h"
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures,bool drawinstance,vector< glm::mat4> modelMatrices,int instanceAmount):vertices(vertices),indices(indices),textures(textures),drawinstance(drawinstance),modelMatrices(modelMatrices)
{
	if (instanceAmount == 0)
		this->instanceAmount = modelMatrices.size();
	AABB.pMin = glm::vec3(1e10);
	AABB.pMax = glm::vec3(-1e10);
	GenerateBoundingBox();
	setupMesh();
	
}



Mesh::~Mesh()
{
	   
}

void Mesh::Draw(Shader& shader,vector<Texture>&textures_RT, CubeMap* cubemap, vector<Texture3D> &voxel_RT)
{ 
	int diffuseIndex = 0;
	int specIndex = 0;
	int normalIndex = 0;
	int heightIndex = 0;
	shader.use();
	for (int i = 0; i < textures.size(); i++)
	{
		
		glActiveTexture(GL_TEXTURE0 + i);
		
		switch (textures[i].type)
		{
		case Texture::Diffuse:
			shader.setInt(textures[i].GetNameInShader(diffuseIndex++), i);
			break;
		case Texture::Normal:
			shader.setInt(textures[i].GetNameInShader(normalIndex++),i);
			break;
		case Texture::Specular:
			shader.setInt(textures[i].GetNameInShader(specIndex++), i);
			break;
		case Texture::Other:
			shader.setInt(textures[i].GetNameInShader(heightIndex++), i);
			break;
		default:
			break;


		}
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		//cout << textures[i].id;
	}
	int loadTextureCount = textures.size();
	for (int i = 0; i < textures_RT.size(); i++) 
	{

		glActiveTexture(GL_TEXTURE0 + i+loadTextureCount);
		shader.setInt(textures_RT[i].nameInShader, i + loadTextureCount);
		glBindTexture(GL_TEXTURE_2D, textures_RT[i].id);


	}
	if (cubemap != nullptr) {
		glActiveTexture(GL_TEXTURE0 + loadTextureCount + textures_RT.size() + 1);
		shader.setInt(cubemap->nameInShader, loadTextureCount + textures_RT.size() + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id);
	}
	loadTextureCount = loadTextureCount + textures_RT.size() + 2;
	for (int i = 0; i < voxel_RT.size(); i++)
	{

		glActiveTexture(GL_TEXTURE0 + i + loadTextureCount);
		shader.setInt(voxel_RT[i].nameInShader, i + loadTextureCount);
		glBindTexture(GL_TEXTURE_3D, voxel_RT[i].id);
		if(voxel_RT[i].enableTextureImage)
		glBindImageTexture(i, voxel_RT[i].id, 0, GL_FALSE, 0, GL_READ_WRITE, voxel_RT[i].format);

	}
	
	// draw mesh
	glBindVertexArray(VAO);
	if(!drawinstance)
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	else
	{
		glDrawElementsInstanced(
			GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, 1000
		);
	}
   
    glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	//glActiveTexture(GL_TEXTURE0);
	
}

void Mesh::setupMesh()
{
	
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BioTangent));
	// ids
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

	// weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
	
	auto temp = std::vector<Vertex>(0);
	vertices.swap(temp);

    glBindVertexArray(0);
}
bool Mesh::InFrustum(const glm::vec4 Plane[])
{
	glm::vec3 center = (AABB_Transform.pMax + AABB_Transform.pMin)/glm::vec3(2.0);
	glm::vec3 extent = (AABB_Transform.pMax - AABB_Transform.pMin) / glm::vec3(2.0);
	
	for (int i = 0; i < 6; i++) {
		glm::vec3 normal = Plane[i];
		float radius = glm::dot(extent, abs(normal));
		float distance= glm::dot(center, normal)+Plane[i].w;

		
		if (distance + radius <= 0.0f)//-distance>=radius
		{
			return false;
		}
	}
	return true;
}
void Mesh::LoadInstance(){
	if (drawinstance)
	{
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, InstanceBuffer);


			GLsizei vec4Size = sizeof(glm::vec4);
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
			glEnableVertexAttribArray(8);
			glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
			glEnableVertexAttribArray(9);
			glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
			glEnableVertexAttribArray(10);
			glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);
			glVertexAttribDivisor(9, 1);
			glVertexAttribDivisor(10, 1);
			glBindVertexArray(0);



	}


}

void Mesh::GenerateBoundingBox()
{
	
	for (int i = 0; i < vertices.size(); i++) 
	{

		AABB.pMax.x = max(vertices[i].Position.x, AABB.pMax.x);
		AABB.pMax.y = max(vertices[i].Position.y, AABB.pMax.y);
		AABB.pMax.z = max(vertices[i].Position.z, AABB.pMax.z);
		AABB.pMin.x = min(vertices[i].Position.x, AABB.pMin.x);
		AABB.pMin.y = min(vertices[i].Position.y, AABB.pMin.y);
		AABB.pMin.z = min(vertices[i].Position.z, AABB.pMin.z);

	}
}

void Mesh::TransformBoundingBox(glm::mat3x3 rotation, glm::vec3 translate, glm::vec3 scale,bool enableRotate)
{
	float  a, b;


	int    i, j;

	/*Copy box A into a min array and a max array for easy reference.*/
	
	glm::vec3 Amax = scale*AABB.pMax;
	glm::vec3 Amin = scale * AABB.pMin;
	AABB_Transform.pMax = translate;
	AABB_Transform.pMin = translate;
	if (enableRotate) {
		for (i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				a = rotation[j][i] * Amin[j];
				b = rotation[j][i] * Amax[j];
				if (a < b) {
					AABB_Transform.pMin[i] += a;
					AABB_Transform.pMax[i] += b;
				}
				else {
					AABB_Transform.pMin[i] += b;
					AABB_Transform.pMax[i] += a;
				}



			}
		}
	}
	else {
		AABB_Transform.pMax += Amax;
		AABB_Transform.pMin += Amin;

	}



}
