in vec3 inPosition;
in vec2 inCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelview;

out vec2 texCoord;

void main() {
	gl_Position = projectionMatrix * modelview * vec4(inPosition, 1.0);
	texCoord = inCoord;
}
