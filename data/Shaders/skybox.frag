#version 410

in vec3 vTexCoord;

out vec4 fragColour;

uniform samplerCube cubeMap;

void main()
{    
    fragColour = texture(cubeMap, vTexCoord);
}