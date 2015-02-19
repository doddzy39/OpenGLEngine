#version 410

in vec2 vTexCoord;
in vec4 vColour;
in vec3 vNormal;

out vec4 FragColor;

uniform vec4 diffuseMaterial;


uniform sampler2D diffuseTexture;
uniform sampler2D ambientTexture;

void main() 
{
	float alpha = texture(ambientTexture, vTexCoord).x;
	alpha = alpha * 0.5f;
	
	
    FragColor = vec4(texture(diffuseTexture, vTexCoord).xyz, alpha);
 };
