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

	CreateVertexArrayFromShapes(shapes, aoVertices);

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
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + Vertex::PositionOffset);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + Vertex::ColourOffset);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + Vertex::NormalOffset);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + Vertex::TexCoord1Offset);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + Vertex::BinormalOffset);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + Vertex::TangentOffset);


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



void OBJMesh::CreateVertexArrayFromShapes(std::vector<shape_t> &shapes, Vertex* aoVertices)
{
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

	CalculateBinormalsAndTangents(aoVertices, shapes[0].mesh.indices);

}

void OBJMesh::CalculateBinormalsAndTangents(std::vector<float>& positions, std::vector<float>& normals, std::vector<float>& texcoords, std::vector<float>& binormals, std::vector<float>& tangents)
{

	for (unsigned int i = 0, j = 0; i < positions.size(); i += 9, j += 6){

		// Shortcuts for vertices
		glm::vec3 & v0 = glm::vec3(
			positions[i + 0],
			positions[i + 1],
			positions[i + 2]);

		glm::vec3 & v1 = glm::vec3(
			positions[i + 3],
			positions[i + 4],
			positions[i + 5]);

		glm::vec3 & v2 = glm::vec3(
			positions[i + 6],
			positions[i + 7],
			positions[i + 8]);

		// Shortcuts for UVs
		glm::vec2 & uv0 = glm::vec2(
			texcoords[j + 0],
			texcoords[j + 1]);

		glm::vec2 & uv1 = glm::vec2(
			texcoords[j + 2],
			texcoords[j + 3]);


		glm::vec2 & uv2 = glm::vec2(
			texcoords[j + 4],
			texcoords[j + 5]);

		// Edges of the triangle : position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 t = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bi = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		for (int k = 0; k < 3; ++k)
		{
			tangents.push_back(t.x);
			tangents.push_back(t.y);
			tangents.push_back(t.z);
		}

		for (int k = 0; k < 3; ++k)
		{
			binormals.push_back(bi.x);
			binormals.push_back(bi.y);
			binormals.push_back(bi.z);
		}
	}
}

void OBJMesh::CalculateBinormalsAndTangents(Vertex* aoVertices, std::vector<unsigned int>& indices)
{
	for (int index = 0; index < indices.size(); index += 3)
	{
		int i0 = indices[index];
		int i1 = indices[index + 1];
		int i2 = indices[index + 2];

		glm::vec3 v0 = aoVertices[i0].position.xyz;
		glm::vec3 v1 = aoVertices[i1].position.xyz;
		glm::vec3 v2 = aoVertices[i2].position.xyz;

		glm::vec2 uv0 = aoVertices[i0].uv;
		glm::vec2 uv1 = aoVertices[i1].uv;		
		glm::vec2 uv2 = aoVertices[i2].uv;

		// Edges of the triangle : position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 t = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bi = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		aoVertices[i0].binormal = bi;
		aoVertices[i1].binormal = bi;
		aoVertices[i2].binormal = bi;
	
		aoVertices[i0].tangent = t;
		aoVertices[i1].tangent = t;
		aoVertices[i2].tangent = t;
	}
}
