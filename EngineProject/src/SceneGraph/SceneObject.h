#pragma once

#include <memory>

class SceneNode;

class SceneObject
{
public:
	SceneObject();
	SceneObject(const SceneObject& a_rOther);
	~SceneObject();

	SceneNode* GetOwningSceneNode();
	std::shared_ptr<SceneNode> getOwningSceneNodeShardPtr();
private:
	void SetOwningSceneNode(std::shared_ptr<SceneNode> a_pOwner);

	std::shared_ptr<SceneNode> m_pOwningSceneNode;
};