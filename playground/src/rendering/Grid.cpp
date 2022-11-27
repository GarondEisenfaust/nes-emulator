#include "rendering/Grid.h"
#include "Util.h"

Grid::Grid(int width, int height) : mWidth(width), mHeight(height) { mPixels.reserve(mWidth * mHeight); }

void Grid::Init() {
  const int gridWidth = 256;
  const int gridHeight = 240;

  const auto rectangleWidth = static_cast<float>(mWidth) / gridWidth;
  const auto rectangleHeight = static_cast<float>(mHeight) / gridHeight;

  for (auto y = 0; y < gridHeight; y++) {
    for (auto x = 0; x < gridWidth; x++) {
      const auto xPos = x * rectangleWidth;
      const auto yPos = y * rectangleHeight;
      mPixels.emplace_back(xPos, yPos, rectangleWidth, rectangleHeight);
    }
  }
}

Pixel& Grid::GetPixel(int x, int y) {
  auto position = x * y;
  return mPixels[position];
}

std::vector<float> Grid::MakePixelData() {
  std::vector<float> result;
  result.reserve(mPixels.size() * 18);
  for (auto& pixel : mPixels) {
    auto data = pixel.CalculateVertexCoordinates();
    std::copy(data.begin(), data.end(), std::back_inserter(result));
  }
  return result;
}

std::vector<float> Grid::MakeColorData() {
  std::vector<float> result;
  for (auto& pixel : mPixels) {
    auto color = pixel.GetColor();
    for (auto i = 0; i < 6; i++) {
      result.push_back(color.r);
      result.push_back(color.g);
      result.push_back(color.b);
      result.push_back(color.a);
    }
  }

  return result;
}

void Grid::UpdateColor() {
  for (auto& pixel : mPixels) {
    auto r = Util::GetRandomFloat();
    auto g = Util::GetRandomFloat();
    auto b = Util::GetRandomFloat();
    auto a = Util::GetRandomFloat();
    PixelColor color{r, g, b, a};
    pixel.SetColor(color);
  }
}
