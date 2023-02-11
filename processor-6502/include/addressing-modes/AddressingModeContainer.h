#pragma once

#include "ABS.h"
#include "ABX.h"
#include "ABY.h"
#include "IAddressingMode.h"
#include "IMM.h"
#include "IMP.h"
#include "IND.h"
#include "IZX.h"
#include "IZY.h"
#include "REL.h"
#include "ZP0.h"
#include "ZPX.h"
#include "ZPY.h"

class Processor6502;

class AddressingModeContainer {
 public:
  AddressingModeContainer(Processor6502* cpu);
  ABS abs;
  ABX abx;
  ABY aby;
  IMM imm;
  IMP imp;
  IND ind;
  IZX izx;
  IZY izy;
  REL rel;
  ZP0 zp0;
  ZPX zpx;
  ZPY zpy;
};
