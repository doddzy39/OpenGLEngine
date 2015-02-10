#ifndef GL_APPLICATION_H_
#define GL_APPLICATION_H_

#include "Engine/gl_core_4_4.h"
#include <GLFW/glfw3.h>

#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class BaseCamera;

class GLApplication
{
public:
	GLApplication(std::string appName, unsigned int uiWidth = 1280, unsigned int uiHeight = 720);
	~GLApplication();

	virtual bool Startup() = 0;
	virtual void Shutdown() = 0;

	virtual bool Update(double dt) = 0;
	virtual void Render() = 0;

	void Run();

protected:

	//Helper functions that can be used by children
	void DisplayGrid(int uiSize);

	unsigned int GetScreenWidth() const { return m_uiScreenWidth; }
	unsigned int GetScreenHeight() const { return m_uiScreenHeight; }


	std::string m_strAppName;
	GLFWwindow* m_pWindow;

	BaseCamera* m_pCamera;
private:
	//You shall not call this!
	GLApplication() {};

	//Initialization helper functions
	bool InitializeOpenGL();

	unsigned int m_uiScreenWidth;
	unsigned int m_uiScreenHeight;

	double m_fTotalRunTime;
	double m_fDeltaTime;
};

#endif