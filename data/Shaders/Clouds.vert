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

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;	


void main() 
{
	vTexCoord = TexCoord;
	vColour = Colour;
	vNormal = mat3(Model) * Normal;

	mat4 PVM = Projection * View * Model;
	gl_Position= PVM * Position;
}