#include "Tutorial1.h"
#include "Gizmos.h"

#include "Cameras/FlyCamera.h"

bool Tutorial1::Startup()
{
	FlyCamera* pCamera = new FlyCamera();

	pCamera->SetInputWindow(m_pWindow);

	pCamera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f);
	pCamera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	
	m_pCamera = pCamera;

	return true;
}

void Tutorial1::Shutdown()
{
}

bool Tutorial1::Update(double dt)
{
	m_pCamera->Update(dt);
	return true;
}

void Tutorial1::Render()
{
	DisplayGrid(20);
	
}
