in vec2 vTexCoord;

uniform sampler2D gSampler;

out vec4 outColor;

void main() {
	vec4 c = texture2D(gSampler, vTexCoord);
	if (c.a < 0.9) discard;
    outColor = c;
}
