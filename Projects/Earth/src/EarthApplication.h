#ifndef EARTH_APPLICATION_H_
#define EARTH_APPLICATION_H_

#include "Engine/GLApplication.h"
#include <glm/glm.hpp>

#include <memory>
#include "SceneGraph/SceneNode.h"

class OBJMesh;
class Shader;
class SkyboxMesh;

class EarthApplication : public GLApplication
{
public:
	EarthApplication() : GLApplication("Tutorial 4 - Advanced Texturing and Lighting", 1920, 1200) {}
	~EarthApplication() {}

	virtual bool Startup();
	virtual void Shutdown();

	virtual bool Update(double dt);
	virtual void Render();

protected:
	std::shared_ptr<SceneNode> m_root;

	//SkyboxMesh*		m_pSkybox;
	//OBJMesh*		m_pEarthMesh;
	//OBJMesh*		m_pCloudMesh;
	//
	//unsigned int	m_uiEarthShader;
	//unsigned int	m_uiCloudShader;
	//
	//glm::mat4		m_earthTransform;
	//glm::mat4		m_cloudTransform;


	float			m_fTimeScale;
private:
};


#endif