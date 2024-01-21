#version 330 core

in vec2 TexCoord;
out vec4 FragColor;
uniform usampler2D theTexture;

const int samplesToGenerate = 12;
const int samplesToTake = 12;
const float pi = 3.14159265358979323846;
const float black = 0.312f;
const float white = 1.100f;
const float levels[16] = float[16](0.228f, 0.312f, 0.552f, 0.880f, 0.616f, 0.840f, 1.100f, 1.100f, 0.192f, 0.256f,
                                   0.448f, 0.712f, 0.500f, 0.676f, 0.896f, 0.896f);
const mat3 yiqToRgbMatrix = mat3(1, 1, 1, 0.946882, -0.274788, -1.108545, 0.623557, -0.635691, 1.709007);

bool InColorPhase(int color, int phase) { return (color + phase + samplesToGenerate) % samplesToTake < 6; }
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

  int offset = samplesToTake / 2;
  vec3 yiq = vec3(0);
  for (int i = 0; i < samplesToGenerate; i++) {
    float level = NtscSignal(pixel, i) / samplesToGenerate;
    yiq += vec3(1, cos((pi / offset) * (i - 8)), sin((pi / offset) * (i - 8))) * level;
  }

  vec3 rgb = yiqToRgbMatrix * yiq;

  FragColor = vec4(rgb, 1.0);
}