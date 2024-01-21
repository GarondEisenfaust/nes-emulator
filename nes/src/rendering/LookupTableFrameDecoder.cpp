#include "LookupTableFrameDecoder.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include <algorithm>

LookupTableFrameDecoder::LookupTableFrameDecoder()
    : mColorPalette(MakePixelColors()), mTexture(mTextureWidth, mTextureHeight, GL_NEAREST) {
  Shader fragmentShader(FragmentShader().source, GL_FRAGMENT_SHADER);
  Shader vertexShader(VertexShader().source, GL_VERTEX_SHADER);

  mShaderProgram.AttachShader(fragmentShader);
  mShaderProgram.AttachShader(vertexShader);
  mShaderProgram.Link();
  mShaderProgram.Use();
}

void LookupTableFrameDecoder::DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle) {
  std::transform(frameData, frameData + mTextureWidth * mTextureHeight, mTextureData.data(),
                 [this](uint16_t pixel) { return this->mColorPalette->at(pixel); });
  mTexture.UpdateData(reinterpret_cast<uint8_t*>(mTextureData.data()));
  Draw();
}

void LookupTableFrameDecoder::Draw() {
  mTexture.Bind();
  mSurface.Draw();
}
