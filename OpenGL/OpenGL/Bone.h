#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/quaternion.hpp>
#include "Camera.h"
#include <GLFW\glfw3.h>
#include"Mymath.h"
struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};
class Bone
{
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);
	~Bone();
    glm::mat4 GetLocalTransform() { return m_Transform; };
    std::string GetBoneName() { return m_Name; };
    int GetBoneID() { return m_ID; }
    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    int GetScaleIndex(float animationTime);
    void Update(float animationTime);
private:
    std::vector<KeyPosition>m_Positions;
    std::vector<KeyRotation>m_Rotations;
    std::vector<KeyScale>m_Scales;
    int m_NumPositions;
    int m_NumRotations;
    int m_NumScalings;
    glm::mat4 m_Transform;
    std::string m_Name;
    int m_ID;

    float GetInterpolateFactor(float firstTimeStamp,float secondTimeStamp,float animationTime);
    glm::mat4 InterpolatePosition(float animationTime);
    glm::mat4 InterpolateRotation(float animationTime);
    glm::mat4 InterpolateScale(float animationTime);

};

