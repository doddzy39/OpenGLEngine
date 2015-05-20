#include "SceneNode.h"
#include "LowLevelUtilityClasses/pow2assert.h"
#include <iostream>

const std::shared_ptr<SceneNode> SceneNode::Invalid = std::make_shared<SceneNode>();

SceneNode::SceneNode()
{

}

SceneNode::~SceneNode()
{
	std::cout << "Scene node destroyed" << std::endl;
	m_pUserData = nullptr;
	m_vChildNodes.clear();
}

std::shared_ptr<SceneNode> SceneNode::Clone()
{
	return std::make_shared<SceneNode>();
}


void SceneNode::AttachChild(std::shared_ptr<SceneNode> a_pChildObject)
{
	m_vChildNodes.push_back(a_pChildObject);
	a_pChildObject->m_pParentNode = shared_from_this();
}

void SceneNode::AttachChild(SceneObject* a_pChildObject)
{
	m_vChildNodes.push_back(a_pChildObject->getOwningSceneNodeShardPtr());
}

void SceneNode::DetachChild(unsigned int a_uiChildID)
{
	//POW2_ASSERT_MSG(a_uiChildID < m_vChildNodes.size(), "Passed ID %s, Num children: %s", a_uiChildID, m_vChildNodes.size());
	auto it = m_vChildNodes.begin() + a_uiChildID;
	m_vChildNodes.erase(it);
}

void SceneNode::DetachChild(std::shared_ptr<SceneNode> a_pChildObject)
{
	auto it = std::find(m_vChildNodes.begin(), m_vChildNodes.end(), a_pChildObject);
	if (it != m_vChildNodes.end())
		m_vChildNodes.erase(it);
}

void SceneNode::DetachChild(std::string a_strName)
{
	for (auto& child : m_vChildNodes)
	{
		if (child->GetName() == a_strName)
		{
			DetachChild(child);
			return;
		}
	}
}

std::shared_ptr<SceneNode> SceneNode::GetChildAt(int a_uiChildID)
{
	auto it = m_vChildNodes.begin() + a_uiChildID;
	return *it;
}

std::shared_ptr<SceneNode> SceneNode::GetChildByName(std::string a_strName)
{
	for (auto& child : m_vChildNodes)
	{
		if (child->GetName() == a_strName)
		{
			return child;
		}
	}

	return SceneNode::Invalid;
}

void SceneNode::SetLocalTransform(const glm::mat4& a_localTransform)
{
	m_localTransform = a_localTransform;
	UpdateChildGlobalTransforms();
}

void SceneNode::SetGlobalTransform(const glm::mat4& a_globalTransform)
{
	auto parentNode = m_pParentNode.lock();

	auto parentTransform = (parentNode != nullptr) ?
		parentNode->GetGlobalTransform() : glm::mat4(1);

	m_globalTransform = a_globalTransform;
	m_localTransform = m_globalTransform * glm::inverse(parentTransform);
	UpdateChildGlobalTransforms();
}

void SceneNode::UpdateChildGlobalTransforms()
{

	auto parentNode = m_pParentNode.lock();

	auto parentTransform = (parentNode != nullptr) ?
		parentNode->GetGlobalTransform() : glm::mat4(1);

	m_globalTransform = parentTransform * m_localTransform;
	for (auto& child : m_vChildNodes)
	{
		child->UpdateChildGlobalTransforms();
	}
}
