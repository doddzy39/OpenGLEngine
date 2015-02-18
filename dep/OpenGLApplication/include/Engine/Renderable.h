#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class BaseCamera;
class Material;

class Renderable
{
public:
	struct Vertex {
		glm::vec4 position;
		glm::vec4 colour;
	};

	Renderable();
	~Renderable();

	virtual void Create() = 0;

	//For now, pass in the render location - we'll make this a full node
	//system later
	virtual void Render(BaseCamera* pCamera, glm::mat4 transform);

	bool PrepareToRender(glm::mat4 transform);

	const unsigned int& GetVBO() const { return m_VBO; };
	const unsigned int& GetIBO() const { return m_IBO; };
	const unsigned int& GetVAO() const { return m_VAO; };

	void SetMaterial(Material* a_pMaterial);
	void SetMaterial(std::string& a_strMaterial);
	Material* GetMaterial();

protected:
	unsigned int m_uiNumberOfIndices;
	unsigned int m_uiNumberOfVerts;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;


private:	

	Material* m_pMaterial;
	std::string m_strMaterialName;
};
#endif