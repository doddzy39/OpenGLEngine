#ifndef _MATERIAL_HANDLER_H_
#define _MATERIAL_HANDLER_H_

#include "tiny_obj_loader.h"

#include <string>
#include <map>


class TextureLoader;
class Material;

class MaterialHandler
{
public:
	~MaterialHandler();

	static MaterialHandler* Initialize();
	static void				Shutdown();
	static MaterialHandler* Get();

	void CreateNewMaterial(std::string& a_strName);
	Material* CreateNewMaterial(tinyobj::material_t* a_pMaterial, std::string& a_strTexturePath);
	//void CreateMaterialsFromFBXScene(FBXFile* a_pScene);

	Material* GetMaterial(std::string& a_strMaterialName);

	static void ConfigureOpenGLTextureSlots(unsigned int uiShaderID);
	
	Material* GetDefaultErrorMaterial();
	Material* GetDefaultScreenAlignedMaterial();

	void SetDefaultShader( unsigned int a_uiShaderID );

	TextureLoader* GetTextureLoader() { return m_pTextureHandler;  }
private:
	MaterialHandler();
	bool DoesMaterialExist( std::string& a_strMaterialName );
	
	//EVVVVVVVVIL
	static MaterialHandler* sm_pInstance;

	TextureLoader* m_pTextureHandler; //Contains all the textures used by these materials
	std::map<std::string, Material*> m_materialMap;
	unsigned int m_uiDefaultShader;

	Material* m_pErrorMaterial;
};

#endif