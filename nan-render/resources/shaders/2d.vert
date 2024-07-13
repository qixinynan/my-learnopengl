#version 330 core
layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 local;
uniform mat4 projection;
uniform mat4 view;

void main() {
  TexCoords = vertex.zw;
  gl_Position = projection * view * local * vec4(vertex.xy, 0.0, 1.0);
}