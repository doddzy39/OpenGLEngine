#include "SkyboxMesh.h"
#include "Engine/MaterialHandler.h"
#include "Engine/ShaderHandler.h"
#include "Engine/Material.h"
#include "Engine/TextureLoader.h"


void SkyboxMesh::Create()
{
	CreateVertices();

	CreateMaterial();
}

void SkyboxMesh::CreateVertices()
{
	m_uiNumberOfIndices = 36;
	m_uiNumberOfVerts = 36;

	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};


	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(GLfloat),
		skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
}

void SkyboxMesh::CreateMaterial()
{
	Material* pMaterial = new Material();

 	unsigned int cubeMap = MaterialHandler::Get()->GetTextureLoader()->LoadCubeMap(m_folderName, m_extension, TextureLoader::POSITION);
	pMaterial->SetTexture(Material::CUBE, cubeMap);
	pMaterial->SetShader(ShaderHandler::Get()->LoadShader("Skybox", "shaders/skybox.vert", "shaders/skybox.frag"));
	SetMaterial(pMaterial);
}

void SkyboxMesh::Render(BaseCamera* pCamera, glm::mat4 transform)
{

	if (!PrepareToRender(transform)) return;

	glDepthMask(GL_FALSE);
	glBindVertexArray(m_VAO);
	
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}
