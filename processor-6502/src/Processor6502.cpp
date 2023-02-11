#include "Processor6502.h"
#include "Bus.h"
#include "Definitions.h"
#include "Util.h"
#include "fmt/format.h"
#include <cstdint>
#include <iostream>

// Constructor
Processor6502::Processor6502() : addressingModes(this) {
  auto& addrM = addressingModes;
  using a = Processor6502;
  lookup = {{"BRK", &a::BRK, &addrM.imm, 7}, {"ORA", &a::ORA, &addrM.izx, 6}, {"???", &a::XXX, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 8}, {"???", &a::NOP, &addrM.imp, 3}, {"ORA", &a::ORA, &addrM.zp0, 3},
            {"ASL", &a::ASL, &addrM.zp0, 5}, {"???", &a::XXX, &addrM.imp, 5}, {"PHP", &a::PHP, &addrM.imp, 3},
            {"ORA", &a::ORA, &addrM.imm, 2}, {"ASL", &a::ASL, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 2},
            {"???", &a::NOP, &addrM.imp, 4}, {"ORA", &a::ORA, &addrM.abs, 4}, {"ASL", &a::ASL, &addrM.abs, 6},
            {"???", &a::XXX, &addrM.imp, 6}, {"BPL", &a::BPL, &addrM.rel, 2}, {"ORA", &a::ORA, &addrM.izy, 5},
            {"???", &a::XXX, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 8}, {"???", &a::NOP, &addrM.imp, 4},
            {"ORA", &a::ORA, &addrM.zpx, 4}, {"ASL", &a::ASL, &addrM.zpx, 6}, {"???", &a::XXX, &addrM.imp, 6},
            {"CLC", &a::CLC, &addrM.imp, 2}, {"ORA", &a::ORA, &addrM.aby, 4}, {"???", &a::NOP, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 7}, {"???", &a::NOP, &addrM.imp, 4}, {"ORA", &a::ORA, &addrM.abx, 4},
            {"ASL", &a::ASL, &addrM.abx, 7}, {"???", &a::XXX, &addrM.imp, 7}, {"JSR", &a::JSR, &addrM.abs, 6},
            {"AND", &a::AND, &addrM.izx, 6}, {"???", &a::XXX, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 8},
            {"BIT", &a::BIT, &addrM.zp0, 3}, {"AND", &a::AND, &addrM.zp0, 3}, {"ROL", &a::ROL, &addrM.zp0, 5},
            {"???", &a::XXX, &addrM.imp, 5}, {"PLP", &a::PLP, &addrM.imp, 4}, {"AND", &a::AND, &addrM.imm, 2},
            {"ROL", &a::ROL, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 2}, {"BIT", &a::BIT, &addrM.abs, 4},
            {"AND", &a::AND, &addrM.abs, 4}, {"ROL", &a::ROL, &addrM.abs, 6}, {"???", &a::XXX, &addrM.imp, 6},
            {"BMI", &a::BMI, &addrM.rel, 2}, {"AND", &a::AND, &addrM.izy, 5}, {"???", &a::XXX, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 8}, {"???", &a::NOP, &addrM.imp, 4}, {"AND", &a::AND, &addrM.zpx, 4},
            {"ROL", &a::ROL, &addrM.zpx, 6}, {"???", &a::XXX, &addrM.imp, 6}, {"SEC", &a::SEC, &addrM.imp, 2},
            {"AND", &a::AND, &addrM.aby, 4}, {"???", &a::NOP, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 7},
            {"???", &a::NOP, &addrM.imp, 4}, {"AND", &a::AND, &addrM.abx, 4}, {"ROL", &a::ROL, &addrM.abx, 7},
            {"???", &a::XXX, &addrM.imp, 7}, {"RTI", &a::RTI, &addrM.imp, 6}, {"EOR", &a::EOR, &addrM.izx, 6},
            {"???", &a::XXX, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 8}, {"???", &a::NOP, &addrM.imp, 3},
            {"EOR", &a::EOR, &addrM.zp0, 3}, {"LSR", &a::LSR, &addrM.zp0, 5}, {"???", &a::XXX, &addrM.imp, 5},
            {"PHA", &a::PHA, &addrM.imp, 3}, {"EOR", &a::EOR, &addrM.imm, 2}, {"LSR", &a::LSR, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 2}, {"JMP", &a::JMP, &addrM.abs, 3}, {"EOR", &a::EOR, &addrM.abs, 4},
            {"LSR", &a::LSR, &addrM.abs, 6}, {"???", &a::XXX, &addrM.imp, 6}, {"BVC", &a::BVC, &addrM.rel, 2},
            {"EOR", &a::EOR, &addrM.izy, 5}, {"???", &a::XXX, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 8},
            {"???", &a::NOP, &addrM.imp, 4}, {"EOR", &a::EOR, &addrM.zpx, 4}, {"LSR", &a::LSR, &addrM.zpx, 6},
            {"???", &a::XXX, &addrM.imp, 6}, {"CLI", &a::CLI, &addrM.imp, 2}, {"EOR", &a::EOR, &addrM.aby, 4},
            {"???", &a::NOP, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 7}, {"???", &a::NOP, &addrM.imp, 4},
            {"EOR", &a::EOR, &addrM.abx, 4}, {"LSR", &a::LSR, &addrM.abx, 7}, {"???", &a::XXX, &addrM.imp, 7},
            {"RTS", &a::RTS, &addrM.imp, 6}, {"ADC", &a::ADC, &addrM.izx, 6}, {"???", &a::XXX, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 8}, {"???", &a::NOP, &addrM.imp, 3}, {"ADC", &a::ADC, &addrM.zp0, 3},
            {"ROR", &a::ROR, &addrM.zp0, 5}, {"???", &a::XXX, &addrM.imp, 5}, {"PLA", &a::PLA, &addrM.imp, 4},
            {"ADC", &a::ADC, &addrM.imm, 2}, {"ROR", &a::ROR, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 2},
            {"JMP", &a::JMP, &addrM.ind, 5}, {"ADC", &a::ADC, &addrM.abs, 4}, {"ROR", &a::ROR, &addrM.abs, 6},
            {"???", &a::XXX, &addrM.imp, 6}, {"BVS", &a::BVS, &addrM.rel, 2}, {"ADC", &a::ADC, &addrM.izy, 5},
            {"???", &a::XXX, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 8}, {"???", &a::NOP, &addrM.imp, 4},
            {"ADC", &a::ADC, &addrM.zpx, 4}, {"ROR", &a::ROR, &addrM.zpx, 6}, {"???", &a::XXX, &addrM.imp, 6},
            {"SEI", &a::SEI, &addrM.imp, 2}, {"ADC", &a::ADC, &addrM.aby, 4}, {"???", &a::NOP, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 7}, {"???", &a::NOP, &addrM.imp, 4}, {"ADC", &a::ADC, &addrM.abx, 4},
            {"ROR", &a::ROR, &addrM.abx, 7}, {"???", &a::XXX, &addrM.imp, 7}, {"???", &a::NOP, &addrM.imp, 2},
            {"STA", &a::STA, &addrM.izx, 6}, {"???", &a::NOP, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 6},
            {"STY", &a::STY, &addrM.zp0, 3}, {"STA", &a::STA, &addrM.zp0, 3}, {"STX", &a::STX, &addrM.zp0, 3},
            {"???", &a::XXX, &addrM.imp, 3}, {"DEY", &a::DEY, &addrM.imp, 2}, {"???", &a::NOP, &addrM.imp, 2},
            {"TXA", &a::TXA, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 2}, {"STY", &a::STY, &addrM.abs, 4},
            {"STA", &a::STA, &addrM.abs, 4}, {"STX", &a::STX, &addrM.abs, 4}, {"???", &a::XXX, &addrM.imp, 4},
            {"BCC", &a::BCC, &addrM.rel, 2}, {"STA", &a::STA, &addrM.izy, 6}, {"???", &a::XXX, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 6}, {"STY", &a::STY, &addrM.zpx, 4}, {"STA", &a::STA, &addrM.zpx, 4},
            {"STX", &a::STX, &addrM.zpy, 4}, {"???", &a::XXX, &addrM.imp, 4}, {"TYA", &a::TYA, &addrM.imp, 2},
            {"STA", &a::STA, &addrM.aby, 5}, {"TXS", &a::TXS, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 5},
            {"???", &a::NOP, &addrM.imp, 5}, {"STA", &a::STA, &addrM.abx, 5}, {"???", &a::XXX, &addrM.imp, 5},
            {"???", &a::XXX, &addrM.imp, 5}, {"LDY", &a::LDY, &addrM.imm, 2}, {"LDA", &a::LDA, &addrM.izx, 6},
            {"LDX", &a::LDX, &addrM.imm, 2}, {"???", &a::XXX, &addrM.imp, 6}, {"LDY", &a::LDY, &addrM.zp0, 3},
            {"LDA", &a::LDA, &addrM.zp0, 3}, {"LDX", &a::LDX, &addrM.zp0, 3}, {"???", &a::XXX, &addrM.imp, 3},
            {"TAY", &a::TAY, &addrM.imp, 2}, {"LDA", &a::LDA, &addrM.imm, 2}, {"TAX", &a::TAX, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 2}, {"LDY", &a::LDY, &addrM.abs, 4}, {"LDA", &a::LDA, &addrM.abs, 4},
            {"LDX", &a::LDX, &addrM.abs, 4}, {"???", &a::XXX, &addrM.imp, 4}, {"BCS", &a::BCS, &addrM.rel, 2},
            {"LDA", &a::LDA, &addrM.izy, 5}, {"???", &a::XXX, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 5},
            {"LDY", &a::LDY, &addrM.zpx, 4}, {"LDA", &a::LDA, &addrM.zpx, 4}, {"LDX", &a::LDX, &addrM.zpy, 4},
            {"???", &a::XXX, &addrM.imp, 4}, {"CLV", &a::CLV, &addrM.imp, 2}, {"LDA", &a::LDA, &addrM.aby, 4},
            {"TSX", &a::TSX, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 4}, {"LDY", &a::LDY, &addrM.abx, 4},
            {"LDA", &a::LDA, &addrM.abx, 4}, {"LDX", &a::LDX, &addrM.aby, 4}, {"???", &a::XXX, &addrM.imp, 4},
            {"CPY", &a::CPY, &addrM.imm, 2}, {"CMP", &a::CMP, &addrM.izx, 6}, {"???", &a::NOP, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 8}, {"CPY", &a::CPY, &addrM.zp0, 3}, {"CMP", &a::CMP, &addrM.zp0, 3},
            {"DEC", &a::DEC, &addrM.zp0, 5}, {"???", &a::XXX, &addrM.imp, 5}, {"INY", &a::INY, &addrM.imp, 2},
            {"CMP", &a::CMP, &addrM.imm, 2}, {"DEX", &a::DEX, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 2},
            {"CPY", &a::CPY, &addrM.abs, 4}, {"CMP", &a::CMP, &addrM.abs, 4}, {"DEC", &a::DEC, &addrM.abs, 6},
            {"???", &a::XXX, &addrM.imp, 6}, {"BNE", &a::BNE, &addrM.rel, 2}, {"CMP", &a::CMP, &addrM.izy, 5},
            {"???", &a::XXX, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 8}, {"???", &a::NOP, &addrM.imp, 4},
            {"CMP", &a::CMP, &addrM.zpx, 4}, {"DEC", &a::DEC, &addrM.zpx, 6}, {"???", &a::XXX, &addrM.imp, 6},
            {"CLD", &a::CLD, &addrM.imp, 2}, {"CMP", &a::CMP, &addrM.aby, 4}, {"NOP", &a::NOP, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 7}, {"???", &a::NOP, &addrM.imp, 4}, {"CMP", &a::CMP, &addrM.abx, 4},
            {"DEC", &a::DEC, &addrM.abx, 7}, {"???", &a::XXX, &addrM.imp, 7}, {"CPX", &a::CPX, &addrM.imm, 2},
            {"SBC", &a::SBC, &addrM.izx, 6}, {"???", &a::NOP, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 8},
            {"CPX", &a::CPX, &addrM.zp0, 3}, {"SBC", &a::SBC, &addrM.zp0, 3}, {"INC", &a::INC, &addrM.zp0, 5},
            {"???", &a::XXX, &addrM.imp, 5}, {"INX", &a::INX, &addrM.imp, 2}, {"SBC", &a::SBC, &addrM.imm, 2},
            {"NOP", &a::NOP, &addrM.imp, 2}, {"???", &a::SBC, &addrM.imp, 2}, {"CPX", &a::CPX, &addrM.abs, 4},
            {"SBC", &a::SBC, &addrM.abs, 4}, {"INC", &a::INC, &addrM.abs, 6}, {"???", &a::XXX, &addrM.imp, 6},
            {"BEQ", &a::BEQ, &addrM.rel, 2}, {"SBC", &a::SBC, &addrM.izy, 5}, {"???", &a::XXX, &addrM.imp, 2},
            {"???", &a::XXX, &addrM.imp, 8}, {"???", &a::NOP, &addrM.imp, 4}, {"SBC", &a::SBC, &addrM.zpx, 4},
            {"INC", &a::INC, &addrM.zpx, 6}, {"???", &a::XXX, &addrM.imp, 6}, {"SED", &a::SED, &addrM.imp, 2},
            {"SBC", &a::SBC, &addrM.aby, 4}, {"NOP", &a::NOP, &addrM.imp, 2}, {"???", &a::XXX, &addrM.imp, 7},
            {"???", &a::NOP, &addrM.imp, 4}, {"SBC", &a::SBC, &addrM.abx, 4}, {"INC", &a::INC, &addrM.abx, 7},
            {"???", &a::XXX, &addrM.imp, 7}};
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
  if (!(lookup[opcode].addrMode == &addressingModes.imp)) {
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

    auto additionalCycle1 = (*lookup[opcode].addrMode)();
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

  if (lookup[opcode].addrMode == &addressingModes.imp) {
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

  if (lookup[opcode].addrMode == &addressingModes.imp) {
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

  if (lookup[opcode].addrMode == &addressingModes.imp) {
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

  if (lookup[opcode].addrMode == &addressingModes.imp) {
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

    if (instruction.addrMode == &addressingModes.imp) {
      addressingMode = "{IMP}";
    } else if (instruction.addrMode == &addressingModes.imm) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#04x} {{IMM}}", value);
    } else if (instruction.addrMode == &addressingModes.zp0) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#04x} {{ZP0}}", value);
    } else if (instruction.addrMode == &addressingModes.zpx) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#04x}, X {{ZPX}}", value);
    } else if (instruction.addrMode == &addressingModes.zpy) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#04x}, Y {{ZPY}} ", value);
    } else if (instruction.addrMode == &addressingModes.izx) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("({:#04x}, X) {{IZX}}", value);
    } else if (instruction.addrMode == &addressingModes.izy) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("({:#04x}), Y {{IZY}}", value);
    } else if (instruction.addrMode == &addressingModes.abs) {
      auto lo = mBus->CpuRead(current, true);
      current++;
      auto hi = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#06x} {{ABS}}", static_cast<uint16_t>(hi << 8) | lo, 4);
    } else if (instruction.addrMode == &addressingModes.abx) {
      auto lo = mBus->CpuRead(current, true);
      current++;
      auto hi = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#06x}, X {{ABX}}", static_cast<uint16_t>(hi << 8) | lo, 4);
    } else if (instruction.addrMode == &addressingModes.aby) {
      auto lo = mBus->CpuRead(current, true);
      current++;
      auto hi = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#06x}, Y {{ABY}}", static_cast<uint16_t>(hi << 8) | lo, 4);
    } else if (instruction.addrMode == &addressingModes.ind) {
      auto lo = mBus->CpuRead(current, true);
      current++;
      auto hi = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("({:#06x}) {{IND}}", static_cast<uint16_t>(hi << 8) | lo, 4);
    } else if (instruction.addrMode == &addressingModes.rel) {
      auto value = mBus->CpuRead(current, true);
      current++;
      addressingMode = fmt::format("{:#04x} [{:#06x}] {{REL}}", value, current + static_cast<int8_t>(value));
    }

    auto instructionAsString = fmt::format("{:#06x}: {} {}", line, instruction.name, addressingMode);
    disassembledCode[line] = instructionAsString;
  }
  return disassembledCode;
}
