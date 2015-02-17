#ifndef _SKYBOX_MESH_H_
#define _SKYBOX_MESH_H_

#include "Engine/Renderable.h"
#include "Engine/gl_core_4_4.h"
#include <GLFW/glfw3.h>

class SkyboxMesh : public Renderable
{
public:
	SkyboxMesh(std::string folderName, std::string extension) : m_folderName(folderName), m_extension(extension) { Create(); }

	void Create();
	void Render(BaseCamera* pCamera, glm::mat4 transform);

	void CreateVertices();
	void CreateMaterial();
private:
	std::string m_folderName;
	std::string m_extension;

};

#endif