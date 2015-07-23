#include "SceneObject.h"
#include "SceneNode.h"
#include <iostream>

SceneObject::SceneObject()
{
	m_pOwningSceneNode = std::make_shared<SceneNode>();
	
	//Give the attached node ownership over us - just as we own it
	m_pOwningSceneNode->AttachUserData(this);
}

SceneObject::SceneObject(const SceneObject& a_rOther)
{
	m_pOwningSceneNode = a_rOther.m_pOwningSceneNode;
}

SceneObject::SceneObject(SceneObject&& a_rOther)
{
	m_pOwningSceneNode = a_rOther.m_pOwningSceneNode;
	a_rOther.m_pOwningSceneNode = nullptr;
}

SceneObject::~SceneObject()
{
	m_pOwningSceneNode = nullptr;
	std::cout << "destroying object" << std::endl;
}

void SceneObject::SetSceneNode(std::shared_ptr<SceneNode> a_pOwner)
{
	m_pOwningSceneNode = a_pOwner;
}

std::weak_ptr<SceneNode> SceneObject::GetSceneNode()
{
	return m_pOwningSceneNode;
}

std::shared_ptr<SceneNode> SceneObject::GetSharedSceneNode()
{
	return m_pOwningSceneNode;
}
