#version 330 core

in vec2 TexCoord;
out vec4 FragColor;
uniform usampler2D theTexture;

void main() { 
    FragColor = vec4(float(texture(theTexture, TexCoord).r)/255, 0.0, 0.0, 1.0); 
}