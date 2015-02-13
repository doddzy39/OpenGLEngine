#include "TextureLoader.h"

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureLoader::~TextureLoader()
{
	auto it = m_textureMap.begin();
	while(it != m_textureMap.end())
	{
		glDeleteTextures(1, &(it->second));
		++it;
	}
}

unsigned int TextureLoader::LoadTexture( std::string& strTextureName )
{
	if(DoesTextureExist(strTextureName)) return m_textureMap[strTextureName];


	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load(strTextureName.c_str(),
		&imageWidth, &imageHeight, &imageFormat, STBI_default);


	unsigned int uiTextureID = 0;


	glGenTextures(1, &uiTextureID);
	glBindTexture(GL_TEXTURE_2D, uiTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	m_textureMap[strTextureName] = uiTextureID;

	return uiTextureID;
}

unsigned int TextureLoader::GetTexture( std::string& strTextureName ) const
{
	if(DoesTextureExist(strTextureName)) return m_textureMap.at(strTextureName);

	return NULL;
}

void TextureLoader::UnloadTexture( std::string& strTextureName )
{
	if(!DoesTextureExist(strTextureName)) return; //Not loaded

	unsigned int uiTextureID = m_textureMap[strTextureName];

	glDeleteTextures(1, &uiTextureID);

	 m_textureMap.erase(strTextureName);
}

bool TextureLoader::DoesTextureExist( std::string& strTextureName ) const
{
	if(m_textureMap.find(strTextureName) == m_textureMap.end()) return false;

	return true;
}