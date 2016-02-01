in vec3 inPosition;
in vec2 inCoord;

uniform mat4 MVP;
uniform uint spriteCountX;
uniform uint spriteCountY;
uniform uint spriteCurrent;
uniform int mirrorTexture;

out vec2 texCoord;

void main() {
    gl_Position = MVP * vec4(inPosition, 1.0);

    texCoord = inCoord;
    texCoord.x /= spriteCountX;
    texCoord.y /= spriteCountY;

    uint x = spriteCurrent % spriteCountX;
    uint y = (spriteCurrent / spriteCountX) % spriteCountY + 1u;

    float shiftX = 1.0f / spriteCountX * x;
    float shiftY = -1.0f / spriteCountY * y;
    texCoord.x += shiftX;
    texCoord.y += shiftY;

    if (mirrorTexture != 0)
        texCoord = texCoord.st * vec2(-1.0, 1.0);
}
