#include "Tutorial4.h"

#include "Gizmos.h"

#include "Cameras/FlyCamera.h"
#include "DefaultMeshes/ObjMesh.h"
#include "Engine/ShaderHandler.h"
#include "Engine/Material.h"


bool Tutorial4::Startup()
{
	FlyCamera* pCamera = new FlyCamera();

	pCamera->SetInputWindow(m_pWindow);

	pCamera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f);
	pCamera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_pCamera = pCamera;

	m_pMeshToDraw = new OBJMesh("sphere/sphere.obj");
	m_uiShader = ShaderHandler::Get()->LoadShader("EarthShader", "Shaders/EarthShader_Normal.vert", "Shaders/EarthShader_Normal.frag");
	m_pMeshToDraw->GetMaterial()->SetShader(m_uiShader);

	glClearColor(0.03f, 0.03f, 0.05f, 1.0f);

	glm::mat4 rot = glm::rotate(glm::radians(23.5f), glm::vec3(1, 0, 0));
	m_earthTransform = m_earthTransform * rot;
	
	m_fTimeScale = 1.0f;

	return true;
}

void Tutorial4::Shutdown()
{
	delete m_pMeshToDraw;
}

bool Tutorial4::Update(double dt)
{
	m_pCamera->Update(dt);


	float fRotAmountPerSecond = (glm::pi<float>() * 2) * (1 / 60.0f);
	glm::mat4 rot = glm::rotate(fRotAmountPerSecond * (float)dt, glm::vec3(0, 1, 0));

	m_earthTransform = m_earthTransform * rot;

	return true;
}

void Tutorial4::Render()
{
	//DisplayGrid(20);


	m_pMeshToDraw->Render(m_pCamera, m_earthTransform);
}
