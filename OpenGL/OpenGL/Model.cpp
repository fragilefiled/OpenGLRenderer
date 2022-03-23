#include"Model.h"

void Model::Draw(Shader &shader, vector<Texture> textures_RT,CubeMap* cubemap, vector<Texture3D> voxel_RT)
{
	for (int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader, textures_RT,cubemap,voxel_RT);
}

void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs| aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
    LoadInstance();
    
    
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
       
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::LoadInstance()
{



    if (drawinstance)
    {
        unsigned int InstanceBuffer;
        glGenBuffers(1, &InstanceBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, InstanceBuffer);
        glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
        
        for (int i = 0; i < meshes.size(); i++) {
            meshes[i].InstanceBuffer = InstanceBuffer;
            meshes[i].LoadInstance();
        }
    }


}

void Model::CreateMesh()
{
  //  int MeshSize = 250;
  //  //fftSize = (int)Mathf.Pow(2, FFTPow);
  // int* vertIndexs =  new int[(MeshSize - 1) * (MeshSize - 1) * 6];
  //glm::uvec3*  positions = new  glm::uvec3[MeshSize * MeshSize];
  //glm::uvec2*  uvs = new glm::uvec2[MeshSize * MeshSize];

  //  int inx = 0;
  //  for (int i = 0; i < MeshSize; i++)
  //  {
  //      for (int j = 0; j < MeshSize; j++)
  //      {
  //          int index = i * MeshSize + j;
  //          positions[index] = glm::vec3 ((j - MeshSize / 2.0f) * MeshLength / MeshSize, 0, (i - MeshSize / 2.0f) * MeshLength / MeshSize);
  //          uvs[index] = new Vector2(j / (MeshSize - 1.0f), i / (MeshSize - 1.0f));

  //          if (i != MeshSize - 1 && j != MeshSize - 1)
  //          {
  //              vertIndexs[inx++] = index;
  //              vertIndexs[inx++] = index + MeshSize;
  //              vertIndexs[inx++] = index + MeshSize + 1;

  //              vertIndexs[inx++] = index;
  //              vertIndexs[inx++] = index + MeshSize + 1;
  //              vertIndexs[inx++] = index + 1;
  //          }
  //      }
  //  }
  //  mesh.vertices = positions;
  //  mesh.SetIndices(vertIndexs, MeshTopology.Triangles, 0);
  //  mesh.uv = uvs;




}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
   
    vector< Mesh::Vertex> vertices;
    vector<Texture> textures;
    vector<unsigned int> indices;
    for (int i = 0; i < mesh->mNumVertices; i++)
    {

        Mesh::Vertex vertex;
        aiVector3D vertex_assimp = mesh->mVertices[i];
        aiVector3D normal_assimp = mesh->mNormals[i];
        aiVector3D tangent_assimp;
        aiVector3D biotangent_assimp;
        if (mesh->HasTangentsAndBitangents()) 
        {
            tangent_assimp = mesh->mTangents[i];
            biotangent_assimp = mesh->mBitangents[i];
        }
           
           
        glm::vec3 pos = glm::vec3(vertex_assimp.x, vertex_assimp.y, vertex_assimp.z);
        vertex.Position = pos;
        if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertex.Normal = glm::vec3(normal_assimp.x, normal_assimp.y, normal_assimp.z);
        vertex.Tangent = glm::vec3(tangent_assimp.x, tangent_assimp.y, tangent_assimp.z);
        vertex.BioTangent = glm::vec3(biotangent_assimp.x, biotangent_assimp.y, biotangent_assimp.z);
        vertices.push_back(vertex);
    }
    for (int i = 0; i < mesh->mNumFaces; i++) 
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);


    }
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, Texture::Diffuse);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, Texture::Specular);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        vector<Texture> normalMaps = loadMaterialTextures(material,
            aiTextureType_HEIGHT, Texture::Normal);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    }
   
    return Mesh(vertices,indices,textures,drawinstance,modelMatrices,instanceAmount);
    
}
vector<Texture>Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture::TextureType textureType)
{
    vector<Texture> textures;
    aiString str;
   
    
    // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
    
    bool skip = false;
    for (unsigned int j = 0; j < mat->GetTextureCount(type); j++)
    {
       // cout <<( mat->GetName()).C_Str()<<endl;
   
       
        
        mat->GetTexture(type, j, &str);
        for (unsigned int i = 0; i < textures_loaded.size(); i++) 
        {
          /*  if (std::strcmp((mat->GetName()).C_Str(), "chain") == 0)
                cout << str.C_Str()<<endl;*/

            if ( std::strcmp(textures_loaded[i].path.data(), (directory + '/' + str.C_Str()).c_str()) == 0)
            {

                textures.push_back(textures_loaded[i]);
                
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }

       
        if (!skip) 
        {

            cout << (directory + '/' + str.C_Str()).c_str() << endl;
            //(directory.append(str.C_Str())).c_str()
                mat->GetTexture(type, j, &str);
                Texture texture((directory+'/'+str.C_Str()).c_str(), textureType);
                texture.id = texture.Bind();
                texture.type = textureType;
                textures.push_back(texture);
                textures_loaded.push_back(texture);
                
              
            
        }
    }
    return textures;
}











Model::~Model()
{
    
    for (int i = 0; i < meshes.size(); i++) {
        glDeleteVertexArrays(1, &meshes[i].VAO);
        glDeleteBuffers(1, &meshes[i].VBO);
        glDeleteBuffers(1, &meshes[i].EBO);
        

    }
}


