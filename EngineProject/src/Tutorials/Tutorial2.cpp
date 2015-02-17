#include "Tutorial2.h"
#include "Gizmos.h"

#include "Cameras/FlyCamera.h"
#include "DefaultMeshes/GridMesh.h"
#include "DefaultMeshes/CubeMesh.h"

bool Tutorial2::Startup()
{
	FlyCamera* pCamera = new FlyCamera();

	pCamera->SetInputWindow(m_pWindow);

	pCamera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f);
	pCamera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	
	m_pCamera = pCamera;

	m_pGridMesh = new GridMesh(100, 100);
	m_pCubeMesh = new CubeMesh(25.0f);
	return true;
}

void Tutorial2::Shutdown()
{
}

bool Tutorial2::Update(double dt)
{
	m_pCamera->Update(dt);
	return true;
}

void Tutorial2::Render()
{
	//DisplayGrid(20);
	m_pGridMesh->Render(m_pCamera, glm::mat4(1));
	m_pCubeMesh->Render(m_pCamera, glm::mat4(1));
}
