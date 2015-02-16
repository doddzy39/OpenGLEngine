#version 410

in vec2 vTexCoord;
in vec4 vColour;
in vec3 vNormal;

out vec4 FragColor;

uniform vec4 ambientMaterial;
uniform vec4 diffuseMaterial;
uniform vec4 specularMaterial;
uniform vec4 emmisiveMatieral;

uniform mat4 View;

uniform sampler2D diffuseTexture;
uniform sampler2D ambientTexture;

uniform vec3 LightDir = vec3(1, 0, 0);
uniform vec4 LightColour = vec4(1, 1, 1, 1);

void main() 
{
	
	float d = dot(normalize(vNormal), LightDir.xyz);

	vec4 dayColour = (LightColour * texture(diffuseTexture, vTexCoord) * d);
	vec4 nightColour = (texture(ambientTexture, vTexCoord) * (1 - d));

	vec4 textureDiffuseColour = dayColour + nightColour;

    FragColor = (ambientMaterial * vColour) + textureDiffuseColour;
 };
