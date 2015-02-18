#ifndef TUTORIAL_2_H_
#define TUTORIAL_2_H_

#include "Engine/GLApplication.h"

class GridMesh;
class CubeMesh;

class Tutorial2 : public GLApplication
{
public:
	Tutorial2() : GLApplication("Tutorial 2") {}
	~Tutorial2() {}

	virtual bool Startup();
	virtual void Shutdown();

	virtual bool Update(double dt);
	virtual void Render();

protected:
	GridMesh* m_pGridMesh;
	CubeMesh* m_pCubeMesh;
private:
};


#endif