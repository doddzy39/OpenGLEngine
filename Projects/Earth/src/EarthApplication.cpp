#include "EarthApplication.h"

#include "Gizmos.h"

#include "Cameras/FlyCamera.h"
#include "DefaultMeshes/ObjMesh.h"
#include "DefaultMeshes/SkyboxMesh.h"
#include "Engine/ShaderHandler.h"
#include "Engine/Material.h"

#include "SceneGraph/SceneNode.h"

#include <memory.h>

#include "DefaultMeshes/CubeMesh.h"


bool EarthApplication::Startup()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FlyCamera* pCamera = new FlyCamera();
	pCamera->SetRotationSpeed(1.0f);

	pCamera->SetInputWindow(m_pWindow);

	pCamera->SetupPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f);
	pCamera->LookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_pCamera = pCamera;

	m_root = std::make_shared<SceneNode>();



	//OBJMesh* pEarth = new OBJMesh("EarthSurface/sphere.obj");
	Renderable* pEarth = new CubeMesh(5);
	delete pEarth;
	
	//GLuint uiEarthShader = ShaderHandler::Get()->LoadShader("EarthShader", "Shaders/EarthShader_Normal.vert", "Shaders/EarthShader_Normal.frag");
	//pEarth->GetMaterial()->SetShader(uiEarthShader);
	//
	//std::shared_ptr<SceneNode> m_earth = pEarth->GetSharedSceneNode();
	//m_root->AttachChild(m_earth);

	//m_root->AttachChild(pCamera->GetSharedSceneNode());

	//m_pCloudMesh = new OBJMesh("CloudSurface/sphere.obj");
	//m_uiCloudShader = ShaderHandler::Get()->LoadShader("CloudShader", "Shaders/Clouds.vert", "Shaders/Clouds.frag");
	//m_pCloudMesh->GetMaterial()->SetShader(m_uiCloudShader);
	//
	//m_pSkybox = new SkyboxMesh("SpaceSkybox", "png");

	glClearColor(0.03f, 0.03f, 0.05f, 1.0f);


	//glm::mat4 scale = glm::scale(glm::vec3(3.0f, 3.0f, 3.0f));
	//glm::mat4 rot = glm::rotate(glm::radians(23.5f), glm::vec3(1, 0, 0));
	//m_earthTransform = m_earthTransform * scale * rot;
	//
	//scale = glm::scale(glm::vec3(3.05f, 3.05f, 3.05f));
	//m_cloudTransform = m_cloudTransform * scale;
	//
	//m_fTimeScale = 1.0f;
	//
	//auto parent = std::make_shared<SceneNode>();
	//auto child = std::make_shared<SceneNode>();
	//parent->AttachChild(child);
	//parent->AttachChild(m_pEarthMesh);
	//
	//glm::mat4 tempTransform;
	//tempTransform[3] = glm::vec4(10, 0, 0, 1);
	//parent->SetLocalTransform(tempTransform);
	//
	//tempTransform[3] = glm::vec4(30, 0, 0, 1);
	//child->SetGlobalTransform(tempTransform);

	return true;
}

void EarthApplication::Shutdown()
{
	m_root = nullptr;

	//delete m_pEarthMesh;
	//delete m_pSkybox;
	int i = 3;
}

bool EarthApplication::Update(double dt)
{
	m_pCamera->Update(dt);


	//float fRotAmountPerSecond = (glm::pi<float>() * 2) * (1 / 60.0f);
	//glm::mat4 rot = glm::rotate(fRotAmountPerSecond * (float)dt, glm::vec3(0, 1, 0));
	//
	//m_earthTransform = m_earthTransform * rot;
	//
	//
	//fRotAmountPerSecond = (glm::pi<float>() * 2) * (1 / 70.f); 
	//rot = glm::rotate(fRotAmountPerSecond * (float)dt, glm::vec3(0, 1, 0));
	//m_cloudTransform = m_cloudTransform * rot;

	return true;
}

void EarthApplication::Render()
{
	//DisplayGrid(20);


	//m_pEarthMesh->Render(m_pCamera, m_earthTransform);
	//m_pCloudMesh->Render(m_pCamera, m_cloudTransform);

	//Gizmos::addTransform(glm::mat4(1), 10.0f);

	//m_pSkybox->Render(m_pCamera, glm::mat4(1));
}
