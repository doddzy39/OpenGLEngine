#version 410

layout(location=0) in vec3 Position;

out vec3 vTexCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main() 
{
	vec4 pos = Projection * mat4(mat3(View)) * Model * vec4(Position, 1.0); 
	gl_Position =  pos.xyww;
    vTexCoord = Position;
}