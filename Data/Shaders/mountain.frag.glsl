#version 400

in vec3 fColor;
uniform int windowHeight;

out vec4 outColor;

void main() {
   float shade = (gl_FragCoord.y / windowHeight);
   
   float baseShade = shade;
   baseShade *= 0.4;
   baseShade += 0.5;
   
   float otherShade = shade;
   otherShade *= 0.5;
   
   int pixelness = 1;
   float power = 200.0;
   
   float r = float(int(1.0 * otherShade * power) / pixelness) / power;
   float g = float(int(1.0 * otherShade * power) / pixelness) / power;
   float b = float(int(1.0 * otherShade * power) / pixelness) / power;
   
   vec4 col = vec4(r, g, b, 1);
   
   gl_FragColor = col;
}