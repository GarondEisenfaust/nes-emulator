#version 330 core

in vec3 vColor;
out vec4 color;

void main() { color = vec4(vColor.x, vColor.y, vColor.z, 1.0f); };
