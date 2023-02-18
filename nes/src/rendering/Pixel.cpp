#include "rendering/Pixel.h"
#include "Definitions.h"
#include <cstring>
#include <tuple>

Pixel::Pixel(float xPos, float yPos, float width, float height) : Pixel(xPos, yPos, width, height, PixelColor{0}) {}

Pixel::Pixel(float xPos, float yPos, float width, float height, PixelColor color)
    : mXPos(xPos), mYPos(yPos), mWidth(width), mHeight(height), mColor(color) {}

void Pixel::SetColor(PixelColor color) { mColor = color; }

PixelColor Pixel::GetColor() { return mColor; }

std::tuple<float, float, float, float> CalculateScreenSpace(float x, float y, float width, float height) {
  float screenX = (2 * x / WIDTH) - 1;
  float screenY = -((2 * y / HEIGHT) - 1);
  float screenWidth = 2 * width / WIDTH;
  float screenHeight = -2 * height / HEIGHT;
  return {screenX, screenY, screenWidth, screenHeight};
}

std::array<float, 18> Pixel::CalculateVertexCoordinates() {
  struct Vertex {
    float x;
    float y;
    float z;
  };

  Vertex topLeft{mXPos, mYPos, 0};
  Vertex topRight{mXPos + mWidth, mYPos, 0};
  Vertex bottomLeft{mXPos, mYPos + mHeight, 0};
  Vertex bottomRight{mXPos + mWidth, mYPos + mHeight, 0};

  std::array<float, 18> result;
  std::memcpy(result.begin() + 3 * 0, &bottomRight, sizeof(Vertex));
  std::memcpy(result.begin() + 3 * 1, &bottomLeft, sizeof(Vertex));
  std::memcpy(result.begin() + 3 * 2, &topLeft, sizeof(Vertex));
  std::memcpy(result.begin() + 3 * 3, &topLeft, sizeof(Vertex));
  std::memcpy(result.begin() + 3 * 4, &topRight, sizeof(Vertex));
  std::memcpy(result.begin() + 3 * 5, &bottomRight, sizeof(Vertex));

  return result;
}
