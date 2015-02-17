#ifndef TUTORIAL_1_H_
#define TUTORIAL_1_H_

#include "Engine/GLApplication.h"

class OBJMesh;

class Tutorial2B : public GLApplication
{
public:
	Tutorial2B() : GLApplication("Tutorial 2B - FBX") {}
	~Tutorial2B() {}

	virtual bool Startup();
	virtual void Shutdown();

	virtual bool Update(double dt);
	virtual void Render();

protected:
	OBJMesh* meshToDraw;
private:
};


#endif