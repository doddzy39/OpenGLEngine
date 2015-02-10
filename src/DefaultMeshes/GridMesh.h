#ifndef _GRID_MESH_H_
#define _GRID_MESH_H_

#include "Engine/Renderable.h"
#include "Engine/gl_core_4_4.h"
#include <GLFW/glfw3.h>

class GridMesh : public Renderable
{
public:
	GridMesh(unsigned int rows, unsigned int cols) : m_rows(rows), m_cols(cols) { Create(); }

	void Create()
	{
		m_uiShaderProgram = Renderable::CreateStandardShader();

		m_uiNumberOfVerts = m_rows * m_cols;
		m_uiNumberOfIndices = (m_rows - 1) * (m_cols - 1) * 6;

		Vertex* aoVertices = new Vertex[m_uiNumberOfVerts];
		for (unsigned int r = 0; r < m_rows; ++r) {
			for (unsigned int c = 0; c < m_cols; ++c) {
				aoVertices[r * m_cols + c].position = glm::vec4(
					(float)c, 0, (float)r, 1);
				// create some arbitrary colour based off something
				// that might not be related to tiling a texture
				glm::vec3 colour = glm::vec3(sinf((c / (float)(m_cols - 1)) *
					(r / (float)(m_rows - 1))));
				aoVertices[r * m_cols + c].colour = glm::vec4(colour, 1);
			}
		}

		// defining index count based off quad count (2 triangles per quad)
		unsigned int* auiIndices = new unsigned int[m_uiNumberOfIndices];
		unsigned int index = 0;
		for (unsigned int r = 0; r < (m_rows - 1); ++r) {
			for (unsigned int c = 0; c < (m_cols - 1); ++c) {
				// triangle 1
				auiIndices[index++] = r * m_cols + c;
				auiIndices[index++] = (r + 1) * m_cols + c;
				auiIndices[index++] = (r + 1) * m_cols + (c + 1);
				// triangle 2
				auiIndices[index++] = r * m_cols + c;
				auiIndices[index++] = (r + 1) * m_cols + (c + 1);
				auiIndices[index++] = r * m_cols + (c + 1);
			}
		}


		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_IBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, (m_rows * m_cols) * sizeof(Vertex),
			aoVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)(sizeof(glm::vec4)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_uiNumberOfIndices *
			sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);


		delete[] aoVertices;
		delete[] auiIndices;

	}

private:
	unsigned int m_rows;
	unsigned int m_cols;
};

#endif