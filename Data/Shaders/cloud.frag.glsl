#version 400

in vec2 texCoord;
out vec4 outColor;

uniform sampler2D gSampler;

void main() {
	vec4 c = texture2D(gSampler, texCoord);
	outColor = texture2D(gSampler, texCoord);
}