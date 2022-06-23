#pragma once
#include"Model.h"
#include"Bone.h"

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};
class Animation {

public:
		Animation(std::string path,Model*model);
        inline float GetTicksPerSecond() { return m_TicksPerSecond; }

        inline float GetDuration() { return m_Duration; }

        inline const AssimpNodeData& GetRootNode() { return m_RootNode; }

        inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
        {
            return m_BoneInfoMap;
        }
        Bone* FindBone(string& name);
private:
    float m_Duration;//持续时间
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    void ReadAnimationTreeData(AssimpNodeData & dest,const aiNode* src);
    void ReadMissingBones(const aiAnimation* animation,Model&model);


};