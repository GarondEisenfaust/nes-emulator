#include "rendering/Grid.h"

Grid::Grid(int gridWidth, int gridHeight) : mGridWidth(gridWidth), mGridHeight(gridHeight), mFrameComplete(false) {}

int Grid::GetGridWidth() { return mGridWidth; }
int Grid::GetGridHeight() { return mGridHeight; }

void Grid::SetPixelColor(int x, int y, PixelColor& color) {
  auto width = GetGridWidth();
  auto height = GetGridHeight();

  if ((0 <= x && x < width) && (0 <= y && y < height)) {
    mTextureData[mCurrentPixel] = color;
    mCurrentPixel = (mCurrentPixel + 1) % (256 * 240);
  }
}

void Grid::CommitFrame() { mFrameComplete = true; }
void Grid::StartNewFrame() { mFrameComplete = false; }
bool Grid::FrameComplete() { return mFrameComplete; }
