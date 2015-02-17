#ifndef TUTORIAL_3_H_
#define TUTORIAL_3_H_

#include "Engine/GLApplication.h"

class OBJMesh;
class Shader;

class Tutorial3 : public GLApplication
{
public:
	Tutorial3() : GLApplication("Tutorial 3 - Lighting") {}
	~Tutorial3() {}

	virtual bool Startup();
	virtual void Shutdown();

	virtual bool Update(double dt);
	virtual void Render();

protected:
	OBJMesh* m_pMeshToDraw;
	unsigned int m_uiShader;


private:
};


#endif