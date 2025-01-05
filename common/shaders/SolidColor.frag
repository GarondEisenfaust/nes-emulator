#version 330 core

uniform vec4 color;
uniform bool highlight;
out vec4 FragColor;

void main() {
  FragColor = color;
  if (highlight) {
    FragColor *= 2.0;
  }
}
