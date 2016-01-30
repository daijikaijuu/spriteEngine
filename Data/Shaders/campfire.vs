in vec3 inPosition;
in vec2 inCoord;

uniform mat4 MVP;
uniform int spriteCount;

uniform float texShift;

out vec2 texCoord;

void main() {
    gl_Position = MVP * vec4(inPosition, 1.0);

    texCoord = inCoord;
    texCoord.x /= spriteCount;
    texCoord.x += texShift;
}
