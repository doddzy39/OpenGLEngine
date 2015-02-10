#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class BaseCamera;

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
	void Render(BaseCamera* pCamera, glm::mat4 transform);
protected:
	static unsigned int CreateStandardShader();
	
	//static unsigned int CreateErrorShader();

	unsigned int m_uiNumberOfIndices;
	unsigned int m_uiNumberOfVerts;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_uiShaderProgram;
private:
	void SetupShaderLocations();


	//Other 'material' information until we set up a full material class
	int m_iModelMatrixShaderLocation;
	int m_iViewMatrixShaderLocation;
	int m_iProjectionMatrixShaderLocation;

	static int sm_iStandardShaderProgram;
};
#endif