#ifndef TEXTURE_HANDLER_H_
#define TEXTURE_HANDLER_H_

#include <map>
#include <string>

class TextureLoader
{
public:
	enum CUBE_MAP_NAMING
	{
		SEQUENCE,
		POSITION
	};

	TextureLoader() {};
	~TextureLoader();

	unsigned int LoadTexture(std::string& strTextureName);
	unsigned int GetTexture(std::string& strTextureName) const;

	unsigned int LoadCubeMap(std::string& strFolderName, std::string strExtention, CUBE_MAP_NAMING eNaming);


	void UnloadTexture(std::string& strTextureName);

private:
	bool DoesTextureExist(std::string& strTextureName) const;

	void LoadCubeMapSequencial(std::string& strFolderName, std::string strExtention);
	void LoadCubeMapPosition(std::string& strFolderName, std::string strExtention);

	std::map<std::string, unsigned int> m_textureMap;
	std::map<std::string, unsigned int> m_cubeMap;
};

#endif