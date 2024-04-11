#version 330 core

in vec2 TexCoord;
out vec4 FragColor;
uniform usampler2D theTexture;
uniform vec4 colorPalette[64];

void main() {
  uint nesColor = texture(theTexture, TexCoord).r;
  FragColor = colorPalette[nesColor];
}
