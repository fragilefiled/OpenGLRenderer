#pragma once
#include"Animation.h"
class Animator 
{
public:
	Animator(Animation*animation){
        m_CurrentTime = 0.0f;
        m_CurrentAnimation = animation;
        m_FinalBoneMatrices.reserve(100);
        for (int i = 0; i < 100; i++)
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}
    void UpdateAnimation(float deltaTime);

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }
    void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
    void Animator::PlayAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime = 0.0f;
    }
private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;
};