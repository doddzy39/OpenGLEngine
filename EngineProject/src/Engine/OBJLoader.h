#pragma once

/*!
 * \file OBJLoader..h
 *
 * \author Jackson Luff
 * \date February 2015
 *
 * 
 */

#include <vector>
#include <string>
#include "gl_core_4_4.h"
#include "glm\glm.hpp"

using namespace std;

struct ObjectData {
	vector<glm::vec3> s_vertices, s_normals, s_colours;
	vector<glm::vec2> s_texCoords;
	vector<GLuint> vIndices, uIndices, nIndices;
};

class OBJLoader
{
public:
	struct ObjVertex
	{
		glm::vec3 position;
		glm::vec3 colour;
		glm::vec3 normal;
	};

	OBJLoader(string objName, std::vector<ObjVertex>& outVertices);
	~OBJLoader();

	inline ObjectData* getModel() const { return m_object; }
	GLenum getRenderMode() const;
private:
	struct Material {
		string s_name;
		glm::vec3 s_colour;
	};

	vector< Material* > m_materials;
	Material *m_currentMaterial;
	ObjectData *m_object;
	bool hasUV, hasNormals, isQuads, indexChecked;

	string    parseString(string src, string code);
	glm::vec2 parseVec2(string src, string code);
	glm::vec3 parseVec3(string src, string code);

	void addIndices(string str);
	void checkIndices(string str);
	void loadObjects(string objPath);
	void loadMaterials(string matPath);
};

