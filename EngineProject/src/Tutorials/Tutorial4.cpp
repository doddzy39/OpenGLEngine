#include "Tutorial4.h"

#include "Gizmos.h"

#include "Cameras/FlyCamera.h"
#include "DefaultMeshes/ObjMesh.h"
#include "DefaultMeshes/SkyboxMesh.h"
#include "Engine/ShaderHandler.h"
#include "Engine/Material.h"


bool Tutorial4::Startup()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	FlyCamera* pCamera = new FlyCamera();
	pCamera->SetRotationSpeed(1.0f);

	pCamera->SetInputWindow(m_pWindow);

	pCamera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f);
	pCamera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_pCamera = pCamera;

	m_pMeshToDraw = new OBJMesh("sphere_hires/sphere.obj");
	m_uiShader = ShaderHandler::Get()->LoadShader("EarthShader", "Shaders/EarthShader_Normal.vert", "Shaders/EarthShader_Normal.frag");
	m_pMeshToDraw->GetMaterial()->SetShader(m_uiShader);

	m_pSkybox = new SkyboxMesh("SpaceSkybox", "png");

	glClearColor(0.03f, 0.03f, 0.05f, 1.0f);


	glm::mat4 scale = glm::scale(glm::vec3(3.0f, 3.0f, 3.0f));
	glm::mat4 rot = glm::rotate(glm::radians(23.5f), glm::vec3(1, 0, 0));
	m_earthTransform = m_earthTransform * scale * rot;
	
	m_fTimeScale = 1.0f;

	return true;
}

void Tutorial4::Shutdown()
{
	delete m_pMeshToDraw;
	delete m_pSkybox;
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

	m_pSkybox->Render(m_pCamera, glm::mat4(1));
}
