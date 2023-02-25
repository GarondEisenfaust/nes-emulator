#pragma once
#include "FLAGS6502.h"
#include "Instruction.h"
#include "LookupTable.h"
#include "addressing-modes/AddressingModeContainer.h"
#include "opcodes/OpcodeContainer.h"
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Bus;

class Cpu {
 public:
  Cpu();
  virtual ~Cpu() = default;

  uint8_t Read(uint16_t addr);
  uint8_t PopFromStack();
  virtual void Write(uint16_t addr, uint8_t data);
  virtual void PushToStack(uint8_t data);

  void Interrupt();
  void NonMaskableInterrupt();

  virtual uint8_t Fetch();

  void Clock();
  void Reset();

  void ConnectBus(Bus* bus);
  std::map<uint16_t, std::string> Disassemble(uint16_t begin, uint16_t end);

  uint8_t fetched = 0x00;
  uint8_t a = 0x00;
  uint8_t x = 0x00;
  uint8_t y = 0x00;
  uint8_t stackPointer = 0x00;
  uint16_t pc = 0x0000;
  uint8_t cycles = 0;
  uint16_t addrAbs = 0x0000;
  uint16_t addrRel = 0x00;
  uint8_t opcode = 0x00;

  Bus* mBus;

  union StatusRegister {
    struct {
      bool c : 1;
      bool z : 1;
      bool i : 1;
      bool d : 1;
      bool b : 1;
      bool u : 1;
      bool v : 1;
      bool n : 1;
    };
    uint8_t reg;
  } status;

  bool BranchIf(bool condition);

  std::unique_ptr<LookupTable> lookup;
  AddressingModeContainer addressingModes;
  OpcodeContainer opcodes;

  void Interrupt(uint16_t address, uint8_t numCycles);
};
