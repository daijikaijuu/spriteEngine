#version 400

in vec2 vTexCoord;

uniform sampler2D texture;

out vec4 outColor;

void main() {
	vec4 c = texture2D(texture, vTexCoord);
	if (c.a < 0.9) discard;
    outColor = c;
}