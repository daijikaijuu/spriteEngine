#version 400

in vec2 texCoord;
out vec4 outColor;

uniform sampler2D gSampler;

void main() {
	vec4 tex = texture(gSampler, texCoord);
	vec4 c = texture2D(gSampler, texCoord);
	if (c.a < 0.7) discard;
	outColor = c;
}