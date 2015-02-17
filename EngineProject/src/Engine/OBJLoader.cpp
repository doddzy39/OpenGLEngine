#include <fstream>
#include <vector>
#include <string>
#include "glm\glm.hpp"
#include "OBJLoader.h"

//Here's a better C version (from Google's Chromium project):
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

using namespace std;

OBJLoader::OBJLoader(string objName, std::vector<ObjVertex>& outVertices) {
	indexChecked = false;
	isQuads = false;

	string fileName = "Objects/" + objName;
	loadMaterials(fileName + ".mtl");
	loadObjects(fileName + ".obj");

	for (GLuint i = 0; i < m_object->s_vertices.size(); ++i)
	{
		ObjVertex vert = ObjVertex();

		if (!hasUV && !hasNormals)
		{
			vert.position = m_object->s_vertices[i];
			vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			vert.colour = glm::vec3(0.5f, 0.5f, 0.5f);
			outVertices.push_back(vert);
		}
		else if (hasUV && !hasNormals)
		{
			vert.position = m_object->s_vertices[i];
			vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			vert.colour = glm::vec3(0.5f, 0.5f, 0.5f);
			outVertices.push_back(vert);
		}
		else if (!hasUV && hasNormals)
		{
			vert.position = m_object->s_vertices[i];
			vert.normal = m_object->s_normals[i];
			vert.colour = glm::vec3(0.5f, 0.5f,  0.5f);
			outVertices.push_back(vert);
		}
		else if (hasUV && hasNormals)
		{
			vert.position = m_object->s_vertices[i];
			vert.normal = m_object->s_normals[i];
			vert.colour = glm::vec3(0.5f, 0.5f, 0.5f);
			outVertices.push_back(vert);
		}
	}
}

OBJLoader::~OBJLoader()
{
	delete m_currentMaterial;
	delete m_object;
	m_materials.clear();
}

string OBJLoader::parseString(string src, string code) {
	char buffer[64];
	string scanStr = code + " %s";
	sscanf_s(src.c_str(), scanStr.c_str(), &buffer, _countof(buffer));
	return string(buffer);
}

glm::vec2 OBJLoader::parseVec2(string src, string code) {
	float x, y;
	string scanStr = code + " %f %f";
	sscanf_s(src.c_str(), scanStr.c_str(), &x, &y);
	return glm::vec2(x, y);
}

glm::vec3 OBJLoader::parseVec3(string src, string code) {
	float x, y, z;
	string scanStr = code + " %f %f %f";
	sscanf_s(src.c_str(), scanStr.c_str(), &x, &y, &z);
	return glm::vec3(x, y, z);
}

void OBJLoader::addIndices(string str) {
	int v1 = -1, u1 = -1, n1 = -1,
		v2 = -1, u2 = -1, n2 = -1,
		v3 = -1, u3 = -1, n3 = -1,
		v4 = -1, u4 = -1, n4 = -1;

	checkIndices(str);

	if (!hasUV && !hasNormals) {
		sscanf_s(str.c_str(), "f %d %d %d %d",
			&v1,
			&v2,
			&v3,
			&v4);
	}
	else if (hasUV && !hasNormals) {
		sscanf_s(str.c_str(), "f %d/%d %d/%d %d/%d %d/%d",
			&v1, &u1,
			&v2, &u2,
			&v3, &u3,
			&v4, &u4);
	}
	else if (!hasUV && hasNormals) {
		sscanf_s(str.c_str(), "f %d//%d %d//%d %d//%d %d//%d",
			&v1, &n1,
			&v2, &n2,
			&v3, &n3,
			&v4, &n4);
	}
	else if (hasUV && hasNormals) {
		sscanf_s(str.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
			&v1, &u1, &n1,
			&v2, &u2, &n2,
			&v3, &u3, &n3,
			&v4, &u4, &n4);
	}

	v1--; u1--; n1--;
	v2--; u2--; n2--;
	v3--; u3--; n3--;
	v4--; u4--; n4--;

	if (v3 >= 0 && !isQuads) {
		m_object->vIndices.push_back(v1);
		m_object->vIndices.push_back(v2);
		m_object->vIndices.push_back(v3);
		if (v4 >= 0) {
			m_object->vIndices.push_back(v4);
		}
	}
	else if (isQuads)
	{
		m_object->vIndices.push_back(v1);
		m_object->vIndices.push_back(v2);
		m_object->vIndices.push_back(v3);
		m_object->vIndices.push_back(v3);
		m_object->vIndices.push_back(v4);
		m_object->vIndices.push_back(v1);
	}

	if (u3 >= 0) {
		m_object->uIndices.push_back(u1);
		m_object->uIndices.push_back(u2);
		m_object->uIndices.push_back(u3);
		if (u4 >= 0) {
			m_object->uIndices.push_back(u4);
		}
	}

	if (n3 >= 0) {
		m_object->nIndices.push_back(n1);
		m_object->nIndices.push_back(n2);
		m_object->nIndices.push_back(n3);
		if (n4 >= 0) {
			m_object->nIndices.push_back(n4);
		}
	}
}

void OBJLoader::checkIndices(string str) {

	if (!indexChecked) {
		int bestSample = 0;
		int sampleV1 = -1, sampleU1 = -1, sampleN1 = -1;
		int tmp = -1, sampleV4 = -1;

		int form1 = sscanf(str.c_str(), "f %d", &sampleV1);
		int form2 = sscanf(str.c_str(), "f %d/%d", &sampleV1, &sampleU1);
		int form3 = sscanf(str.c_str(), "f %d//%d", &sampleV1, &sampleN1);
		int form4 = sscanf(str.c_str(), "f %d/%d/%d", &sampleV1, &sampleU1, &sampleN1);

		if (form1 > bestSample) {
			bestSample = form1;
			hasUV = false;
			hasNormals = false;
		}

		if (form2 > bestSample) {
			bestSample = form2;
			hasUV = true;
			hasNormals = false;
		}

		if (form3 > bestSample) {
			bestSample = form3;
			hasUV = false;
			hasNormals = true;
		}

		if (form4 > bestSample) {
			bestSample = form4;
			hasUV = true;
			hasNormals = true;
			isQuads = true;
		}

		indexChecked = true;
	}
}

void OBJLoader::loadObjects(string objPath) {
	ifstream file(objPath);
	string buffer;
	m_object = new ObjectData();
	m_currentMaterial = m_materials[0];

	if (file.is_open()) {
		while (!file.eof()) {
			getline(file, buffer);

			if (buffer.find("v ") == 0) {
				m_object->s_vertices.push_back(parseVec3(buffer, "v "));
				m_object->s_colours.push_back(m_currentMaterial->s_colour);
			}
			else if (buffer.find("vn ") == 0) {
				m_object->s_normals.push_back(parseVec3(buffer, "vn "));
			}
			else if (buffer.find("vt ") == 0) {
				m_object->s_texCoords.push_back(parseVec2(buffer, "vt "));
			}
			else if (buffer.find("usemtl ") == 0) {
				string name = parseString(buffer, "usemtl ");
				for (GLuint i = 0; i < m_materials.size(); i++) {
					if (name == m_materials[i]->s_name)
						m_currentMaterial = m_materials[i];
				}
			}
			else if (buffer.find("f ") == 0)
				addIndices(buffer);
			else if (buffer.find("# ") == 0)
				printf("%s \n", buffer.c_str());
		}

		printf("Object Load: successful...");
	}
	else
		printf("Unable to find: %s\n", objPath.c_str());
}

void OBJLoader::loadMaterials(string matPath) {
	ifstream file(matPath);
	string buffer;
	m_currentMaterial = NULL;

	if (file.is_open()) {
		while (!file.eof()) {
			getline(file, buffer);

			if (buffer.find("newmtl") == 0) {
				if (m_currentMaterial != NULL)
					m_materials.push_back(m_currentMaterial);

				m_currentMaterial = new Material();
				m_currentMaterial->s_name = parseString(buffer, "newmtl");
			}
			else if (buffer.find("Kd") == 0)
				m_currentMaterial->s_colour = parseVec3(buffer, "Kd");
		}
	}
	else printf("Unable to find path: %s\n", matPath.c_str());

	if (m_currentMaterial == NULL) {
		m_currentMaterial = new Material();
		m_currentMaterial->s_colour = glm::vec3(1, 1, 1);
	}

	m_materials.push_back(m_currentMaterial);
}