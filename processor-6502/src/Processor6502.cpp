#include "Processor6502.h"
#include "Bus.h"
#include "Definitions.h"
#include "Util.h"
#include "addressing-modes/AddressingModes.h"
#include "fmt/format.h"
#include <cstdint>
#include <iostream>

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

// Constructor
Processor6502::Processor6502() {
  using a = Processor6502;
  lookup = {{"BRK", &a::BRK, imm, 7}, {"ORA", &a::ORA, izx, 6}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 8},
            {"???", &a::NOP, imp, 3}, {"ORA", &a::ORA, zp0, 3}, {"ASL", &a::ASL, zp0, 5}, {"???", &a::XXX, imp, 5},
            {"PHP", &a::PHP, imp, 3}, {"ORA", &a::ORA, imm, 2}, {"ASL", &a::ASL, imp, 2}, {"???", &a::XXX, imp, 2},
            {"???", &a::NOP, imp, 4}, {"ORA", &a::ORA, abs, 4}, {"ASL", &a::ASL, abs, 6}, {"???", &a::XXX, imp, 6},
            {"BPL", &a::BPL, rel, 2}, {"ORA", &a::ORA, izy, 5}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 8},
            {"???", &a::NOP, imp, 4}, {"ORA", &a::ORA, zpx, 4}, {"ASL", &a::ASL, zpx, 6}, {"???", &a::XXX, imp, 6},
            {"CLC", &a::CLC, imp, 2}, {"ORA", &a::ORA, aby, 4}, {"???", &a::NOP, imp, 2}, {"???", &a::XXX, imp, 7},
            {"???", &a::NOP, imp, 4}, {"ORA", &a::ORA, abx, 4}, {"ASL", &a::ASL, abx, 7}, {"???", &a::XXX, imp, 7},
            {"JSR", &a::JSR, abs, 6}, {"AND", &a::AND, izx, 6}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 8},
            {"BIT", &a::BIT, zp0, 3}, {"AND", &a::AND, zp0, 3}, {"ROL", &a::ROL, zp0, 5}, {"???", &a::XXX, imp, 5},
            {"PLP", &a::PLP, imp, 4}, {"AND", &a::AND, imm, 2}, {"ROL", &a::ROL, imp, 2}, {"???", &a::XXX, imp, 2},
            {"BIT", &a::BIT, abs, 4}, {"AND", &a::AND, abs, 4}, {"ROL", &a::ROL, abs, 6}, {"???", &a::XXX, imp, 6},
            {"BMI", &a::BMI, rel, 2}, {"AND", &a::AND, izy, 5}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 8},
            {"???", &a::NOP, imp, 4}, {"AND", &a::AND, zpx, 4}, {"ROL", &a::ROL, zpx, 6}, {"???", &a::XXX, imp, 6},
            {"SEC", &a::SEC, imp, 2}, {"AND", &a::AND, aby, 4}, {"???", &a::NOP, imp, 2}, {"???", &a::XXX, imp, 7},
            {"???", &a::NOP, imp, 4}, {"AND", &a::AND, abx, 4}, {"ROL", &a::ROL, abx, 7}, {"???", &a::XXX, imp, 7},
            {"RTI", &a::RTI, imp, 6}, {"EOR", &a::EOR, izx, 6}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 8},
            {"???", &a::NOP, imp, 3}, {"EOR", &a::EOR, zp0, 3}, {"LSR", &a::LSR, zp0, 5}, {"???", &a::XXX, imp, 5},
            {"PHA", &a::PHA, imp, 3}, {"EOR", &a::EOR, imm, 2}, {"LSR", &a::LSR, imp, 2}, {"???", &a::XXX, imp, 2},
            {"JMP", &a::JMP, abs, 3}, {"EOR", &a::EOR, abs, 4}, {"LSR", &a::LSR, abs, 6}, {"???", &a::XXX, imp, 6},
            {"BVC", &a::BVC, rel, 2}, {"EOR", &a::EOR, izy, 5}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 8},
            {"???", &a::NOP, imp, 4}, {"EOR", &a::EOR, zpx, 4}, {"LSR", &a::LSR, zpx, 6}, {"???", &a::XXX, imp, 6},
            {"CLI", &a::CLI, imp, 2}, {"EOR", &a::EOR, aby, 4}, {"???", &a::NOP, imp, 2}, {"???", &a::XXX, imp, 7},
            {"???", &a::NOP, imp, 4}, {"EOR", &a::EOR, abx, 4}, {"LSR", &a::LSR, abx, 7}, {"???", &a::XXX, imp, 7},
            {"RTS", &a::RTS, imp, 6}, {"ADC", &a::ADC, izx, 6}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 8},
            {"???", &a::NOP, imp, 3}, {"ADC", &a::ADC, zp0, 3}, {"ROR", &a::ROR, zp0, 5}, {"???", &a::XXX, imp, 5},
            {"PLA", &a::PLA, imp, 4}, {"ADC", &a::ADC, imm, 2}, {"ROR", &a::ROR, imp, 2}, {"???", &a::XXX, imp, 2},
            {"JMP", &a::JMP, ind, 5}, {"ADC", &a::ADC, abs, 4}, {"ROR", &a::ROR, abs, 6}, {"???", &a::XXX, imp, 6},
            {"BVS", &a::BVS, rel, 2}, {"ADC", &a::ADC, izy, 5}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 8},
            {"???", &a::NOP, imp, 4}, {"ADC", &a::ADC, zpx, 4}, {"ROR", &a::ROR, zpx, 6}, {"???", &a::XXX, imp, 6},
            {"SEI", &a::SEI, imp, 2}, {"ADC", &a::ADC, aby, 4}, {"???", &a::NOP, imp, 2}, {"???", &a::XXX, imp, 7},
            {"???", &a::NOP, imp, 4}, {"ADC", &a::ADC, abx, 4}, {"ROR", &a::ROR, abx, 7}, {"???", &a::XXX, imp, 7},
            {"???", &a::NOP, imp, 2}, {"STA", &a::STA, izx, 6}, {"???", &a::NOP, imp, 2}, {"???", &a::XXX, imp, 6},
            {"STY", &a::STY, zp0, 3}, {"STA", &a::STA, zp0, 3}, {"STX", &a::STX, zp0, 3}, {"???", &a::XXX, imp, 3},
            {"DEY", &a::DEY, imp, 2}, {"???", &a::NOP, imp, 2}, {"TXA", &a::TXA, imp, 2}, {"???", &a::XXX, imp, 2},
            {"STY", &a::STY, abs, 4}, {"STA", &a::STA, abs, 4}, {"STX", &a::STX, abs, 4}, {"???", &a::XXX, imp, 4},
            {"BCC", &a::BCC, rel, 2}, {"STA", &a::STA, izy, 6}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 6},
            {"STY", &a::STY, zpx, 4}, {"STA", &a::STA, zpx, 4}, {"STX", &a::STX, zpy, 4}, {"???", &a::XXX, imp, 4},
            {"TYA", &a::TYA, imp, 2}, {"STA", &a::STA, aby, 5}, {"TXS", &a::TXS, imp, 2}, {"???", &a::XXX, imp, 5},
            {"???", &a::NOP, imp, 5}, {"STA", &a::STA, abx, 5}, {"???", &a::XXX, imp, 5}, {"???", &a::XXX, imp, 5},
            {"LDY", &a::LDY, imm, 2}, {"LDA", &a::LDA, izx, 6}, {"LDX", &a::LDX, imm, 2}, {"???", &a::XXX, imp, 6},
            {"LDY", &a::LDY, zp0, 3}, {"LDA", &a::LDA, zp0, 3}, {"LDX", &a::LDX, zp0, 3}, {"???", &a::XXX, imp, 3},
            {"TAY", &a::TAY, imp, 2}, {"LDA", &a::LDA, imm, 2}, {"TAX", &a::TAX, imp, 2}, {"???", &a::XXX, imp, 2},
            {"LDY", &a::LDY, abs, 4}, {"LDA", &a::LDA, abs, 4}, {"LDX", &a::LDX, abs, 4}, {"???", &a::XXX, imp, 4},
            {"BCS", &a::BCS, rel, 2}, {"LDA", &a::LDA, izy, 5}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 5},
            {"LDY", &a::LDY, zpx, 4}, {"LDA", &a::LDA, zpx, 4}, {"LDX", &a::LDX, zpy, 4}, {"???", &a::XXX, imp, 4},
            {"CLV", &a::CLV, imp, 2}, {"LDA", &a::LDA, aby, 4}, {"TSX", &a::TSX, imp, 2}, {"???", &a::XXX, imp, 4},
            {"LDY", &a::LDY, abx, 4}, {"LDA", &a::LDA, abx, 4}, {"LDX", &a::LDX, aby, 4}, {"???", &a::XXX, imp, 4},
            {"CPY", &a::CPY, imm, 2}, {"CMP", &a::CMP, izx, 6}, {"???", &a::NOP, imp, 2}, {"???", &a::XXX, imp, 8},
            {"CPY", &a::CPY, zp0, 3}, {"CMP", &a::CMP, zp0, 3}, {"DEC", &a::DEC, zp0, 5}, {"???", &a::XXX, imp, 5},
            {"INY", &a::INY, imp, 2}, {"CMP", &a::CMP, imm, 2}, {"DEX", &a::DEX, imp, 2}, {"???", &a::XXX, imp, 2},
            {"CPY", &a::CPY, abs, 4}, {"CMP", &a::CMP, abs, 4}, {"DEC", &a::DEC, abs, 6}, {"???", &a::XXX, imp, 6},
            {"BNE", &a::BNE, rel, 2}, {"CMP", &a::CMP, izy, 5}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 8},
            {"???", &a::NOP, imp, 4}, {"CMP", &a::CMP, zpx, 4}, {"DEC", &a::DEC, zpx, 6}, {"???", &a::XXX, imp, 6},
            {"CLD", &a::CLD, imp, 2}, {"CMP", &a::CMP, aby, 4}, {"NOP", &a::NOP, imp, 2}, {"???", &a::XXX, imp, 7},
            {"???", &a::NOP, imp, 4}, {"CMP", &a::CMP, abx, 4}, {"DEC", &a::DEC, abx, 7}, {"???", &a::XXX, imp, 7},
            {"CPX", &a::CPX, imm, 2}, {"SBC", &a::SBC, izx, 6}, {"???", &a::NOP, imp, 2}, {"???", &a::XXX, imp, 8},
            {"CPX", &a::CPX, zp0, 3}, {"SBC", &a::SBC, zp0, 3}, {"INC", &a::INC, zp0, 5}, {"???", &a::XXX, imp, 5},
            {"INX", &a::INX, imp, 2}, {"SBC", &a::SBC, imm, 2}, {"NOP", &a::NOP, imp, 2}, {"???", &a::SBC, imp, 2},
            {"CPX", &a::CPX, abs, 4}, {"SBC", &a::SBC, abs, 4}, {"INC", &a::INC, abs, 6}, {"???", &a::XXX, imp, 6},
            {"BEQ", &a::BEQ, rel, 2}, {"SBC", &a::SBC, izy, 5}, {"???", &a::XXX, imp, 2}, {"???", &a::XXX, imp, 8},
            {"???", &a::NOP, imp, 4}, {"SBC", &a::SBC, zpx, 4}, {"INC", &a::INC, zpx, 6}, {"???", &a::XXX, imp, 6},
            {"SED", &a::SED, imp, 2}, {"SBC", &a::SBC, aby, 4}, {"NOP", &a::NOP, imp, 2}, {"???", &a::XXX, imp, 7},
            {"???", &a::NOP, imp, 4}, {"SBC", &a::SBC, abx, 4}, {"INC", &a::INC, abx, 7}, {"???", &a::XXX, imp, 7}};
}

void Processor6502::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mCpu = this;
}

uint8_t Processor6502::Read(uint16_t addr) { return mBus->CpuRead(addr); }
uint8_t Processor6502::PopFromStack() {
  stackPointer++;
  return Read(STACK_BEGIN + stackPointer);
}

void Processor6502::Write(uint16_t addr, uint8_t data) { mBus->CpuWrite(addr, data); }
void Processor6502::PushToStack(uint8_t data) {
  Write(STACK_BEGIN + stackPointer, data);
  stackPointer--;
}

uint8_t Processor6502::Fetch() {
  if (!(lookup[opcode].addrMode == &Processor6502::IMP)) {
    fetched = Read(addrAbs);
  }
  return fetched;
}

void Processor6502::Clock() {
  if (cycles == 0) {
    opcode = Read(pc);
    status.u = true;
    pc++;

    auto instruction = lookup[opcode];
    cycles = lookup[opcode].cycles;

    auto additionalCycle1 = lookup[opcode].addrMode();
    auto additionalCycle2 = (this->*lookup[opcode].operate)();

    cycles += additionalCycle1 && additionalCycle2;
    status.u = true;
  }
  cycles--;
}

void Processor6502::Reset() {
  addrAbs = 0xFFFC;
  uint16_t lo = Read(addrAbs + 0);
  uint16_t hi = Read(addrAbs + 1);

  pc = (hi << 8) | lo;
  // nestest
  //  pc = 0xC000;

  a = 0;
  x = 0;
  y = 0;
  stackPointer = 0xFD;
  status.reg = 0x00;
  status.u = true;

  addrRel = 0x0000;
  addrAbs = 0x0000;
  fetched = 0x00;

  cycles = 8;
}

// Opcodes ======================================================
bool Processor6502::ADC() {
  Fetch();
  auto temp = a + fetched + status.c;
  status.c = temp > 255;
  status.z = (temp & 0x00FF) == 0;

  status.v = (~(a ^ fetched) & (a ^ temp)) & 0x0080;
  status.n = temp & 0x80;
  a = temp & 0x00FF;
  return 1;
}

bool Processor6502::AND() {
  Fetch();
  a = a & fetched;
  status.z = a == 0x00;
  status.n = a & 0x80;
  return 1;
}

bool Processor6502::ASL() {
  Fetch();
  auto temp = fetched << 1;
  status.c = (temp & 0xFF00) > 0;
  status.z = (temp & 0x00FF) == 0x00;
  status.n = temp & 0x80;

  if (lookup[opcode].addrMode == &Processor6502::IMP) {
    a = temp & 0x00FF;
  } else {
    Write(addrAbs, temp & 0x00FF);
  }
  return 0;
}

bool Processor6502::BCC() { return BranchIf(status.c == 0); }

bool Processor6502::BCS() { return BranchIf(status.c == 1); }

bool Processor6502::BEQ() { return BranchIf(status.z == 1); }

bool Processor6502::BIT() {
  Fetch();
  auto temp = a & fetched;
  status.z = (temp & 0x00FF) == 0x00;
  status.n = fetched & (1 << 7);
  status.v = fetched & (1 << 6);
  return 0;
}

bool Processor6502::BMI() { return BranchIf(status.n == 1); }

bool Processor6502::BNE() { return BranchIf(status.z == 0); }

bool Processor6502::BPL() { return BranchIf(status.n == 0); }

bool Processor6502::BRK() {
  pc++;

  status.i = true;
  PushToStack((pc >> 8) & 0x00FF);
  PushToStack(pc & 0x00FF);

  status.b = true;
  PushToStack(status.reg);
  status.b = false;

  pc = Read(0xFFFE) | (Read(0xFFFF) << 8);
  return 0;
}

bool Processor6502::BVC() { return BranchIf(status.v == 0); }

bool Processor6502::BVS() { return BranchIf(status.v == 1); }

bool Processor6502::CLC() {
  status.c = false;
  return 0;
}

bool Processor6502::CLD() {
  status.d = false;
  return 0;
}

bool Processor6502::CLI() {
  status.i = false;
  return 0;
}

bool Processor6502::CLV() {
  status.v = false;
  return 0;
}

bool Processor6502::CMP() {
  Fetch();
  auto temp = a - fetched;
  status.c = a >= fetched;
  status.z = (temp & 0x00FF) == 0x0000;
  status.n = temp & 0x0080;
  return 1;
}

bool Processor6502::CPX() {
  Fetch();
  auto temp = x - fetched;
  status.c = x >= fetched;
  status.z = (temp & 0x00FF) == 0x0000;
  status.n = temp & 0x0080;
  return 0;
}

bool Processor6502::CPY() {
  Fetch();
  auto temp = y - fetched;
  status.c = y >= fetched;
  status.z = (temp & 0x00FF) == 0x0000;
  status.n = temp & 0x0080;
  return 0;
}

bool Processor6502::DEC() {
  Fetch();
  auto temp = fetched - 1;
  Write(addrAbs, temp & 0x00FF);
  status.z = (temp & 0x00FF) == 0x0000;
  status.n = temp & 0x0080;
  return 0;
}

bool Processor6502::DEX() {
  x--;
  status.z = x == 0x00;
  status.n = x & 0x80;
  return 0;
}

bool Processor6502::DEY() {
  y--;
  status.z = y == 0x00;
  status.n = y & 0x80;
  return 0;
}

bool Processor6502::EOR() {
  Fetch();
  a = a ^ fetched;
  status.z = a == 0x00;
  status.n = a & 0x80;
  return 1;
}

bool Processor6502::INC() {
  Fetch();
  auto temp = fetched + 1;
  Write(addrAbs, temp);
  status.z = (temp & 0x00FF) == 0x0000;
  status.n = temp & 0x0080;
  return 0;
}

bool Processor6502::INX() {
  x++;
  status.z = x == 0x00;
  status.n = x & 0x80;
  return 0;
}

bool Processor6502::INY() {
  y++;
  status.z = y == 0;
  status.n = y & 0x80;
  return 0;
}

bool Processor6502::JMP() {
  pc = addrAbs;
  return 0;
}

bool Processor6502::JSR() {
  pc--;
  PushToStack((pc >> 8) & 0x00FF);
  PushToStack(pc & 0x00FF);

  pc = addrAbs;
  return 0;
}

bool Processor6502::LDA() {
  Fetch();
  a = fetched;
  status.z = a == 0x00;
  status.n = a & 0x80;
  return 1;
}

bool Processor6502::LDX() {
  Fetch();
  x = fetched;
  status.z = x == 0x00;
  status.n = x & 0x80;
  return 1;
}

bool Processor6502::LDY() {
  Fetch();
  y = fetched;
  status.z = y == 0x00;
  status.n = y & 0x80;
  return 1;
}

bool Processor6502::LSR() {
  Fetch();
  status.c = fetched & 0x0001;
  auto temp = fetched >> 1;
  status.z = (temp & 0x00FF) == 0x0000;
  status.n = temp & 0x0080;

  if (lookup[opcode].addrMode == &Processor6502::IMP) {
    a = temp & 0x00FF;
  } else {
    Write(addrAbs, temp & 0x00FF);
  }
  return 0;
}

bool Processor6502::NOP() {
  switch (opcode) {
    case 0x1C:
    case 0x3C:
    case 0x5C:
    case 0x7C:
    case 0xDC:
    case 0xFC:
      return 1;
      break;
  }
  return 0;
}

bool Processor6502::ORA() {
  Fetch();
  a = a | fetched;
  status.z = a == 0x00;
  status.n = a & 0x80;
  return 1;
}

bool Processor6502::PHA() {
  PushToStack(a);
  return 0;
}

bool Processor6502::PHP() {
  PushToStack(status.reg | B | U);
  status.b = false;
  status.u = false;
  return 0;
}

bool Processor6502::PLA() {
  a = PopFromStack();
  status.z = a == 0x00;
  status.n = a & 0x80;
  return 0;
}

bool Processor6502::PLP() {
  status.reg = PopFromStack();
  status.u = true;
  return 0;
}

bool Processor6502::ROL() {
  Fetch();
  auto temp = fetched << 1 | status.c;
  status.c = temp & 0xFF00;
  status.z = (temp & 0x00FF) == 0x0000;
  status.n = temp & 0x0080;

  if (lookup[opcode].addrMode == &Processor6502::IMP) {
    a = temp & 0x00FF;
  } else {
    Write(addrAbs, temp & 0x00FF);
  }
  return 0;
}

bool Processor6502::ROR() {
  Fetch();

  auto temp = status.c << 7 | (fetched >> 1);
  status.c = fetched & 0x01;
  status.z = (temp & 0x00FF) == 0x00;
  status.n = temp & 0x0080;

  if (lookup[opcode].addrMode == &Processor6502::IMP) {
    a = temp & 0x00FF;
  } else {
    Write(addrAbs, temp & 0x00FF);
  }
  return 0;
}

bool Processor6502::RTI() {
  status.reg = PopFromStack();
  status.reg &= ~B;
  status.reg &= ~U;

  pc = PopFromStack();
  pc |= PopFromStack() << 8;
  return 0;
}

bool Processor6502::RTS() {
  pc = PopFromStack();
  pc |= PopFromStack() << 8;

  pc++;
  return 0;
}

bool Processor6502::SBC() {
  Fetch();

  uint16_t value = fetched ^ 0x00FF;

  auto temp = a + value + status.c;
  status.c = temp & 0xFF00;
  status.z = (temp & 0x00FF) == 0;
  status.v = (temp ^ a) & (temp ^ value) & 0x0080;
  status.n = temp & 0x0080;
  a = temp & 0x00FF;
  return 1;
}

bool Processor6502::SEC() {
  status.c = true;
  return 0;
}

bool Processor6502::SED() {
  status.d = true;
  return 0;
}

bool Processor6502::SEI() {
  status.i = true;
  return 0;
}

bool Processor6502::STA() {
  Write(addrAbs, a);
  return 0;
}

bool Processor6502::STX() {
  Write(addrAbs, x);
  return 0;
}

bool Processor6502::STY() {
  Write(addrAbs, y);
  return 0;
}

bool Processor6502::TAX() {
  x = a;
  status.z = x == 0x00;
  status.n = x & 0x80;
  return 0;
}

bool Processor6502::TAY() {
  y = a;
  status.z = y == 0x00;
  status.n = y & 0x80;
  return 0;
}

bool Processor6502::TSX() {
  x = stackPointer;
  status.z = x == 0x00;
  status.n = x & 0x80;
  return 0;
}

bool Processor6502::TXA() {
  a = x;
  status.z = a == 0x00;
  status.n = a & 0x80;
  return 0;
}

bool Processor6502::TXS() {
  stackPointer = x;
  return 0;
}

bool Processor6502::TYA() {
  a = y;
  status.z = a == 0x00;
  status.n = a & 0x80;
  return 0;
}

bool Processor6502::XXX() { return 0; }

bool Processor6502::BranchIf(bool condition) {
  if (condition) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
  return 0;
}

void Processor6502::Interrupt(uint16_t address, uint8_t numCycles) {
  PushToStack((pc >> 8) & 0x00FF);
  PushToStack(pc & 0x00FF);

  status.b = false;
  status.u = true;
  status.i = true;
  PushToStack(status.reg);

  addrAbs = address;
  uint16_t lo = Read(addrAbs + 0);
  uint16_t hi = Read(addrAbs + 1);
  pc = (hi << 8) | lo;

  cycles = numCycles;
}

void Processor6502::Interrupt() {
  if (status.i != 0) {
    return;
  }

  Interrupt(0xFFFE, 7);
}

void Processor6502::NonMaskableInterrupt() { Interrupt(0xFFFA, 8); }

std::map<uint16_t, std::string> Processor6502::Disassemble(uint16_t begin, uint16_t end) {
  uint32_t current = begin;
  std::map<uint16_t, std::string> disassembledCode;

  while (current <= end) {
    auto line = current;
    uint8_t opcode = mBus->CpuRead(current, true);
    current++;
    auto& instruction = lookup[opcode];
    std::string addressingMode = "";

    if (instruction.addrMode == &Processor6502::IMP) {
      addressingMode = "{IMP}";
    } else if (instruction.addrMode == &Processor6502::IMM) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#04x} {{IMM}}", value);
    } else if (instruction.addrMode == &Processor6502::ZP0) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#04x} {{ZP0}}", value);
    } else if (instruction.addrMode == &Processor6502::ZPX) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#04x}, X {{ZPX}}", value);
    } else if (instruction.addrMode == &Processor6502::ZPY) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#04x}, Y {{ZPY}} ", value);
    } else if (instruction.addrMode == &Processor6502::IZX) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("({:#04x}, X) {{IZX}}", value);
    } else if (instruction.addrMode == &Processor6502::IZY) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("({:#04x}), Y {{IZY}}", value);
    } else if (instruction.addrMode == &Processor6502::ABS) {
      auto lo = mBus->CpuRead(current, true);
      current++;
      auto hi = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#06x} {{ABS}}", static_cast<uint16_t>(hi << 8) | lo, 4);
    } else if (instruction.addrMode == &Processor6502::ABX) {
      auto lo = mBus->CpuRead(current, true);
      current++;
      auto hi = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#06x}, X {{ABX}}", static_cast<uint16_t>(hi << 8) | lo, 4);
    } else if (instruction.addrMode == &Processor6502::ABY) {
      auto lo = mBus->CpuRead(current, true);
      current++;
      auto hi = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#06x}, Y {{ABY}}", static_cast<uint16_t>(hi << 8) | lo, 4);
    } else if (instruction.addrMode == &Processor6502::IND) {
      auto lo = mBus->CpuRead(current, true);
      current++;
      auto hi = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("({:#06x}) {{IND}}", static_cast<uint16_t>(hi << 8) | lo, 4);
    } else if (instruction.addrMode == &Processor6502::REL) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#04x} [{:#06x}] {{REL}}", value, current + static_cast<int8_t>(value));
    }

    auto instructionAsString = fmt::format("{:#06x}: {} {}", line, instruction.name, addressingMode);
    disassembledCode[line] = instructionAsString;
  }
  return disassembledCode;
}
