in vec2 texCoord;
out vec4 outColor;

uniform float alpha;
uniform float shiftX;
uniform sampler2D gSampler;

void main() {
    vec4 c = texture(gSampler, vec2(texCoord.x + shiftX, texCoord.y));
    if (c.a < alpha) discard;
    outColor = c;
}
