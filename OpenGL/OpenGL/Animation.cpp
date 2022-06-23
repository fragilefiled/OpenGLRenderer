#include"Animation.h"
Animation::Animation(std::string path, Model* model)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    auto animation = scene->mAnimations[0];
    m_Duration = animation->mDuration;
    m_TicksPerSecond = animation->mTicksPerSecond;
    ReadAnimationTreeData(m_RootNode, scene->mRootNode);
    ReadMissingBones(animation, *model);
}

Bone* Animation::FindBone(string& name)
{
    auto iter = std::find_if(m_Bones.begin(), m_Bones.end(), [name](Bone& bone) {return bone.GetBoneName() == name; });
    if (iter == m_Bones.end())
        return nullptr;
    else
        return &(*iter);

}

void Animation::ReadAnimationTreeData(AssimpNodeData& dest, const aiNode* src)
{
    dest.name = src->mName.data;
    dest.childrenCount = src->mNumChildren;
    dest.transformation = ConvertMatrixToGLMFormat(src->mTransformation);
    for (int i = 0; i < dest.childrenCount; i++) {
        AssimpNodeData newData;
        ReadAnimationTreeData(newData, src->mChildren[i]);
        dest.children.push_back(newData);

    }

}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
{
    int size = animation->mNumChannels;
    auto& boneInfoMap = model.GetBoneInfoMap();
    int& boneCount = model.GetBoneCount();
    for (int i = 0; i < size; i++) {
        auto channel = animation->mChannels[i];
        std::string bonename = channel->mNodeName.data;
        if (boneInfoMap.find(bonename) == boneInfoMap.end()) {
            boneInfoMap[bonename].id = boneCount;
            boneCount++;
        }
        m_Bones.push_back(Bone(bonename, boneInfoMap[bonename].id, channel));
    }
    m_BoneInfoMap = boneInfoMap;
}
