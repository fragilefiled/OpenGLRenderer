#pragma once
#include<iostream>
#include<vector>
#include "Texture.h"
#include "Texture3D.h"
#include "ShaderCompile.h"
#include "CubeMap.h"
using namespace std;
#define MAX_BONE_INFLUENCE 4
class Mesh
{
public:
	struct  Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 BioTangent;
		//bone indexes which will influence this vertex
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];
	};
	vector<Vertex> vertices;
	vector<Texture> textures;
	vector<unsigned int> indices;
	vector<glm::mat4> modelMatrices;
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, bool drawinstance=false, vector<glm::mat4> modelMatrices = vector< glm::mat4>(), int instanceAmount=0);
	~Mesh();
	void Draw(Shader& shader, vector<Texture> textures_RT=vector<Texture>(),CubeMap*cubemap=nullptr, vector<Texture3D> voxel_RT=vector<Texture3D>());
	void setupMesh();

	void LoadInstance();

	
	unsigned int InstanceBuffer;
	unsigned int  VAO, VBO;
	unsigned int  EBO;
private:
	bool drawinstance=false;

	
	int instanceAmount;

	
};

