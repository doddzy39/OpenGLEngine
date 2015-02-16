#include "MaterialHandler.h"


#include "Material.h"
#include "TextureLoader.h"
#include "ShaderHandler.h"

#include <assert.h>
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>


MaterialHandler* MaterialHandler::sm_pInstance = nullptr;

MaterialHandler* MaterialHandler::Initialize()
{
	if(sm_pInstance == nullptr)
	{
		sm_pInstance = new MaterialHandler();
		return sm_pInstance;
	}

	return sm_pInstance;
}

void MaterialHandler::Shutdown()
{
	if(sm_pInstance != nullptr)
	{
		delete sm_pInstance;
		sm_pInstance = NULL;
	}
}

MaterialHandler* MaterialHandler::Get()
{
	return sm_pInstance;
}

MaterialHandler::MaterialHandler() : m_pErrorMaterial(nullptr)
{
	m_pTextureHandler = new TextureLoader();

}

MaterialHandler::~MaterialHandler()
{
	//Delete all of the loaded materials
	for(auto it = m_materialMap.begin();it != m_materialMap.end();++it)
	{
		delete it->second;
	}

	//Delete our texture handler
	delete m_pTextureHandler;

	delete m_pErrorMaterial;
}


void MaterialHandler::ConfigureOpenGLTextureSlots( unsigned int uiShaderID )
{
	glUseProgram(uiShaderID);
	//Need to tell the shader where to find textures to use (if the shader uses them)
	GLuint shaderHandle = glGetUniformLocation(uiShaderID, "diffuseTexture");
	glUniform1i(shaderHandle, Material::DIFFUSE);
	shaderHandle = glGetUniformLocation(uiShaderID, "ambientTexture");
	glUniform1i(shaderHandle, Material::AMBIENT);
	shaderHandle = glGetUniformLocation(uiShaderID, "glowTexture");
	glUniform1i(shaderHandle, Material::GLOW);
	shaderHandle = glGetUniformLocation(uiShaderID, "specularTexture");
	glUniform1i(shaderHandle, Material::SPECULAR);
	shaderHandle = glGetUniformLocation(uiShaderID, "glossTexture");
	glUniform1i(shaderHandle, Material::GLOSS);
	shaderHandle = glGetUniformLocation(uiShaderID, "normalTexture");
	glUniform1i(shaderHandle, Material::NORMAL);
	shaderHandle = glGetUniformLocation(uiShaderID, "alphaTexture");
	glUniform1i(shaderHandle, Material::ALPHA);
	shaderHandle = glGetUniformLocation(uiShaderID, "displacementTexture");
	glUniform1i(shaderHandle, Material::DISPLACEMENT);
}

Material* MaterialHandler::GetDefaultErrorMaterial()
{
	if(m_pErrorMaterial != nullptr)
		return m_pErrorMaterial;
	else
	{
		m_pErrorMaterial = new Material();
		if(ShaderHandler::Get()) m_pErrorMaterial->SetShader(ShaderHandler::Get()->GetErrorShader());
		return m_pErrorMaterial;
	}

	return nullptr;
}

Material* MaterialHandler::GetMaterial( std::string& a_strMaterialName )
{
	if(DoesMaterialExist(a_strMaterialName)) return m_materialMap.at(a_strMaterialName);

	return NULL;
}

bool MaterialHandler::DoesMaterialExist( std::string& a_strMaterialName )
{
	if(m_materialMap.find(a_strMaterialName) == m_materialMap.end()) return false;

	return true;
}

void MaterialHandler::CreateNewMaterial( std::string& a_strName )
{
	Material* pMaterial = new Material;
	m_materialMap[a_strName] = pMaterial;
	pMaterial->SetShader(m_uiDefaultShader);
}

Material* MaterialHandler::CreateNewMaterial( tinyobj::material_t* a_pMaterial, std::string& a_strTexturePath )
{
	assert(a_pMaterial != nullptr);
	Material* pMaterial = new Material;
	m_materialMap[a_pMaterial->name] = pMaterial;

	strcpy(pMaterial->name, a_pMaterial->name.c_str());

	pMaterial->ambient = glm::vec4(a_pMaterial->ambient[0], a_pMaterial->ambient[1], a_pMaterial->ambient[2], 1);
	pMaterial->diffuse = glm::vec4(a_pMaterial->diffuse[0], a_pMaterial->diffuse[1], a_pMaterial->diffuse[2], 1);
	pMaterial->specular = glm::vec4(a_pMaterial->specular[0], a_pMaterial->specular[1], a_pMaterial->specular[2], 1);
	pMaterial->emissive = glm::vec4(a_pMaterial->emission[0], a_pMaterial->emission[1], a_pMaterial->emission[2], 1);;

	if (a_pMaterial->diffuse_texname != "")
	{
		pMaterial->SetTexture(Material::DIFFUSE, m_pTextureHandler->LoadTexture(a_strTexturePath + std::string(a_pMaterial->diffuse_texname)));
	}
	if (a_pMaterial->ambient_texname != "")
	{
		pMaterial->SetTexture(Material::AMBIENT, m_pTextureHandler->LoadTexture(a_strTexturePath + std::string(a_pMaterial->ambient_texname)));
	}

	if (a_pMaterial->normal_texname != "")
	{
		pMaterial->SetTexture(Material::NORMAL, m_pTextureHandler->LoadTexture(a_strTexturePath + std::string(a_pMaterial->normal_texname)));
	}

	if (a_pMaterial->specular_texname != "")
	{
		pMaterial->SetTexture(Material::SPECULAR, m_pTextureHandler->LoadTexture(a_strTexturePath + std::string(a_pMaterial->specular_texname)));
	}

	pMaterial->SetShader(m_uiDefaultShader);

	return pMaterial;
}


void MaterialHandler::SetDefaultShader( unsigned int a_uiShaderID )
{
	m_uiDefaultShader = a_uiShaderID;
}
