#include "Material.h"

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "MaterialHandler.h"

void Material::SetAsActiveMaterial()
{
	glUseProgram(m_iShaderID);

	glUniform4fv(m_iDiffuseShaderLocation, 1, glm::value_ptr(diffuse));
	glUniform4fv(m_iAmbientShaderLocation, 1, glm::value_ptr(ambient));
	glUniform4fv(m_iSpecularShaderLocation, 1, glm::value_ptr(specular));
	glUniform4fv(m_iemissiveShaderLocation, 1, glm::value_ptr(emissive));

	//Next we need to bind the correct textures
	for(auto it = m_textureMap.begin();it != m_textureMap.end();++it)
	{
		if(it->second > 0)
		{
			glActiveTexture(GL_TEXTURE0 + it->first);
			if (it->first != CUBE)
			{
				glBindTexture(GL_TEXTURE_2D, it->second);
			}
			else
			{
				glBindTexture(GL_TEXTURE_CUBE_MAP, it->second);
			}
		}
	}
}

void Material::SetShader( unsigned int a_uiShaderID )
{
	m_iShaderID = a_uiShaderID;

	m_iDiffuseShaderLocation = glGetUniformLocation(m_iShaderID, "diffuseMaterial");
	m_iAmbientShaderLocation = glGetUniformLocation(m_iShaderID, "ambientMaterial");
	m_iSpecularShaderLocation = glGetUniformLocation(m_iShaderID, "specularMaterial");
	m_iemissiveShaderLocation = glGetUniformLocation(m_iShaderID, "emmisiveMaterial");

	m_iModelMatrixShaderLocation = glGetUniformLocation(m_iShaderID, "Model");
	m_iViewMatrixShaderLocation = glGetUniformLocation(m_iShaderID, "View");
	m_iProjectionMatrixShaderLocation = glGetUniformLocation(m_iShaderID, "Projection");

	MaterialHandler::Get()->ConfigureOpenGLTextureSlots(m_iShaderID);
}

void Material::SetShaderModelMatrix( glm::mat4& a_rModelMatrix )
{
	glUseProgram(m_iShaderID);
	glUniformMatrix4fv(m_iModelMatrixShaderLocation, 1, false, glm::value_ptr(a_rModelMatrix));
}

void Material::SetShaderProjectionMatrix( glm::mat4& a_rProjectionMatrix )
{
	glUseProgram(m_iShaderID);

	glUniformMatrix4fv(m_iProjectionMatrixShaderLocation, 1, false, glm::value_ptr(a_rProjectionMatrix));
}

void Material::SetShaderViewMatrix( glm::mat4& a_rViewMatrix )
{
	glUseProgram(m_iShaderID);

	glUniformMatrix4fv(m_iViewMatrixShaderLocation, 1, false, glm::value_ptr(a_rViewMatrix));
}
