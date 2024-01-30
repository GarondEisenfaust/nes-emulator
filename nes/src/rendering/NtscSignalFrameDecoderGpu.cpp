#include "GL/glew.h"
#include "NtscSignalFrameDecoderGpu.h"
#include "NtscDecoderShader.h"
#include "VertexShader.h"

NtscSignalFrameDecoderGpu::NtscSignalFrameDecoderGpu()
    : mCurrentPixelTexture(mTextureWidth, mTextureHeight, GL_NEAREST) {
  Shader fragmentShader(NtscDecoderShader().source, GL_FRAGMENT_SHADER);
  Shader vertexShader(VertexShader().source, GL_VERTEX_SHADER);

  mShaderProgram.AttachShader(fragmentShader);
  mShaderProgram.AttachShader(vertexShader);
  mShaderProgram.Link();

  mShaderProgram.Use();
  mShaderProgram.SetUniform("samplesToGenerate", mSamplesToGeneratePerPixel);
  mShaderProgram.SetUniform("samplesToTake", mSamplesToTakePerPixel);
  mShaderProgram.SetUniform("hueFix", mHueFix);
}

void NtscSignalFrameDecoderGpu::DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle) {
  for (int y = 0; y < mTextureHeight; y++) {
    for (int x = 0; x < mTextureWidth; x++) {
      const int i = (y * mTextureWidth) + x;

      auto& thePixel = mPixels[i];
      thePixel.current = frameData[i];
      thePixel.index = i;
      if (i % mTextureWidth == 0) {
        thePixel.previous = thePixel.current;
      } else {
        thePixel.previous = frameData[i - 1];
      }
      thePixel.cycle = ppuCycle;
      ppuCycle++;
    }
    ppuCycle += mCyclesForScanline - mTextureWidth;
  }

  mCurrentPixelTexture.UpdateData(reinterpret_cast<uint16_t*>(mPixels.data()));
  Draw();
}

void NtscSignalFrameDecoderGpu::Draw() {
  mCurrentPixelTexture.Bind();
  mSurface.Draw();
}
