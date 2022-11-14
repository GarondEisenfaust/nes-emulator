#version 330 core

in vec3 vColor;

void main() { gl_FragColor = vec4(vColor.r, vColor.g, vColor.b, 1.0f); };
