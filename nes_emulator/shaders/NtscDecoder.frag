#version 330 core

in vec2 TexCoord;
out vec4 FragColor;
uniform isampler2D theTexture;
uniform int samplesToGenerate;
uniform int samplesToTake;
uniform float hueFix;

const uint MAX_SAMPLES_TO_GENERATE = 12u;
const float PI = 3.14159265358979323846;
const float BLACK = 0.312f;
const float WHITE = 1.100f;
const float LEVELS[16] = float[16](0.228f, 0.312f, 0.552f, 0.880f, 0.616f, 0.840f, 1.100f, 1.100f, 0.192f, 0.256f,
                                   0.448f, 0.712f, 0.500f, 0.676f, 0.896f, 0.896f);
const mat3 YIQ_TO_RGB_MATRIX = mat3(1, 1, 1, 0.946882, -0.274788, -1.108545, 0.623557, -0.635691, 1.709007);

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

  float low = LEVELS[0 + level];
  float high = LEVELS[4 + level];
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

  return (signal - BLACK) / (WHITE - BLACK);
}

void main() {
  int pixel = texture(theTexture, TexCoord).r;
  int previousPixel = texture(theTexture, TexCoord).g;
  int ppuCycle = texture(theTexture, TexCoord).b % samplesToTake * samplesToGenerate;
  int index = texture(theTexture, TexCoord).a;

  float samples[MAX_SAMPLES_TO_GENERATE];
  int sampleIndex = 0;
  int difference = samplesToTake - samplesToGenerate;

  for (int i = index - difference; i < index; i++) {
    samples[sampleIndex] = NtscSignal(previousPixel, i + ppuCycle);
    sampleIndex++;
  }
  for (int i = index; i < index + samplesToGenerate; i++) {
    samples[sampleIndex] = NtscSignal(pixel, i + ppuCycle);
    sampleIndex++;
  }

  vec3 yiq = vec3(0);
  int offset = samplesToTake / 2;
  for (int i = 0; i < samplesToTake; i++) {
    float level = samples[i] / samplesToTake;
    int phase = index - difference + i;
    float x = (PI / offset) * (phase + hueFix + ppuCycle);
    yiq += vec3(1, cos(x), sin(x)) * level;
  }

  vec3 rgb = YIQ_TO_RGB_MATRIX * yiq;
  FragColor = vec4(rgb, 1.0);
}