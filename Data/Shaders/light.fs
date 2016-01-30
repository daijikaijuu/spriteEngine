in vec2 texCoord;

out vec4 outColor;

uniform vec3 lightColor;
uniform vec2 resolution;
uniform sampler2D gSampler;
uniform sampler2D lightmap;

void main() {
    vec2 a = gl_FragCoord.xy / resolution;
    vec4 t0 = texture(lightmap, a);
    vec4 t1 = texture(gSampler, texCoord);
    vec4 o = t0 + t1;
    //o *= vec4(lightColor, 0.1);
    //if (o.a > 0.9) discard;
    outColor = o;
}
