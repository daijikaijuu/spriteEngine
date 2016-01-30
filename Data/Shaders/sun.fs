in vec2 texCoord;

out vec4 outColor;

uniform sampler2D gSampler;

void main() {
    outColor = vec4(1.0, 0.0, 0.0, 0.0);//texture(gSampler, texCoord) * vec4(1, 1, 0, 1);
}
