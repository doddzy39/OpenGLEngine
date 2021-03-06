#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Colour;
layout(location=2) in vec3 Normal;
layout(location=3) in vec2 TexCoord;
layout(location=4) in vec3 BiNormal;
layout(location=5) in vec3 Tangent;

out vec2 vTexCoord;
out vec4 vColour;
out vec3 vNormal;
out vec3 vBiNormal;
out vec3 vTangent;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main() 
{
	vTexCoord = TexCoord;
	vColour = Colour;

	mat4 PVM = Projection * View * Model;
	vNormal = mat3(Model) * Normal;
	vBiNormal = mat3(Model) * BiNormal;
	vTangent = mat3(Model) * Tangent;

	gl_Position= PVM * Position;
}