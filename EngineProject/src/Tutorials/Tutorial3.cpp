#include "Tutorial3.h"

#include "Gizmos.h"

#include "Cameras/FlyCamera.h"
#include "DefaultMeshes/ObjMesh.h"
#include "Engine/ShaderHandler.h"
#include "Engine/Material.h"


bool Tutorial3::Startup()
{
	FlyCamera* pCamera = new FlyCamera();

	pCamera->SetInputWindow(m_pWindow);

	pCamera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f);
	pCamera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_pCamera = pCamera;

	m_pMeshToDraw = new OBJMesh("sphere/sphere.obj");
	m_uiShader = ShaderHandler::Get()->LoadShader("OBJLight", "Shaders/OBJShader.vert", "Shaders/DirectionalLightShader.frag");
	m_pMeshToDraw->GetMaterial()->SetShader(m_uiShader);

	return true;
}

void Tutorial3::Shutdown()
{
	delete m_pMeshToDraw;
}

bool Tutorial3::Update(double dt)
{
	m_pCamera->Update(dt);
	return true;
}

void Tutorial3::Render()
{
	DisplayGrid(20);

	m_pMeshToDraw->Render(m_pCamera, glm::mat4(1));
}
