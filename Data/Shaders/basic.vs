in vec3 inPosition;
in vec2 inCoord;
in vec3 inColor;

uniform mat4 MVP;

out vec2 texCoord;
out vec3 fragColor;

void main() {
    gl_Position = MVP * vec4(inPosition, 1.0);
    texCoord = inCoord;
    fragColor = inColor;
}
