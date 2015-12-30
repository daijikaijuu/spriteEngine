#version 400

in vec2 texCoord;
out vec4 outColor;

uniform sampler2D gSampler;

void main() {
	vec4 tex = texture(gSampler, texCoord);
	if (tex.a < 0.2) discard;
	outColor = texture2D(gSampler, texCoord);
}