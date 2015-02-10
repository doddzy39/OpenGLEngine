#include "Renderable.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include "BaseCamera.h"

int Renderable::sm_iStandardShaderProgram = 0;

Renderable::Renderable()
: m_VAO(0)
, m_VBO(0)
, m_IBO(0)
, m_uiShaderProgram(0)
, m_uiNumberOfIndices(0)
, m_uiNumberOfVerts(0)
, m_iModelMatrixShaderLocation(-1)
, m_iViewMatrixShaderLocation(-1)
, m_iProjectionMatrixShaderLocation(-1) 
{

}

Renderable::~Renderable()
{

	if (m_VAO == 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}
	if (m_VBO == 0)
	{
		glDeleteBuffers(1, &m_VBO);
	}
	if (m_IBO == 0)
	{
		glDeleteBuffers(1, &m_IBO);
	}
}

void Renderable::Render(BaseCamera* pCamera, glm::mat4 transform)
{
	//If no Indices..nothing to render!
	if (m_uiNumberOfIndices == 0) return;

	//Also don't render if we don't have a working shader - we'll make a
	//a nice, clear error shader for this later
	if (m_uiShaderProgram == 0) return;

	//Get the shader locations if we don't have them already
	SetupShaderLocations();

	glUseProgram(m_uiShaderProgram);
	glUniformMatrix4fv(m_iModelMatrixShaderLocation, 1, false, glm::value_ptr(transform));
	glUniformMatrix4fv(m_iViewMatrixShaderLocation, 1, false, glm::value_ptr(pCamera->GetView()));
	glUniformMatrix4fv(m_iProjectionMatrixShaderLocation, 1, false, glm::value_ptr(pCamera->GetProjection()));

	//Bind the vertex array for this object
	glBindVertexArray(m_VAO);

	//Render the object..
	glDrawElements(GL_TRIANGLES, m_uiNumberOfIndices, GL_UNSIGNED_INT, 0);

	//Unbind the vertex array
	glBindVertexArray(0);
}

void Renderable::SetupShaderLocations()
{
	if (m_uiShaderProgram == 0) return; //No shader to load stuff from

	if (m_iModelMatrixShaderLocation == -1)
	{
		m_iModelMatrixShaderLocation = glGetUniformLocation(m_uiShaderProgram, "Model");
	}

	if (m_iViewMatrixShaderLocation)
	{
		m_iViewMatrixShaderLocation = glGetUniformLocation(m_uiShaderProgram, "View");
	}

	if (m_iProjectionMatrixShaderLocation)
	{
		m_iProjectionMatrixShaderLocation = glGetUniformLocation(m_uiShaderProgram, "Projection");
	}
}

unsigned int Renderable::CreateStandardShader()
{
	if (sm_iStandardShaderProgram)
	{
		return sm_iStandardShaderProgram;
	}
	else
	{
		const char* vsSource = "#version 150\n \
							   	in vec4 Position; \
								in vec4 Colour; \
								out vec4 vColour; \
								uniform mat4 Projection; \
								uniform mat4 View; \
								uniform mat4 Model; \
								void main() { vColour = Colour; gl_Position = Projection * View * Model * Position; }";

		const char* fsSource = "#version 150\n \
							   	in vec4 vColour; \
								out vec4 FragColor; \
								void main()	{ FragColor = vColour; }";


		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vs, 1, (const char**)&vsSource, 0);
		glCompileShader(vs);

		glShaderSource(fs, 1, (const char**)&fsSource, 0);
		glCompileShader(fs);

		sm_iStandardShaderProgram = glCreateProgram();
		glAttachShader(sm_iStandardShaderProgram, vs);
		glAttachShader(sm_iStandardShaderProgram, fs);
		glBindAttribLocation(sm_iStandardShaderProgram, 0, "Position");
		glBindAttribLocation(sm_iStandardShaderProgram, 1, "Colour");
		glLinkProgram(sm_iStandardShaderProgram);

		int success = GL_FALSE;
		glGetProgramiv(sm_iStandardShaderProgram, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			int infoLogLength = 0;
			glGetShaderiv(sm_iStandardShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];

			glGetShaderInfoLog(sm_iStandardShaderProgram, infoLogLength, 0, infoLog);
			printf("Error: Failed to link Gizmo shader program!\n%s\n", infoLog);
			delete[] infoLog;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);

		return sm_iStandardShaderProgram;
	}
}
