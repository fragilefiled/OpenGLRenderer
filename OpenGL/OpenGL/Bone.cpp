#include"Bone.h"

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel): m_Name(name),m_ID(ID),m_Transform(1.0F)
{
	m_NumPositions = channel->mNumPositionKeys;
	m_NumRotations = channel->mNumRotationKeys;
	m_NumScalings = channel->mNumScalingKeys;
	if (&name == nullptr)
		std::cout << name<<std::endl;
	for (int positionIdx = 0; positionIdx < m_NumPositions; positionIdx++) 
	{
		aiVector3D aiPosition = channel->mPositionKeys[positionIdx].mValue;
		float timeStamp = channel->mPositionKeys[positionIdx].mTime;
		KeyPosition keyPositionData;
		keyPositionData.position = GetGLMVec(aiPosition);
		keyPositionData.timeStamp = timeStamp;
		m_Positions.push_back(keyPositionData);

	}
	for (int rotationIdx = 0; rotationIdx < m_NumPositions; rotationIdx++)
	{
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIdx].mValue;
		float timeStamp = channel->mRotationKeys[rotationIdx].mTime;
		KeyRotation keyRotationData;
		keyRotationData.orientation = GetGLMQuat(aiOrientation);
		keyRotationData.timeStamp = timeStamp;
		m_Rotations.push_back(keyRotationData);

	}
	for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
	{
		aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
		float timeStamp = channel->mScalingKeys[keyIndex].mTime;
		KeyScale data;
		data.scale = GetGLMVec(scale);
		data.timeStamp = timeStamp;
		m_Scales.push_back(data);
	}

}

Bone::~Bone()
{
}

int Bone::GetPositionIndex(float animationTime)
{
	for (int index = 0; index < m_NumPositions - 1; index++) {
		if (m_Positions[index + 1].timeStamp > animationTime)
			return index;
	}
	std::cout << "position index Error";
	return -1;
}

int Bone::GetRotationIndex(float animationTime)
{
	for (int index = 0; index < m_NumRotations - 1; index++) {
		if (m_Rotations[index + 1].timeStamp > animationTime)
			return index;
	}
	std::cout << "position index Error";
	return -1;
}

int Bone::GetScaleIndex(float animationTime)
{
	for (int index = 0; index < m_NumScalings - 1; index++) {
		if (m_Scales[index + 1].timeStamp > animationTime)
			return index;
	}
	std::cout << "position index Error";
	return -1;
}

void Bone::Update(float animationTime)
{
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 translate = InterpolatePosition(animationTime);
	glm::mat4 scale = InterpolateScale(animationTime);
	m_Transform = translate * rotation * scale;
}

float Bone::GetInterpolateFactor(float firstTimeStamp, float secondTimeStamp, float animationTime)
{
	return (animationTime - firstTimeStamp) / (secondTimeStamp - firstTimeStamp);
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	int index0 = GetPositionIndex(animationTime);
	int index1 = index0 + 1;
	auto keyPosition0 = m_Positions[index0];
	auto keyPosition1 = m_Positions[index1];
	float lerpFactor = GetInterpolateFactor(keyPosition0.timeStamp, keyPosition1.timeStamp, animationTime);
	glm::vec3 lerpPos = glm::mix(keyPosition0.position, keyPosition1.position, lerpFactor);
	return glm::translate(glm::mat4(1.0f), lerpPos);

}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	int index0 = GetPositionIndex(animationTime);
	int index1 = index0 + 1;
	KeyRotation keyRotation0 = m_Rotations[index0];
	KeyRotation keyRotation1 = m_Rotations[index1];
	float lerpFactor = GetInterpolateFactor(keyRotation0.timeStamp, keyRotation1.timeStamp, animationTime);
	glm::quat lerpOrientation = glm::slerp(keyRotation0.orientation, keyRotation1.orientation, lerpFactor);
	glm::quat finalOrientation = glm::normalize(lerpOrientation);

	return glm::toMat4(finalOrientation);
}

glm::mat4 Bone::InterpolateScale(float animationTime)
{
	int index0 = GetPositionIndex(animationTime);
	int index1 = index0 + 1;
	KeyScale keyScale0 = m_Scales[index0];
	KeyScale keyScale1 = m_Scales[index1];
	float lerpFactor = GetInterpolateFactor(keyScale0.timeStamp, keyScale1.timeStamp, animationTime);
	glm::vec3 lerpScale = glm::mix(keyScale0.scale, keyScale1.scale, lerpFactor);
	return glm::scale(glm::mat4(1.0f),lerpScale);
}
