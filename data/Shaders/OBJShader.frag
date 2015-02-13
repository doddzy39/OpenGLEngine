#version 410

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D diffuseTexture;

void main() 
{
    FragColor = texture(diffuseTexture, vTexCoord);
 };
