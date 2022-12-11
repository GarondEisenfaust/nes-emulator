#pragma once
#include <cstdint>

union StatusRegister {
  struct {
    uint8_t unused : 5;
    uint8_t spriteOverflow : 1;
    uint8_t spriteZeroHit : 1;
    uint8_t verticalBlank : 1;
  };
  uint8_t reg;
};

union MaskRegister {
  struct {
    uint8_t grayscale : 1;
    uint8_t renderBackgroundLeft : 1;
    uint8_t renderSpritesLeft : 1;
    uint8_t renderBackground : 1;
    uint8_t renderSprites : 1;
    uint8_t enhanceRed : 1;
    uint8_t enhanceGreen : 1;
    uint8_t enhanceBlue : 1;
  };
  uint8_t reg;
};

union ControlRegister {
  struct {
    uint8_t nametableX : 1;
    uint8_t nametableY : 1;
    uint8_t incrementMode : 1;
    uint8_t patternSprite : 1;
    uint8_t patternBackground : 1;
    uint8_t spriteSize : 1;
    uint8_t slaveMode : 1;
    uint8_t enableNmi : 1;
  };

  uint8_t reg;
};
