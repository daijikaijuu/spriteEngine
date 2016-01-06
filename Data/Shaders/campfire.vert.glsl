in vec3 inPosition;
in vec2 inCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelview;
uniform int spriteCount;

uniform float texShift;

out vec2 vTexCoord;

void main() {
	gl_Position = projectionMatrix * modelview * vec4(inPosition, 1.0);

	vTexCoord = inCoord;
	vTexCoord.x /= spriteCount;
	vTexCoord.x += texShift;
}
