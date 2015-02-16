#version 410

in vec2 vTexCoord;
in vec4 vColour;
in vec3 vNormal;
in vec3 vBiNormal;
in vec3 vTangent;

out vec4 FragColor;

uniform vec4 ambientMaterial;
uniform vec4 diffuseMaterial;
uniform vec4 specularMaterial;
uniform vec4 emmisiveMatieral;

uniform mat4 Model;

uniform sampler2D diffuseTexture;
uniform sampler2D ambientTexture;
uniform sampler2D normalTexture;

uniform vec3 LightDir = vec3(1, 0, 0);
uniform vec4 LightColour = vec4(1, 1, 1, 1);

void main() 
{
	mat3 TBN = mat3(
			normalize( vTangent ),
			normalize( vBiNormal ),
			normalize( vNormal ));

	vec3 N = texture(normalTexture, vTexCoord).xyz * 2 - 1;
	vec3 ModelN = normalize( TBN * N );
	
	vec3 lightDir_modelSpace = inverse(mat3(Model)) * LightDir;

	float d = max(0.0, dot(ModelN, normalize( lightDir_modelSpace )));

	vec4 dayColour = (LightColour * texture(diffuseTexture, vTexCoord) * d);
	vec4 nightColour = (texture(ambientTexture, vTexCoord) * (1 - d));
	
	vec4 textureDiffuseColour = dayColour + nightColour;

    FragColor = (ambientMaterial * vColour) + textureDiffuseColour;
 };
