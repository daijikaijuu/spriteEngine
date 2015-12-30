#version 400

in vec3 pos;
in vec3 color;

uniform mat4 projection;
uniform mat4 modelview;

out vec3 fColor;

void main() {
	gl_Position = projection * modelview * vec4(pos, 1.0);
	fColor = color;
}