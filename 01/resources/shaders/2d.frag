#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture1;
uniform mat4 texMat;

void main() {
  vec4 texCoords = texMat * vec4(TexCoords, 0.0, 1.0);
  vec4 texColor = texture(texture1, texCoords.xy);
  color = texColor;
}