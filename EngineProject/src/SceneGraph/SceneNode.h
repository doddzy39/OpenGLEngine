#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>

#include "SceneObject.h"

class SceneNode : public std::enable_shared_from_this<SceneNode>
{
public:
	SceneNode();
	~SceneNode();

	//No copy constructor - specifically have to 'clone' the object
	SceneNode(const SceneNode& a_rSceneNode) = delete;
	SceneNode& operator=(const SceneNode& a_rSceneNode) = delete;

	std::shared_ptr<SceneNode> Clone();

	std::shared_ptr<SceneNode> GetParent() const { return m_pParentNode; }
	
	void AttachChild(std::shared_ptr<SceneNode> a_pChildObject);
	void AttachChild(SceneObject* a_pChildObject);
	
	void DetachChild(unsigned int a_uiChildID);
	void DetachChild(std::shared_ptr<SceneNode> a_pChildObject);
	void DetachChild(std::string a_strName);

	unsigned int GetNumberOfChildren() const { return m_vChildNodes.size(); }
	bool		 HasChildren() const { return !(m_vChildNodes.empty());  }
	
	std::shared_ptr<SceneNode> GetChildAt(int a_uiChildID);
	std::shared_ptr<SceneNode> GetChildByName(std::string a_strName);
	
	void		 AttachUserData(SceneObject* a_pUserData) { m_pUserData.reset(a_pUserData); }
	SceneObject* GetAttachedUserData() const { return m_pUserData.get(); }
	SceneObject* ReleaseUserData() { return m_pUserData.release(); }

	const glm::mat4& GetGlobalTransform() const { return m_globalTransform;  }
	//glm::mat4&		 GetGlobalTransform() { return m_globalTransform;  }

	const glm::mat4& GetLocalTransform() const { return m_localTransform; }
	//glm::mat4&		 GetLocalTransform() { return m_localTransform; }

	void SetLocalTransform(const glm::mat4& a_localTransform);
	void SetGlobalTransform(const glm::mat4& a_globalTransform);

	std::string GetName() { return m_strNodeName;  }
	void SetName(std::string a_strName) { m_strNodeName = a_strName; }


	const static std::shared_ptr<SceneNode> Invalid;

private:
	void UpdateChildGlobalTransforms();
	
	std::string m_strNodeName;
	
	glm::mat4 m_localTransform;
	glm::mat4 m_globalTransform;

	std::shared_ptr<SceneNode> m_pParentNode;
	std::vector<std::shared_ptr<SceneNode>> m_vChildNodes;

	std::unique_ptr<SceneObject> m_pUserData;
};