#ifndef TEXTURE_HANDLER_H_
#define TEXTURE_HANDLER_H_

#include <map>
#include <string>

class TextureLoader
{
public:
	TextureLoader() {};
	~TextureLoader();

	unsigned int LoadTexture(std::string& strTextureName);
	unsigned int GetTexture(std::string& strTextureName) const;

	void UnloadTexture(std::string& strTextureName);

private:
	bool DoesTextureExist(std::string& strTextureName) const;


	std::map<std::string, unsigned int> m_textureMap;
};

#endif