#include "Tutorial2B.h"
#include "Gizmos.h"

#include "Cameras/FlyCamera.h"

#include "DefaultMeshes/ObjMesh.h"


bool Tutorial2B::Startup()
{
	FlyCamera* pCamera = new FlyCamera();

	pCamera->SetInputWindow(m_pWindow);

	pCamera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f);
	pCamera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_pCamera = pCamera;
	
	meshToDraw = new OBJMesh("sphere/sphere.obj");

	return true;
}

void Tutorial2B::Shutdown()
{
	delete meshToDraw;
}

bool Tutorial2B::Update(double dt)
{
	m_pCamera->Update(dt);
	return true;
}

void Tutorial2B::Render()
{
	DisplayGrid(20);

	meshToDraw->Render(m_pCamera, glm::mat4(1));
}
