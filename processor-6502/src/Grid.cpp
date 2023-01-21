#include "Grid.h"
#include "Util.h"

Grid::Grid(int width, int height, int gridWidth, int gridHeight)
    : mWidth(width), mHeight(height), mGridWidth(gridWidth), mGridHeight(gridHeight) {
  mPixels.reserve(mWidth * mHeight);
}

void Grid::Init() {
  const auto rectangleWidth = static_cast<float>(mWidth) / mGridWidth;
  const auto rectangleHeight = static_cast<float>(mHeight) / mGridHeight;

  for (auto y = 0; y < mGridHeight; y++) {
    for (auto x = 0; x < mGridWidth; x++) {
      const auto xPos = x * rectangleWidth;
      const auto yPos = y * rectangleHeight;
      mPixels.emplace_back(xPos, yPos, rectangleWidth, rectangleHeight);
    }
  }
}

Pixel& Grid::GetPixel(int x, int y) {
  auto position = y * mGridWidth + x;
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

int Grid::GetGridWidth() { return mGridWidth; }
int Grid::GetGridHeight() { return mGridHeight; }
