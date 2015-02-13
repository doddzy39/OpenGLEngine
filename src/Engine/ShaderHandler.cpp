#include "ShaderHandler.h"


#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <string>


ShaderHandler* ShaderHandler::sm_pInstance = nullptr;

ShaderHandler* ShaderHandler::Initialize()
{
	if(sm_pInstance == nullptr)
	{
		sm_pInstance = new ShaderHandler();
		return sm_pInstance;
	}

	return sm_pInstance;
}

void ShaderHandler::Shutdown()
{
	if(sm_pInstance != nullptr)
	{
		delete sm_pInstance;
		sm_pInstance = NULL;
	}
}

ShaderHandler* ShaderHandler::Get()
{
	return sm_pInstance;
}

ShaderHandler::ShaderHandler()
	:	m_uiErrorShader(0)
{
	CreateErrorShader();
}

ShaderHandler::~ShaderHandler()
{
	for(auto it = m_shaderMap.begin();it != m_shaderMap.end(); ++it)
	{
		glDeleteShader(it->second);
	}

	glDeleteShader(m_uiErrorShader);

}

unsigned int ShaderHandler::GetShader(std::string& a_strShaderName)
{
	if(DoesShaderExist(a_strShaderName)) return m_shaderMap.at(a_strShaderName);

	return 0;
}

bool ShaderHandler::DoesShaderExist( std::string& a_strMaterialName )
{
	if (m_shaderMap.find(a_strMaterialName) != m_shaderMap.end())
	{
		return true;
	}

	return false;
}

unsigned int ShaderHandler::GetErrorShader()
{
	return m_uiErrorShader;
}

void ShaderHandler::CreateErrorShader()
{
	// create shaders
	char* vsSource = "#version 330\n\
					 layout(location=0)in vec4 Position; \
					 uniform mat4 Projection; \
					 uniform mat4 View; \
					 uniform mat4 Model; \
					 void main() { gl_Position = Projection * View * Model * Position; }";

	char* fsSource = "#version 330\n \
					 out vec4 outColour; \
					 void main()	{ outColour = vec4(1, 0, 1, 1); }";

	GLuint vsHandle = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsHandle = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsHandle, 1, (const char**)&vsSource, 0);
	glCompileShader(vsHandle);

	glShaderSource(fsHandle, 1, (const char**)&fsSource, 0);
	glCompileShader(fsHandle);

	m_uiErrorShader = glCreateProgram();
	glAttachShader(m_uiErrorShader, vsHandle);
	glAttachShader(m_uiErrorShader, fsHandle);
	glBindAttribLocation(m_uiErrorShader, 0, "Position");
	glBindFragDataLocation(m_uiErrorShader, 0, "outColour");
	glLinkProgram(m_uiErrorShader);

	m_shaderMap["errorShader"] = m_uiErrorShader;
}

unsigned int ShaderHandler::LoadShader(std::string a_strShaderName, const char* a_szVertexShader, const char* a_szPixelShader, const char* a_szGeometryShader /*= nullptr*/, const char* a_szTessellationControlShader /*= nullptr*/, const char* a_szTessellationEvaluationShader /*= nullptr*/ )
{
	GLuint vs = LoadShader(a_szVertexShader, GL_VERTEX_SHADER);
	GLuint fs = LoadShader(a_szPixelShader, GL_FRAGMENT_SHADER);

	GLuint gs = 0;
	GLuint tcs = 0;
	GLuint tes = 0;

	if(a_szGeometryShader != nullptr)
	{
		gs = LoadShader(a_szGeometryShader, GL_GEOMETRY_SHADER);
	}

	if(a_szTessellationControlShader && a_szTessellationEvaluationShader)
	{
		tcs = LoadShader(a_szTessellationControlShader, GL_TESS_CONTROL_SHADER);
		tes = LoadShader(a_szTessellationEvaluationShader, GL_TESS_EVALUATION_SHADER);
	}

	unsigned int uiShaderID = CreateProgram(vs, tcs, tes, gs, fs);
	m_shaderMap[a_strShaderName] = uiShaderID;

	return uiShaderID;
}

unsigned int ShaderHandler::LoadShader(const char* a_strName, unsigned int a_uiType)
{
	int success = GL_FALSE;

	unsigned int handle = glCreateShader(a_uiType);
	unsigned char* source = FileToBuffer(a_strName);

	glShaderSource(handle, 1, (const char**)&source, 0);
	glCompileShader(handle);

	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(handle, infoLogLength, 0, infoLog);
		printf("Error: Failed to compile shader!\n");
		printf("%s", infoLog);
		printf("\n");
		delete[] infoLog;
		return 0;
	}

	delete[] source;

	return handle;

}

void ShaderHandler::SetProjectionMatrix( glm::mat4 a_rProjMat )
{
	for(auto it = m_shaderMap.begin();it != m_shaderMap.end();++it)
	{
		glUseProgram(it->second);
		GLint loc = glGetUniformLocation(it->second, "Projection");
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(a_rProjMat));
	}
}

void ShaderHandler::SetViewMatrix( glm::mat4 a_rViewMat )
{
	for(auto it = m_shaderMap.begin();it != m_shaderMap.end();++it)
	{
		glUseProgram(it->second);
		GLint loc = glGetUniformLocation(it->second, "View");
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(a_rViewMat));
	}
}

void ShaderHandler::SetEyePosition( glm::vec4 a_rEyePos )
{
	//unsigned int uiError;
	for(auto it = m_shaderMap.begin();it != m_shaderMap.end();++it)
	{
		glUseProgram(it->second);
		GLint loc = glGetUniformLocation(it->second, "EyePos");
		glUniform4fv(loc, 1, glm::value_ptr(a_rEyePos));

		//unsigned int uiError = glGetError();
	}
}

unsigned char* ShaderHandler::FileToBuffer(const char* a_strName)
{
	// open file for text reading
	FILE* pFile = fopen(a_strName, "rb");
	if (pFile == nullptr)
	{
		printf("Error: Unable to open file '%s' for reading!\n", a_strName);
		return nullptr;
	}

	// get number of bytes in file
	fseek(pFile, 0, SEEK_END);
	unsigned int uiLength = (unsigned int)ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// allocate buffer and read file contents
	unsigned char* acBuffer = new unsigned char[uiLength + 1];
	memset(acBuffer, 0, uiLength + 1);
	fread(acBuffer, sizeof(unsigned char), uiLength, pFile);

	fclose(pFile);
	return acBuffer;
}

unsigned int ShaderHandler::CreateProgram(GLuint vs, GLuint tcs, GLuint tes, GLuint gs, GLuint fs)
{
	int success = GL_FALSE;

	// create a shader program and attach the shaders to it
	unsigned int handle = glCreateProgram();
	glAttachShader(handle, vs);
	glAttachShader(handle, tcs);
	glAttachShader(handle, tes);
	glAttachShader(handle, gs);
	glAttachShader(handle, fs);

	// link the program together and log errors
	glLinkProgram(handle);

	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(handle, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s", infoLog);
		printf("\n");
		delete[] infoLog;
		return 0;
	}

	return handle;
}
