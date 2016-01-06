in vec3 pos;

in vec3 color;

uniform mat4 projection;

out vec3 fColor;
out vec2 texCoordV;

void main() {
	gl_Position = projection * vec4(pos, 1.0);
	fColor = color;
}
