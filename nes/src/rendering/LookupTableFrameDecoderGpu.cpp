#include <glad/glad.h>
#include "LookupTableFrameDecoderGpu.h"
#include "LookupShader.h"
#include "VertexShader.h"
#include <algorithm>

LookupTableFrameDecoderGpu::LookupTableFrameDecoderGpu()
    : mColorPalette(MakePixelColorsFloat()), mTexture(mTextureWidth, mTextureHeight, GL_NEAREST) {
  Shader fragmentShader(LookupShader().source, GL_FRAGMENT_SHADER);
  Shader vertexShader(VertexShader().source, GL_VERTEX_SHADER);

  mShaderProgram.AttachShader(fragmentShader);
  mShaderProgram.AttachShader(vertexShader);
  mShaderProgram.Link();
  mShaderProgram.Use();
  mShaderProgram.SetUniform("colorPalette", *mColorPalette);
}

void LookupTableFrameDecoderGpu::DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle) {
  mTexture.UpdateData(reinterpret_cast<uint16_t*>(frameData));
  Draw();
}

void LookupTableFrameDecoderGpu::Draw() {
  mTexture.Bind();
  mSurface.Draw();
}
