#ifndef _FBX_MESH_H_
#define _FBX_MESH_H_

#include "Engine/Renderable.h"
#include "Engine/gl_core_4_4.h"
#include <GLFW/glfw3.h>

#include  <string>
#include "tiny_obj_loader.h"

class OBJMesh : public Renderable
{
public:
	struct Vertex
	{
		enum Offsets
		{
			PositionOffset = 0,
			ColourOffset = PositionOffset + sizeof(glm::vec4),
			NormalOffset = ColourOffset + sizeof(glm::vec4),
			TexCoord1Offset = NormalOffset + sizeof(glm::vec3),
			BinormalOffset = TexCoord1Offset + sizeof(glm::vec2),
			TangentOffset = BinormalOffset + sizeof(glm::vec3),
		};

		glm::vec4 position;
		glm::vec4 colour;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec3 binormal;
		glm::vec3 tangent;
	};

	OBJMesh(std::string filePath) { m_filePath = filePath;  Create(); }

	void Create();


private:
	void CreateVAOFromShapes(std::vector<tinyobj::shape_t> &shapes);

	void CreateVertexArrayFromShapes(std::vector<tinyobj::shape_t> &shapes, Vertex* aoVertices);

	void CreateMaterialFromMaterials(std::vector<tinyobj::material_t> &materials);

	std::string GetMaterialDirectory();
	void CalculateBinormalsAndTangents(std::vector<float>& positions, std::vector<float>& normals, std::vector<float>& texcoords, std::vector<float>& binormals, std::vector<float>& tangents);
	void CalculateBinormalsAndTangents(Vertex* aoVertices, std::vector<unsigned int>& indices);

	std::string m_filePath;
};

#endif