#include "Renderable.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>

#include "Cameras/BaseCamera.h"
#include "Engine/Material.h"
#include "Engine/MaterialHandler.h"


Renderable::Renderable()
: m_VAO(0)
, m_VBO(0)
, m_IBO(0)
, m_uiNumberOfIndices(0)
, m_uiNumberOfVerts(0)
, m_pMaterial(nullptr)
, m_strMaterialName("")
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
	if(!PrepareToRender(transform)) return;

	//Bind the vertex array for this object
	glBindVertexArray(m_VAO);

	//Render the object..
	glDrawElements(GL_TRIANGLES, m_uiNumberOfIndices, GL_UNSIGNED_INT, 0);

	//Unbind the vertex array
	glBindVertexArray(0);
}
//
//unsigned int Renderable::CreateStandardShader()
//{
//	if (sm_iStandardShaderProgram)
//	{
//		return sm_iStandardShaderProgram;
//	}
//	else
//	{
//		const char* vsSource = "#version 150\n \
//							   	in vec4 Position; \
//								in vec4 Colour; \
//								out vec4 vColour; \
//								uniform mat4 Projection; \
//								uniform mat4 View; \
//								uniform mat4 Model; \
//								void main() { vColour = Colour; gl_Position = Projection * View * Model * Position; }";
//
//		const char* fsSource = "#version 150\n \
//							   	in vec4 vColour; \
//								out vec4 FragColor; \
//								void main()	{ FragColor = vColour; }";
//
//
//		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
//		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
//
//		glShaderSource(vs, 1, (const char**)&vsSource, 0);
//		glCompileShader(vs);
//
//		glShaderSource(fs, 1, (const char**)&fsSource, 0);
//		glCompileShader(fs);
//
//		sm_iStandardShaderProgram = glCreateProgram();
//		glAttachShader(sm_iStandardShaderProgram, vs);
//		glAttachShader(sm_iStandardShaderProgram, fs);
//		glBindAttribLocation(sm_iStandardShaderProgram, 0, "Position");
//		glBindAttribLocation(sm_iStandardShaderProgram, 1, "Colour");
//		glLinkProgram(sm_iStandardShaderProgram);
//
//		int success = GL_FALSE;
//		glGetProgramiv(sm_iStandardShaderProgram, GL_LINK_STATUS, &success);
//		if (success == GL_FALSE)
//		{
//			int infoLogLength = 0;
//			glGetShaderiv(sm_iStandardShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
//			char* infoLog = new char[infoLogLength];
//
//			glGetShaderInfoLog(sm_iStandardShaderProgram, infoLogLength, 0, infoLog);
//			printf("Error: Failed to link Gizmo shader program!\n%s\n", infoLog);
//			delete[] infoLog;
//		}
//
//		glDeleteShader(vs);
//		glDeleteShader(fs);
//
//		return sm_iStandardShaderProgram;
//	}
//}

void Renderable::SetMaterial(Material* a_pMaterial)
{
	m_pMaterial = a_pMaterial;
	m_strMaterialName = m_pMaterial->name;
}

void Renderable::SetMaterial(std::string& a_strMaterial)
{
	m_strMaterialName = a_strMaterial;
	m_pMaterial = MaterialHandler::Get()->GetMaterial(a_strMaterial);
}

Material* Renderable::GetMaterial()
{
	return m_pMaterial;
}

bool Renderable::PrepareToRender(glm::mat4 transform)
{
	//If no Indices..nothing to render!
	if (m_uiNumberOfIndices == 0) return false;

	if (m_pMaterial == nullptr) //This will only ever happen on the first render
	{
		//Attempt to get the material
		if (MaterialHandler::Get()) m_pMaterial = MaterialHandler::Get()->GetMaterial(m_strMaterialName);
		//if still null, set to error shader
		if (MaterialHandler::Get() && m_pMaterial == nullptr) m_pMaterial = MaterialHandler::Get()->GetDefaultErrorMaterial();
	}

	//Set the material as the current material
	if (m_pMaterial != nullptr) m_pMaterial->SetAsActiveMaterial();

	//Set world/model transform
	m_pMaterial->SetShaderModelMatrix(transform);

	return true;
}
