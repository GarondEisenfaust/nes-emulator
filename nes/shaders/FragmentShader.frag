#version 330 core

in vec2 TexCoord;
out vec4 FragColor;
uniform usampler2D theTexture;

#define M_PI 3.1415926535897932384626433832795
const mat3 rgbMatrix = mat3(vec3(1, 1, 1), vec3(0.946882, -0.274788, -1.108545), vec3(0.623557, -0.635691, 1.709007));
const float black = 0.312f;
const float white = 1.100f;
const float levels[16] = float[16](0.228f, 0.312f, 0.552f, 0.880f, 0.616f, 0.840f, 1.100f, 1.100f, 0.192f, 0.256f,
                                   0.448f, 0.712f, 0.500f, 0.676f, 0.896f, 0.896f);

bool InColorPhase(int color, int phase) { return (color + phase + 12) % 12 < 6; }
bool BitIsSet(int value, int bitNum) { return (value & bitNum) > 0; }

float NtscSignal(int pixel, int phase) {
  int color = (pixel & 0x0F);
  int level = (pixel >> 4) & 3;
  int emphasis = (pixel >> 6);
  if (color > 13) {
    level = 1;
  }

  int attenuation = 0;
  if ((BitIsSet(emphasis, 1) && InColorPhase(0, phase)) || (BitIsSet(emphasis, 2) && InColorPhase(4, phase)) ||
      (BitIsSet(emphasis, 4) && InColorPhase(8, phase)) && (color > 0xE)) {
    attenuation = 0;
  }

  float low = levels[0 + level];
  float high = levels[4 + level];
  if (color == 0) {
    low = high;
  }
  if (color > 12) {
    high = low;
  }

  float signal = low;
  if (InColorPhase(color, phase)) {
    signal = high;
  }

  return (signal - black) / (white - black);
}

void main() {
  int pixel = int(texture(theTexture, TexCoord).r);

  vec3 signal = vec3(0);
  for (int i = 0; i < 12; i++) {
    float level = NtscSignal(pixel, i) / 12;
    signal += vec3(1, cos((M_PI / 6) * (i - 8)), sin((M_PI / 6) * (i - 8))) * level;
  }

  vec3 asRgb = rgbMatrix * signal;

  FragColor = vec4(asRgb, 1.0);
}