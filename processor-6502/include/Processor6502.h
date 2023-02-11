#pragma once
#include "FLAGS6502.h"
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

class Bus;

class Processor6502 {
 public:
  Processor6502();
  virtual ~Processor6502() = default;

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

 protected:
  // Opcodes ======================================================
  bool ADC();
  bool AND();
  bool ASL();
  bool BCC();
  bool BCS();
  bool BEQ();
  bool BIT();
  bool BMI();
  bool BNE();
  bool BPL();
  bool BRK();
  bool BVC();
  bool BVS();
  bool CLC();
  bool CLD();
  bool CLI();
  bool CLV();
  bool CMP();
  bool CPX();
  bool CPY();
  bool DEC();
  bool DEX();
  bool DEY();
  bool EOR();
  bool INC();
  bool INX();
  bool INY();
  bool JMP();
  bool JSR();
  bool LDA();
  bool LDX();
  bool LDY();
  bool LSR();
  bool NOP();
  bool ORA();
  bool PHA();
  bool PHP();
  bool PLA();
  bool PLP();
  bool ROL();
  bool ROR();
  bool RTI();
  bool RTS();
  bool SBC();
  bool SEC();
  bool SED();
  bool SEI();
  bool STA();
  bool STX();
  bool STY();
  bool TAX();
  bool TAY();
  bool TSX();
  bool TXA();
  bool TXS();
  bool TYA();
  bool XXX();

  bool BranchIf(bool condition);

 protected:
  struct Instruction {
    std::string name;
    bool (Processor6502::*operate)(void) = nullptr;
    IAddressingMode* addrMode;
    uint8_t cycles = 0;
  };

  std::vector<Instruction> lookup;

  void Interrupt(uint16_t address, uint8_t numCycles);
};
