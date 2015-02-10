#ifndef _CUBE_MESH_H_
#define _CUBE_MESH_H_

#include "Renderable.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

class CubeMesh : public Renderable
{
public:
	CubeMesh(float size) : m_fSize(size) { Create(); }

	void Create()
	{
		m_uiShaderProgram = Renderable::CreateStandardShader();

		m_uiNumberOfVerts = 12;
		m_uiNumberOfIndices = 36;

		float fHalfSize = m_fSize * 0.5f;

		Vertex aVertices[8];
		aVertices[0].position = glm::vec4(-fHalfSize, -fHalfSize, -fHalfSize, 1);
		aVertices[1].position = glm::vec4(-fHalfSize, -fHalfSize, fHalfSize, 1);
		aVertices[2].position = glm::vec4(fHalfSize, -fHalfSize, fHalfSize, 1);
		aVertices[3].position = glm::vec4(fHalfSize, -fHalfSize, -fHalfSize, 1);
		aVertices[4].position = glm::vec4(-fHalfSize, fHalfSize, -fHalfSize, 1);
		aVertices[5].position = glm::vec4(-fHalfSize, fHalfSize, fHalfSize, 1);
		aVertices[6].position = glm::vec4(fHalfSize, fHalfSize, fHalfSize, 1);
		aVertices[7].position = glm::vec4(fHalfSize, fHalfSize, -fHalfSize, 1);

		aVertices[0].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		aVertices[1].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		aVertices[2].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		aVertices[3].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		aVertices[4].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		aVertices[5].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		aVertices[6].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		aVertices[7].colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		unsigned int aIndicies[36] = {
			0, 7, 4, //Front
			0, 3, 7,
			1, 4, 5, //Left
			1, 0, 4,
			2, 5, 6, //Back
			2, 1, 5,
			3, 6, 7, //Right
			3, 2, 6,
			4, 7, 6, //Top
			4, 6, 5,
			0, 2, 3,  //Bottom
			0, 1, 2

		};


		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_IBO);
		glGenVertexArrays(1, &m_VAO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		glBufferData(GL_ARRAY_BUFFER, m_uiNumberOfVerts * sizeof(Vertex), aVertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_uiNumberOfIndices * sizeof(unsigned int), aIndicies, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + sizeof(glm::vec4));

		// unbind vertex array
		glBindVertexArray(0);
	}

private:
	float m_fSize;
};

#endif