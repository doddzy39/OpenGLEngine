#ifndef SHADER_HANDLER_H_
#define SHADER_HANDLER_H_

#include <string>
#include <map>
#include <glm/glm.hpp>


class ShaderHandler
{
public:
	~ShaderHandler();

	static ShaderHandler*	Initialize();
	static void				Shutdown();
	static ShaderHandler*	Get();

	unsigned int GetShader(std::string& a_strShaderName);

	unsigned int LoadShader(std::string a_strShaderName,
							const char* a_szVertexShader, const char* a_szPixelShader,
							const char* a_szGeometryShader = nullptr,
							const char* a_szTessellationControlShader = nullptr, const char* a_szTessellationEvaluationShader = nullptr);

	unsigned int GetErrorShader();
	unsigned int GetScreenAlignedShader();

	//TODO: REWRITE THESE SO THEY DON'T DO LOOKUPS EACH FRAME
	void SetProjectionMatrix(glm::mat4 a_rProjMat);
	void SetViewMatrix(glm::mat4 a_rViewMat);
	void SetEyePosition(glm::vec4 a_rEyePos);
private:
	ShaderHandler();

	bool DoesShaderExist( std::string& a_strMaterialName );
	unsigned int LoadShader(const char* a_strName, unsigned int a_uiType);

	//Setup default shaders here - these are ones that must exist for the engine to work
	void CreateErrorShader();
	unsigned char* FileToBuffer(const char* a_strName);
	unsigned int CreateProgram(unsigned int, unsigned int tcs, unsigned int tes, unsigned int gs, unsigned int fs);
	std::map<std::string, unsigned int> m_shaderMap;

	unsigned int m_uiErrorShader;

	//EVVVVVVVVIL
	static ShaderHandler* sm_pInstance;
};

#endif