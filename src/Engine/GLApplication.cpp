#include "GLApplication.h"

#include "Gizmos.h"
#include "BaseCamera.h"
#include <GLFW\glfw3.h>

#include "ShaderHandler.h"
#include "MaterialHandler.h"

GLApplication::GLApplication(std::string appName, unsigned int uiWidth, unsigned int uiHeight)
{
	m_strAppName = appName;
	m_pCamera = nullptr;

	m_uiScreenWidth = uiWidth;
	m_uiScreenHeight = uiHeight;

	m_fTotalRunTime = glfwGetTime();
	m_fDeltaTime = 0.0f;
}

GLApplication::~GLApplication()
{

}

bool GLApplication::InitializeOpenGL()
{
	if (glfwInit() == false)
		return false;

	m_pWindow = glfwCreateWindow(m_uiScreenWidth, m_uiScreenHeight, m_strAppName.c_str(), nullptr, nullptr);

	if (m_pWindow == nullptr)
	{
		glfwTerminate();
		return false;
	}

	
	glfwMakeContextCurrent(m_pWindow);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
			glfwDestroyWindow(m_pWindow);
			glfwTerminate();
			return false;
	}

	Gizmos::create();

	glClearColor( 0.25f, 0.25f, 0.25f, 1 );
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	return true;
}


void GLApplication::Run()
{
	InitializeOpenGL();
	
	//Initialize Shader and Material Handler
	ShaderHandler::Initialize();
	MaterialHandler::Initialize();

	Startup();


	assert(m_pCamera != NULL && "Camera not initialized");
	assert(m_pCamera->GetPerspectiveSet() == true && "Camera Perspective not set");
	
	bool bRunning = true;
	while (!glfwWindowShouldClose(m_pWindow) && bRunning)
	{
		double currTime = glfwGetTime();
		m_fDeltaTime = currTime - m_fTotalRunTime;
		m_fTotalRunTime = currTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Gizmos::clear();

		bRunning = Update(m_fDeltaTime);


		ShaderHandler::Get()->SetEyePosition(glm::vec4(m_pCamera->GetPosition(), 1));
		ShaderHandler::Get()->SetProjectionMatrix(m_pCamera->GetProjection());
		ShaderHandler::Get()->SetViewMatrix(m_pCamera->GetView());
		Render();

		Gizmos::draw(m_pCamera->GetProjectionView());

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}

	Shutdown();

	Gizmos::destroy();

	MaterialHandler::Shutdown();
	ShaderHandler::Shutdown();
	
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();

}

void GLApplication::DisplayGrid(int uiSize)
{
	assert(uiSize > 0 && "Needs to be a positive size");

	Gizmos::addTransform(glm::mat4(1));
	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	int uiHalfSize = uiSize / 2;
	for (int i = 0; i < uiSize + 1; ++i)
	{
		Gizmos::addLine(glm::vec3(-uiHalfSize + i, 0, uiHalfSize),
			glm::vec3(-uiHalfSize + i, 0, -uiHalfSize),
			i == uiHalfSize ? white : black);

		Gizmos::addLine(glm::vec3(uiHalfSize, 0, -uiHalfSize + i),
			glm::vec3(-uiHalfSize, 0, -uiHalfSize + i),
			i == uiHalfSize ? white : black);
	}
}
