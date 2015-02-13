#include "ObjMesh.h"
#include <tiny_obj_loader.h>

#include <string>
#include "Engine/ShaderHandler.h"
#include "Engine/Material.h"
#include "Engine/MaterialHandler.h"

using namespace tinyobj;

void OBJMesh::Create()
{
	std::vector<shape_t> shapes;
	std::vector<material_t> materials;

	std::string dirPath = GetMaterialDirectory();

	std::string err = LoadObj(shapes, materials, m_filePath.c_str(), dirPath.c_str());

	if (err == "" && !shapes.empty())
	{
		CreateVAOFromShapes(shapes);
	}

	if (err == "" && !materials.empty())
	{
		CreateMaterialFromMaterials(materials);
	}
}


void OBJMesh::CreateVAOFromShapes(std::vector<shape_t> &shapes)
{
	//Only load the first shape for now
	m_uiNumberOfVerts = shapes[0].mesh.positions.size() / 3;
	m_uiNumberOfIndices = shapes[0].mesh.indices.size();

	Vertex* aoVertices = new Vertex[m_uiNumberOfVerts];

	unsigned int uiIndex = 0;
	unsigned int uiUVIndex = 0;
	std::vector<float>& positions = shapes[0].mesh.positions;
	std::vector<float>& normals = shapes[0].mesh.normals;
	std::vector<float>& texcoords = shapes[0].mesh.texcoords;
	for (unsigned int i = 0; i < m_uiNumberOfVerts; ++i)
	{
		aoVertices[i].position = glm::vec4(positions[uiIndex], positions[uiIndex + 1], positions[uiIndex + 2], 1.0f);
		aoVertices[i].colour = glm::vec4(1, 1, 1, 1);
		aoVertices[i].normal = glm::vec3(normals[uiIndex], normals[uiIndex + 1], normals[uiIndex + 2]);
		aoVertices[i].uv = glm::vec2(texcoords[uiUVIndex], 1 - texcoords[uiUVIndex + 1]);
		uiIndex += 3;
		uiUVIndex += 2;
	}

	std::vector<unsigned int>& indices = shapes[0].mesh.indices;

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glBufferData(GL_ARRAY_BUFFER, m_uiNumberOfVerts * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_uiNumberOfIndices * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + Vertex::PositionOffset);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + Vertex::ColourOffset);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + Vertex::NormalOffset);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + Vertex::TexCoord1Offset);


	// unbind vertex array
	glBindVertexArray(0);

	delete[] aoVertices;
}

void OBJMesh::CreateMaterialFromMaterials(std::vector<material_t>& materials)
{
	std::string dirPath = GetMaterialDirectory();

	//Load only the first material for now
	material_t& mat = materials[0];

	SetMaterial(MaterialHandler::Get()->CreateNewMaterial(&mat, dirPath));
	GetMaterial()->SetShader(ShaderHandler::Get()->LoadShader("OBJShader", "Shaders/OBJShader.vert", "Shaders/OBJShader.frag"));

}

std::string OBJMesh::GetMaterialDirectory()
{
	std::string dirPath;
	size_t last_slash_idx = m_filePath.rfind('\\');
	if (last_slash_idx == std::string::npos)
	{
		last_slash_idx = m_filePath.rfind('/');
	}

	if (std::string::npos != last_slash_idx)
	{
		dirPath = m_filePath.substr(0, last_slash_idx);
	}
	else
	{
		//No directory, use current
		dirPath = "";
	}

	return dirPath + "/";
}
