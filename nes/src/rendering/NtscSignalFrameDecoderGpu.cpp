#include "GL/glew.h"
#include "NtscSignalFrameDecoderGpu.h"
#include "NtscDecoderShader.h"
#include "VertexShader.h"

NtscSignalFrameDecoderGpu::NtscSignalFrameDecoderGpu() : mTexture(mTextureWidth, mTextureHeight, GL_NEAREST) {
  Shader fragmentShader(NtscDecoderShader().source, GL_FRAGMENT_SHADER);
  Shader vertexShader(VertexShader().source, GL_VERTEX_SHADER);

  mShaderProgram.AttachShader(fragmentShader);
  mShaderProgram.AttachShader(vertexShader);
  mShaderProgram.Link();
  mShaderProgram.Use();
}

void NtscSignalFrameDecoderGpu::DecodeAndDraw(uint16_t* frameData, unsigned int ppuCycle) {
  mTexture.UpdateData(frameData);
  Draw();
}

void NtscSignalFrameDecoderGpu::Draw() {
  mTexture.Bind();
  mSurface.Draw();
}
