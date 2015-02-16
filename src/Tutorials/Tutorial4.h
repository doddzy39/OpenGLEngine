#ifndef TUTORIAL_4_H_
#define TUTORIAL_4_H_

#include "Engine/GLApplication.h"
#include <glm/glm.hpp>

class OBJMesh;
class Shader;

class Tutorial4 : public GLApplication
{
public:
	Tutorial4() : GLApplication("Tutorial 4 - Advanced Texturing and Lighting") {}
	~Tutorial4() {}

	virtual bool Startup();
	virtual void Shutdown();

	virtual bool Update(double dt);
	virtual void Render();

protected:
	OBJMesh*		m_pMeshToDraw;
	unsigned int	m_uiShader;

	glm::mat4		m_earthTransform;


	float			m_fTimeScale;
private:
};


#endif