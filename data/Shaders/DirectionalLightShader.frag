#version 410

in vec2 vTexCoord;
in vec3 vNormal;
in vec4 vColour;

out vec4 FragColor;

uniform vec4 ambientMaterial;
uniform vec4 diffuseMaterial;
uniform vec4 specularMaterial;
uniform vec4 emmisiveMatieral;

uniform sampler2D diffuseTexture;

uniform vec3 LightDir = vec3(1, 0, 0);
uniform vec4 LightColour = vec4(1, 1, 1, 1);

void main() 
{
	float d = max(0, dot(vNormal, LightDir.xyz));

    FragColor = (ambientMaterial * vColour) + (LightColour * texture(diffuseTexture, vTexCoord) * d);
 };
