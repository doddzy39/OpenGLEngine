#version 410

in vec4 vPosition;
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

uniform vec4 EyePos;
uniform mat4 Model;

uniform sampler2D diffuseTexture;
uniform sampler2D ambientTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

uniform vec3 LightDir = vec3(1, 0, 0);

uniform vec4 LightDiffuseColour = vec4(1, 1, 1, 1);
uniform vec4 LightAmbientColour = vec4(1, 1, 1, 1);
uniform vec4 LightSpecularColour = vec4(0.3, 0.3, 0.3, 1);

uniform float LightSpecularPower = 5.0;

void main() 
{
	mat3 TBN = mat3(
			normalize( vTangent ),
			normalize( vBiNormal ),
			normalize( vNormal ));

	vec3 N = texture(normalTexture, vTexCoord).xyz * 2 - 1;
	vec3 normal_modelSpace = normalize( TBN * N );
	
	vec3 lightDir_modelSpace = inverse(mat3(Model)) * LightDir;

	float d = max(0.0, dot(normal_modelSpace, normalize( lightDir_modelSpace )));

	vec4 dayColour = (LightDiffuseColour * texture(diffuseTexture, vTexCoord) * d);
	vec4 nightColour = (texture(ambientTexture, vTexCoord) * (1 - d));
	
	vec4 textureDiffuseColour = dayColour + nightColour;
	
	//Eye to model
	vec3 eyeDir = normalize(vec3(EyePos - vPosition));
	
	vec3 negLightDir = normalize(-LightDir);
	vec3 lightReflect = normalize(reflect(negLightDir, mat3(Model) * vNormal));
	
	float specDot = max(0.0, dot(lightReflect, eyeDir));
	float specVal = texture(specularTexture, vTexCoord).x;
	
	vec4 finalAmbient = (ambientMaterial * LightAmbientColour * vColour);
	vec4 finalDiffuse = diffuseMaterial * textureDiffuseColour;
	vec4 finalSpecularColour = (LightSpecularColour) * pow(specDot, LightSpecularPower) * specVal;
    //finalAmbient + textureDiffuseColour + 
	FragColor = finalAmbient + finalDiffuse + finalSpecularColour;
 };
