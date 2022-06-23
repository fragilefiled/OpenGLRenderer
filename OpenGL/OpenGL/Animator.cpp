#include"Animator.h"

void Animator::UpdateAnimation(float deltaTime)
{
    m_DeltaTime = deltaTime;
    if (m_CurrentAnimation)
    {
        
        m_CurrentTime +=deltaTime * m_CurrentAnimation->GetTicksPerSecond();
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
        //cout << deltaTime;
        CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
    }

}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* bone = m_CurrentAnimation->FindBone(nodeName);
    if (bone)
    {
        bone->Update(m_CurrentTime);
        nodeTransform = bone->GetLocalTransform();
    }
    glm::mat4 globalTransformation = parentTransform * nodeTransform;
    auto  boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        m_FinalBoneMatrices[index] = globalTransformation * offset;

    }
    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);

}
