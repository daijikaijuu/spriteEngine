in vec2 texCoord;
out vec4 outColor;

uniform float alpha;
uniform sampler2D gSampler;

void main() {
    vec4 c = texture(gSampler, texCoord);
    if (c.a < alpha) discard;
    outColor = c;
}
