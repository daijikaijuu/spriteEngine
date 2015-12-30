#version 400

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;

uniform mat4 projectionMatrix;

out vec2 texCoord;

void main() {
	gl_Position = projectionMatrix * vec4(inPosition, 1.0);
	texCoord = inCoord;
}