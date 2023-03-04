#pragma once
#include <cstdint>

union StatusRegister {
  struct {
    bool unused : 5;
    bool spriteOverflow : 1;
    bool spriteZeroHit : 1;
    bool verticalBlank : 1;
  };
  uint8_t reg;
};

union MaskRegister {
  struct {
    bool grayscale : 1;
    bool renderBackgroundLeft : 1;
    bool renderSpritesLeft : 1;
    bool renderBackground : 1;
    bool renderSprites : 1;
    bool enhanceRed : 1;
    bool enhanceGreen : 1;
    bool enhanceBlue : 1;
  };
  uint8_t reg;
};

union ControlRegister {
  struct {
    bool nametableX : 1;
    bool nametableY : 1;
    bool incrementMode : 1;
    bool patternSprite : 1;
    bool patternBackground : 1;
    bool spriteSize : 1;
    bool slaveMode : 1;
    bool enableNmi : 1;
  };
  uint8_t reg;
};

union LoopyRegister {
  struct {
    uint16_t coarseX : 5;
    uint16_t coarseY : 5;
    bool nameTableX : 1;
    bool nameTableY : 1;
    uint16_t fineY : 3;
    bool unused : 1;
  };
  uint16_t reg = 0x0000;
};
