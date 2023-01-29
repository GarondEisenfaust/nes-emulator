#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 inColor;

out vec4 vColor;
uniform float width;
uniform float height;

void main() {
  float x = (2 * position.x / width) - 1;
  float y = -((2 * position.y / height) - 1);

  gl_Position = vec4(x, y, position.z, 1.0);
  vColor = inColor;
}
