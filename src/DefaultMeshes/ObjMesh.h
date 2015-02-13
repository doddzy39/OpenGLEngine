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
		};

		glm::vec4 position;
		glm::vec4 colour;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	OBJMesh(std::string filePath) { m_filePath = filePath;  Create(); }

	void Create();


private:
	void CreateVAOFromShapes(std::vector<tinyobj::shape_t> &shapes);
	void CreateMaterialFromMaterials(std::vector<tinyobj::material_t> &materials);

	std::string GetMaterialDirectory();

	std::string m_filePath;
};

#endif