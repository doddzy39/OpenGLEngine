#ifndef MATERIAL_H_
#define MATERIAL_H_


#include <map>
#include "gl_core_4_4.h"
#include <glm/glm.hpp>

class Material
{
public:
	enum TextureTypes
	{
		DIFFUSE,
		AMBIENT,
		GLOW,
		SPECULAR,
		GLOSS,
		NORMAL,
		ALPHA,
		DISPLACEMENT,
		CUBE,
	};

	char			name[MAX_PATH];
	glm::vec4			ambient;	// RGB + Ambient Factor stored in A
	glm::vec4			diffuse;	// RGBA
	glm::vec4			specular;	// RGB + Shininess/Gloss stored in A
	glm::vec4			emissive;	// RGB + Emissive Factor stored in A

	void SetTexture(TextureTypes a_eType, unsigned int a_uiTextureID) { m_textureMap[a_eType] = a_uiTextureID; }

	void			   SetShader(unsigned int a_uiShaderID); //Also binds the shader - watch out for this
	const unsigned int GetShaderID() { return m_iShaderID; };
	
	//TODO: SPEED THIS UP, CHANGE TO WORK WITH RENDER PIPELINE ONCE WE HAVE A PROPER ONE
	void SetAsActiveMaterial();

	//These will all used cached lookup values, so will be as fast as possible
	void SetShaderModelMatrix(glm::mat4& a_rModelMatrix);
	void SetShaderProjectionMatrix(glm::mat4& a_rProjectionMatrix);
	void SetShaderViewMatrix(glm::mat4& a_rViewMatrix);
private:


	std::map<TextureTypes, unsigned int> m_textureMap;
	unsigned int m_iShaderID;

	int	m_iDiffuseShaderLocation;
	int m_iAmbientShaderLocation;
	int m_iSpecularShaderLocation;
	int m_iemissiveShaderLocation;

	int m_iModelMatrixShaderLocation;
	int m_iViewMatrixShaderLocation;
	int m_iProjectionMatrixShaderLocation;
};
#endif