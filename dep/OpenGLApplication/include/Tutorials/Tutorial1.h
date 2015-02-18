#ifndef TUTORIAL_1_H_
#define TUTORIAL_1_H_

#include "Engine/GLApplication.h"

class Tutorial1 : public GLApplication
{
public:
	Tutorial1() : GLApplication("Tutorial 1") {}
	~Tutorial1() {}

	virtual bool Startup();
	virtual void Shutdown();

	virtual bool Update(double dt);
	virtual void Render();

protected:
private:
};


#endif