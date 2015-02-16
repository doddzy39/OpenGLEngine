#include "TextureLoader.h"

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
#include <list>

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

unsigned int TextureLoader::LoadCubeMap(std::string& strFolderName, std::string strExtention, CUBE_MAP_NAMING eNaming)
{
	if (m_cubeMap[strFolderName])
	{
		return m_cubeMap[strFolderName];
	}


	unsigned int uiTextureID = 0;
	glGenTextures(1, &uiTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, uiTextureID);

	switch (eNaming)
	{
	case TextureLoader::SEQUENCE:
		LoadCubeMapSequencial(strFolderName, strExtention);
		break;
	case TextureLoader::POSITION:
		LoadCubeMapPosition(strFolderName, strExtention);
		break;
	default:
		break;
	}
	

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	m_cubeMap[strFolderName] = uiTextureID;

	return uiTextureID;
}

void TextureLoader::LoadCubeMapSequencial(std::string& strFolderName, std::string strExtention)
{
	for (int i = 0; i < 6; ++i)
	{
		int imageWidth = 0, imageHeight = 0, imageFormat = 0;

		std::string filename = strFolderName;
		filename += "/";
		char buff[2];
		itoa(i, buff, 10);
		filename += buff;
		filename += ".";
		filename += strExtention;

		unsigned char* data = stbi_load(filename.c_str(),
			&imageWidth, &imageHeight, &imageFormat, STBI_default);

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
	}
}

void TextureLoader::LoadCubeMapPosition(std::string& strFolderName, std::string strExtention)
{
	std::list<std::string> files;
	files.push_back("RT");
	files.push_back("LF");
	files.push_back("UP");
	files.push_back("DN");
	files.push_back("BK");
	files.push_back("FR");

	unsigned int uiCounter = 0;
	for each(std::string name in files)
	{
		int imageWidth = 0, imageHeight = 0, imageFormat = 0;

		std::string filename = strFolderName;
		filename += "/";
		filename += name;
		filename += ".";
		filename += strExtention;

		unsigned char* data = stbi_load(filename.c_str(),
			&imageWidth, &imageHeight, &imageFormat, STBI_default);

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + uiCounter,
			0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);

		uiCounter++;
	}
}
