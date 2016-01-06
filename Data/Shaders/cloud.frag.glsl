in vec2 texCoord;
out vec4 outColor;

uniform sampler2D gSampler;

void main() {
	outColor = texture2D(gSampler, texCoord);
}
