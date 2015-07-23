#pragma once

#include <memory>

class SceneNode;

class SceneObject : public std::enable_shared_from_this<SceneObject>
{
	friend SceneNode;

public:
	SceneObject();
	SceneObject(const SceneObject& a_rOther);
	SceneObject(SceneObject&& a_rOther);
	~SceneObject();

	std::weak_ptr<SceneNode> GetSceneNode();
	std::shared_ptr<SceneNode> GetSharedSceneNode();
private:
	void SetSceneNode(std::shared_ptr<SceneNode> a_pOwner);

	std::shared_ptr<SceneNode> m_pOwningSceneNode;
};