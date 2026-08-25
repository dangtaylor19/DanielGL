#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aSpriteCoord;
layout (location = 2) in vec3 aNormal;

out vec2 textureCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * transform * vec4(aPos, 1.0);
	textureCoord = aSpriteCoord;

	fragPos = vec3(transform * vec4(aPos, 1.0));
	normal = mat3(transpose(inverse(transform))) * aNormal;  
}