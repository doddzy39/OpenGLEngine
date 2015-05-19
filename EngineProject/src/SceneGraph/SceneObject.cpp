#include "SceneObject.h"
#include "SceneNode.h"
#include <iostream>

SceneObject::SceneObject()
{
	m_pOwningSceneNode = std::make_shared<SceneNode>();
	
	//Give the attached node full control of this object
	m_pOwningSceneNode->AttachUserData(this);
}

SceneObject::SceneObject(const SceneObject& a_rOther)
{
	m_pOwningSceneNode = a_rOther.m_pOwningSceneNode;
}

SceneObject::~SceneObject()
{
	std::cout << "destroying object" << std::endl;
}

void SceneObject::SetOwningSceneNode(std::shared_ptr<SceneNode> a_pOwner)
{
	m_pOwningSceneNode = a_pOwner;
}

SceneNode* SceneObject::GetOwningSceneNode()
{
	return m_pOwningSceneNode.get();
}

std::shared_ptr<SceneNode> SceneObject::getOwningSceneNodeShardPtr()
{
	return m_pOwningSceneNode;
}
