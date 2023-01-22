#include "Processor6502.h"
#include "Bus.h"
#include "Definitions.h"
#include "Util.h"
#include "fmt/format.h"
#include <cstdint>
#include <iostream>

// Constructor
Processor6502::Processor6502() {
  using a = Processor6502;
  lookup = {{"BRK", &a::BRK, &a::IMM, 7}, {"ORA", &a::ORA, &a::IZX, 6}, {"???", &a::XXX, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 3}, {"ORA", &a::ORA, &a::ZP0, 3},
            {"ASL", &a::ASL, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5}, {"PHP", &a::PHP, &a::IMP, 3},
            {"ORA", &a::ORA, &a::IMM, 2}, {"ASL", &a::ASL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
            {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ABS, 4}, {"ASL", &a::ASL, &a::ABS, 6},
            {"???", &a::XXX, &a::IMP, 6}, {"BPL", &a::BPL, &a::REL, 2}, {"ORA", &a::ORA, &a::IZY, 5},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4},
            {"ORA", &a::ORA, &a::ZPX, 4}, {"ASL", &a::ASL, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"CLC", &a::CLC, &a::IMP, 2}, {"ORA", &a::ORA, &a::ABY, 4}, {"???", &a::NOP, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ABX, 4},
            {"ASL", &a::ASL, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7}, {"JSR", &a::JSR, &a::ABS, 6},
            {"AND", &a::AND, &a::IZX, 6}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"BIT", &a::BIT, &a::ZP0, 3}, {"AND", &a::AND, &a::ZP0, 3}, {"ROL", &a::ROL, &a::ZP0, 5},
            {"???", &a::XXX, &a::IMP, 5}, {"PLP", &a::PLP, &a::IMP, 4}, {"AND", &a::AND, &a::IMM, 2},
            {"ROL", &a::ROL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"BIT", &a::BIT, &a::ABS, 4},
            {"AND", &a::AND, &a::ABS, 4}, {"ROL", &a::ROL, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"BMI", &a::BMI, &a::REL, 2}, {"AND", &a::AND, &a::IZY, 5}, {"???", &a::XXX, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4}, {"AND", &a::AND, &a::ZPX, 4},
            {"ROL", &a::ROL, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6}, {"SEC", &a::SEC, &a::IMP, 2},
            {"AND", &a::AND, &a::ABY, 4}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
            {"???", &a::NOP, &a::IMP, 4}, {"AND", &a::AND, &a::ABX, 4}, {"ROL", &a::ROL, &a::ABX, 7},
            {"???", &a::XXX, &a::IMP, 7}, {"RTI", &a::RTI, &a::IMP, 6}, {"EOR", &a::EOR, &a::IZX, 6},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 3},
            {"EOR", &a::EOR, &a::ZP0, 3}, {"LSR", &a::LSR, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
            {"PHA", &a::PHA, &a::IMP, 3}, {"EOR", &a::EOR, &a::IMM, 2}, {"LSR", &a::LSR, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 2}, {"JMP", &a::JMP, &a::ABS, 3}, {"EOR", &a::EOR, &a::ABS, 4},
            {"LSR", &a::LSR, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6}, {"BVC", &a::BVC, &a::REL, 2},
            {"EOR", &a::EOR, &a::IZY, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"???", &a::NOP, &a::IMP, 4}, {"EOR", &a::EOR, &a::ZPX, 4}, {"LSR", &a::LSR, &a::ZPX, 6},
            {"???", &a::XXX, &a::IMP, 6}, {"CLI", &a::CLI, &a::IMP, 2}, {"EOR", &a::EOR, &a::ABY, 4},
            {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4},
            {"EOR", &a::EOR, &a::ABX, 4}, {"LSR", &a::LSR, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
            {"RTS", &a::RTS, &a::IMP, 6}, {"ADC", &a::ADC, &a::IZX, 6}, {"???", &a::XXX, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 3}, {"ADC", &a::ADC, &a::ZP0, 3},
            {"ROR", &a::ROR, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5}, {"PLA", &a::PLA, &a::IMP, 4},
            {"ADC", &a::ADC, &a::IMM, 2}, {"ROR", &a::ROR, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
            {"JMP", &a::JMP, &a::IND, 5}, {"ADC", &a::ADC, &a::ABS, 4}, {"ROR", &a::ROR, &a::ABS, 6},
            {"???", &a::XXX, &a::IMP, 6}, {"BVS", &a::BVS, &a::REL, 2}, {"ADC", &a::ADC, &a::IZY, 5},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4},
            {"ADC", &a::ADC, &a::ZPX, 4}, {"ROR", &a::ROR, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"SEI", &a::SEI, &a::IMP, 2}, {"ADC", &a::ADC, &a::ABY, 4}, {"???", &a::NOP, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4}, {"ADC", &a::ADC, &a::ABX, 4},
            {"ROR", &a::ROR, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 2},
            {"STA", &a::STA, &a::IZX, 6}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 6},
            {"STY", &a::STY, &a::ZP0, 3}, {"STA", &a::STA, &a::ZP0, 3}, {"STX", &a::STX, &a::ZP0, 3},
            {"???", &a::XXX, &a::IMP, 3}, {"DEY", &a::DEY, &a::IMP, 2}, {"???", &a::NOP, &a::IMP, 2},
            {"TXA", &a::TXA, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2}, {"STY", &a::STY, &a::ABS, 4},
            {"STA", &a::STA, &a::ABS, 4}, {"STX", &a::STX, &a::ABS, 4}, {"???", &a::XXX, &a::IMP, 4},
            {"BCC", &a::BCC, &a::REL, 2}, {"STA", &a::STA, &a::IZY, 6}, {"???", &a::XXX, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 6}, {"STY", &a::STY, &a::ZPX, 4}, {"STA", &a::STA, &a::ZPX, 4},
            {"STX", &a::STX, &a::ZPY, 4}, {"???", &a::XXX, &a::IMP, 4}, {"TYA", &a::TYA, &a::IMP, 2},
            {"STA", &a::STA, &a::ABY, 5}, {"TXS", &a::TXS, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5},
            {"???", &a::NOP, &a::IMP, 5}, {"STA", &a::STA, &a::ABX, 5}, {"???", &a::XXX, &a::IMP, 5},
            {"???", &a::XXX, &a::IMP, 5}, {"LDY", &a::LDY, &a::IMM, 2}, {"LDA", &a::LDA, &a::IZX, 6},
            {"LDX", &a::LDX, &a::IMM, 2}, {"???", &a::XXX, &a::IMP, 6}, {"LDY", &a::LDY, &a::ZP0, 3},
            {"LDA", &a::LDA, &a::ZP0, 3}, {"LDX", &a::LDX, &a::ZP0, 3}, {"???", &a::XXX, &a::IMP, 3},
            {"TAY", &a::TAY, &a::IMP, 2}, {"LDA", &a::LDA, &a::IMM, 2}, {"TAX", &a::TAX, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 2}, {"LDY", &a::LDY, &a::ABS, 4}, {"LDA", &a::LDA, &a::ABS, 4},
            {"LDX", &a::LDX, &a::ABS, 4}, {"???", &a::XXX, &a::IMP, 4}, {"BCS", &a::BCS, &a::REL, 2},
            {"LDA", &a::LDA, &a::IZY, 5}, {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5},
            {"LDY", &a::LDY, &a::ZPX, 4}, {"LDA", &a::LDA, &a::ZPX, 4}, {"LDX", &a::LDX, &a::ZPY, 4},
            {"???", &a::XXX, &a::IMP, 4}, {"CLV", &a::CLV, &a::IMP, 2}, {"LDA", &a::LDA, &a::ABY, 4},
            {"TSX", &a::TSX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 4}, {"LDY", &a::LDY, &a::ABX, 4},
            {"LDA", &a::LDA, &a::ABX, 4}, {"LDX", &a::LDX, &a::ABY, 4}, {"???", &a::XXX, &a::IMP, 4},
            {"CPY", &a::CPY, &a::IMM, 2}, {"CMP", &a::CMP, &a::IZX, 6}, {"???", &a::NOP, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 8}, {"CPY", &a::CPY, &a::ZP0, 3}, {"CMP", &a::CMP, &a::ZP0, 3},
            {"DEC", &a::DEC, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5}, {"INY", &a::INY, &a::IMP, 2},
            {"CMP", &a::CMP, &a::IMM, 2}, {"DEX", &a::DEX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
            {"CPY", &a::CPY, &a::ABS, 4}, {"CMP", &a::CMP, &a::ABS, 4}, {"DEC", &a::DEC, &a::ABS, 6},
            {"???", &a::XXX, &a::IMP, 6}, {"BNE", &a::BNE, &a::REL, 2}, {"CMP", &a::CMP, &a::IZY, 5},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4},
            {"CMP", &a::CMP, &a::ZPX, 4}, {"DEC", &a::DEC, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"CLD", &a::CLD, &a::IMP, 2}, {"CMP", &a::CMP, &a::ABY, 4}, {"NOP", &a::NOP, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 7}, {"???", &a::NOP, &a::IMP, 4}, {"CMP", &a::CMP, &a::ABX, 4},
            {"DEC", &a::DEC, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7}, {"CPX", &a::CPX, &a::IMM, 2},
            {"SBC", &a::SBC, &a::IZX, 6}, {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"CPX", &a::CPX, &a::ZP0, 3}, {"SBC", &a::SBC, &a::ZP0, 3}, {"INC", &a::INC, &a::ZP0, 5},
            {"???", &a::XXX, &a::IMP, 5}, {"INX", &a::INX, &a::IMP, 2}, {"SBC", &a::SBC, &a::IMM, 2},
            {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::SBC, &a::IMP, 2}, {"CPX", &a::CPX, &a::ABS, 4},
            {"SBC", &a::SBC, &a::ABS, 4}, {"INC", &a::INC, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"BEQ", &a::BEQ, &a::REL, 2}, {"SBC", &a::SBC, &a::IZY, 5}, {"???", &a::XXX, &a::IMP, 2},
            {"???", &a::XXX, &a::IMP, 8}, {"???", &a::NOP, &a::IMP, 4}, {"SBC", &a::SBC, &a::ZPX, 4},
            {"INC", &a::INC, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6}, {"SED", &a::SED, &a::IMP, 2},
            {"SBC", &a::SBC, &a::ABY, 4}, {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
            {"???", &a::NOP, &a::IMP, 4}, {"SBC", &a::SBC, &a::ABX, 4}, {"INC", &a::INC, &a::ABX, 7},
            {"???", &a::XXX, &a::IMP, 7}};
}

void Processor6502::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mCpu = this;
}

uint8_t Processor6502::Read(uint16_t addr) { return mBus->CpuRead(addr); }
uint8_t Processor6502::ReadFromStack(uint16_t addr) { return Read(STACK_BEGIN + addr); }

void Processor6502::Write(uint16_t addr, uint8_t data) { mBus->CpuWrite(addr, data); }
void Processor6502::WriteToStack(uint16_t addr, uint8_t data) { Write(STACK_BEGIN + addr, data); }

uint8_t Processor6502::Fetch() {
  if (!(lookup[opcode].addrMode == &Processor6502::IMP)) {
    fetched = Read(addrAbs);
  }
  return fetched;
}

void Processor6502::Clock() {
  if (cycles == 0) {
    opcode = Read(pc);
    SetFlag(U, true);
    pc++;

    auto instruction = lookup[opcode];
    cycles = lookup[opcode].cycles;

    auto additionalCycle1 = (this->*lookup[opcode].addrMode)();
    auto additionalCycle2 = (this->*lookup[opcode].operate)();

    cycles += (additionalCycle1 & additionalCycle2);

    SetFlag(U, true);
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
  status = 0x00 | U;

  addrRel = 0x0000;
  addrAbs = 0x0000;
  fetched = 0x00;

  cycles = 8;
}

uint8_t Processor6502::GetFlag(FLAGS6502 flag) {
  auto p = (status & flag);
  return ((status & flag) > 0) ? 1 : 0;
}

void Processor6502::SetFlag(FLAGS6502 flag, std::function<bool(void)> value) { Util::SetFlag(status, flag, value); }

void Processor6502::SetFlag(FLAGS6502 flag, bool value) { Util::SetFlag(status, flag, value); }

// Addressing Modes =============================================
uint8_t Processor6502::IMP() {
  fetched = a;
  return 0;
}

uint8_t Processor6502::IMM() {
  addrAbs = pc++;
  return 0;
}

uint8_t Processor6502::ZP0() {
  addrAbs = Read(pc);
  pc++;
  addrAbs &= 0x00FF;
  return 0;
}

uint8_t Processor6502::ZPX() {
  addrAbs = Read(pc) + x;
  pc++;
  addrAbs &= 0x00FF;
  return 0;
}

uint8_t Processor6502::ZPY() {
  addrAbs = Read(pc) + y;
  pc++;
  addrAbs &= 0x00FF;
  return 0;
}

uint8_t Processor6502::REL() {
  addrRel = Read(pc);
  pc++;
  if (addrRel & 0x80) {
    addrRel |= 0xFF00;
  }
  return 0;
}

uint8_t Processor6502::ABS() {
  uint16_t low = Read(pc);
  pc++;
  uint16_t high = Read(pc);
  pc++;

  addrAbs = (high << 8) | low;

  return 0;
}

uint8_t Processor6502::ABX() {
  uint16_t low = Read(pc);
  pc++;
  uint16_t high = Read(pc);
  pc++;

  addrAbs = ((high << 8) | low) + x;

  if ((addrAbs & 0xFF00) != (high << 8)) {
    return 1;
  } else {
    return 0;
  }
}

uint8_t Processor6502::ABY() {
  uint16_t low = Read(pc);
  pc++;
  uint16_t high = Read(pc);
  pc++;

  addrAbs = ((high << 8) | low) + y;

  if ((addrAbs & 0xFF00) != (high << 8)) {
    return 1;
  } else {
    return 0;
  }
}

uint8_t Processor6502::IND() {
  uint16_t ptrLow = Read(pc);
  pc++;
  uint16_t ptrHigh = Read(pc);
  pc++;

  uint16_t ptr = (ptrHigh << 8) | ptrLow;

  if (ptrLow == 0x00FF) {
    addrAbs = (Read(ptr & 0xFF00) << 8) | Read(ptr + 0);
  } else {
    addrAbs = (Read(ptr + 1) << 8) | Read(ptr + 0);
  }
  return 0;
}

uint8_t Processor6502::IZX() {
  uint16_t t = Read(pc);
  pc++;

  uint16_t lo = Read((t + static_cast<uint16_t>(x)) & 0x00FF);
  uint16_t hi = Read((t + static_cast<uint16_t>(x) + 1) & 0x00FF);

  addrAbs = (hi << 8) | lo;

  return 0;
}

uint8_t Processor6502::IZY() {
  uint16_t t = Read(pc);
  pc++;

  uint16_t lo = Read(t & 0x00FF);
  uint16_t hi = Read((t + 1) & 0x00FF);

  addrAbs = ((hi << 8) | lo) + y;

  if ((addrAbs & 0xFF00) != (hi << 8)) {
    return 1;
  } else {
    return 0;
  }
}

// Opcodes ======================================================
uint8_t Processor6502::ADC() {
  Fetch();
  temp = static_cast<uint16_t>(a) + static_cast<uint16_t>(fetched) + static_cast<uint16_t>(GetFlag(C));
  SetFlag(C, temp > 255);
  SetFlag(Z, (temp & 0x00FF) == 0);

  SetFlag(V, (~(static_cast<uint16_t>(a) ^ static_cast<uint16_t>(fetched)) &
              (static_cast<uint16_t>(a) ^ static_cast<uint16_t>(temp))) &
                 0x0080);
  SetFlag(N, temp & 0x80);
  a = temp & 0x00FF;
  return 1;
}

uint8_t Processor6502::AND() {
  Fetch();
  a = a & fetched;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 1;
}

uint8_t Processor6502::ASL() {
  Fetch();
  temp = static_cast<uint16_t>(fetched) << 1;
  SetFlag(C, (temp & 0xFF00) > 0);
  SetFlag(Z, (temp & 0x00FF) == 0x00);
  SetFlag(N, temp & 0x80);

  if (lookup[opcode].addrMode == &Processor6502::IMP) {
    a = temp & 0x00FF;
  } else {
    Write(addrAbs, temp & 0x00FF);
  }
  return 0;
}

uint8_t Processor6502::BCC() {
  if (GetFlag(C) == 0) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
  return 0;
}

uint8_t Processor6502::BCS() {
  if (GetFlag(C) == 1) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
  return 0;
}

uint8_t Processor6502::BEQ() {
  if (GetFlag(Z) == 1) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
  return 0;
}

uint8_t Processor6502::BIT() {
  Fetch();
  temp = a & fetched;
  SetFlag(Z, (temp & 0x00FF) == 0x00);
  SetFlag(N, fetched & (1 << 7));
  SetFlag(V, fetched & (1 << 6));
  return 0;
}

uint8_t Processor6502::BMI() {
  if (GetFlag(N) == 1) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
  return 0;
}

uint8_t Processor6502::BNE() {
  if (GetFlag(Z) == 0) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
  return 0;
}

uint8_t Processor6502::BPL() {
  if (GetFlag(N) == 0) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
  return 0;
}

uint8_t Processor6502::BRK() {
  pc++;

  SetFlag(I, true);
  WriteToStack(stackPointer, (pc >> 8) & 0x00FF);
  stackPointer--;
  WriteToStack(stackPointer, pc & 0x00FF);
  stackPointer--;

  SetFlag(B, true);
  WriteToStack(stackPointer, status);
  stackPointer--;
  SetFlag(B, false);

  pc = static_cast<uint16_t>(Read(0xFFFE)) | (static_cast<uint16_t>(Read(0xFFFF)) << 8);
  return 0;
}

uint8_t Processor6502::BVC() {
  if (GetFlag(V) == 0) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
  return 0;
}

uint8_t Processor6502::BVS() {
  if (GetFlag(V) == 1) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
  return 0;
}

uint8_t Processor6502::CLC() {
  SetFlag(C, false);
  return 0;
}

uint8_t Processor6502::CLD() {
  SetFlag(D, false);
  return 0;
}

uint8_t Processor6502::CLI() {
  SetFlag(I, false);
  return 0;
}

uint8_t Processor6502::CLV() {
  SetFlag(V, false);
  return 0;
}

uint8_t Processor6502::CMP() {
  Fetch();
  temp = static_cast<uint16_t>(a) - static_cast<uint16_t>(fetched);
  SetFlag(C, a >= fetched);
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  return 1;
}

uint8_t Processor6502::CPX() {
  Fetch();
  temp = static_cast<uint16_t>(x) - static_cast<uint16_t>(fetched);
  SetFlag(C, x >= fetched);
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  return 0;
}

uint8_t Processor6502::CPY() {
  Fetch();
  temp = static_cast<uint16_t>(y) - static_cast<uint16_t>(fetched);
  SetFlag(C, y >= fetched);
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  return 0;
}

uint8_t Processor6502::DEC() {
  Fetch();
  temp = fetched - 1;
  Write(addrAbs, temp & 0x00FF);
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  return 0;
}

uint8_t Processor6502::DEX() {
  x--;
  SetFlag(Z, x == 0x00);
  SetFlag(N, x & 0x80);
  return 0;
}

uint8_t Processor6502::DEY() {
  y--;
  SetFlag(Z, y == 0x00);
  SetFlag(N, y & 0x80);
  return 0;
}

uint8_t Processor6502::EOR() {
  Fetch();
  a = a ^ fetched;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 1;
}

uint8_t Processor6502::INC() {
  Fetch();
  temp = fetched + 1;
  Write(addrAbs, temp);
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  return 0;
}

uint8_t Processor6502::INX() {
  x++;
  SetFlag(Z, x == 0x00);
  SetFlag(N, x & 0x80);
  return 0;
}

uint8_t Processor6502::INY() {
  y++;
  SetFlag(Z, y == 0);
  SetFlag(N, y & 0x80);
  return 0;
}

uint8_t Processor6502::JMP() {
  pc = addrAbs;
  return 0;
}

uint8_t Processor6502::JSR() {
  pc--;
  WriteToStack(stackPointer, (pc >> 8) & 0x00FF);
  stackPointer--;
  WriteToStack(stackPointer, pc & 0x00FF);
  stackPointer--;

  pc = addrAbs;
  return 0;
}

uint8_t Processor6502::LDA() {
  Fetch();
  a = fetched;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 1;
}

uint8_t Processor6502::LDX() {
  Fetch();
  x = fetched;
  SetFlag(Z, x == 0x00);
  SetFlag(N, x & 0x80);
  return 1;
}

uint8_t Processor6502::LDY() {
  Fetch();
  y = fetched;
  SetFlag(Z, y == 0x00);
  SetFlag(N, y & 0x80);
  return 1;
}

uint8_t Processor6502::LSR() {
  Fetch();
  SetFlag(C, fetched & 0x0001);
  temp = fetched >> 1;
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);

  if (lookup[opcode].addrMode == &Processor6502::IMP) {
    a = temp & 0x00FF;
  } else {
    Write(addrAbs, temp & 0x00FF);
  }
  return 0;
}

uint8_t Processor6502::NOP() {
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

uint8_t Processor6502::ORA() {
  Fetch();
  a = a | fetched;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 1;
}

uint8_t Processor6502::PHA() {
  WriteToStack(stackPointer, a);
  stackPointer--;
  return 0;
}

uint8_t Processor6502::PHP() {
  WriteToStack(stackPointer, status | B | U);
  stackPointer--;
  SetFlag(B, false);
  SetFlag(U, false);
  return 0;
}

uint8_t Processor6502::PLA() {
  stackPointer++;
  a = ReadFromStack(stackPointer);
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 0;
}

uint8_t Processor6502::PLP() {
  stackPointer++;
  status = ReadFromStack(stackPointer);
  SetFlag(U, true);
  return 0;
}

uint8_t Processor6502::ROL() {
  Fetch();
  temp = static_cast<uint16_t>(fetched << 1) | GetFlag(C);
  SetFlag(C, temp & 0xFF00);
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);

  if (lookup[opcode].addrMode == &Processor6502::IMP) {
    a = temp & 0x00FF;
  } else {
    Write(addrAbs, temp & 0x00FF);
  }
  return 0;
}

uint8_t Processor6502::ROR() {
  Fetch();

  temp = static_cast<uint16_t>(GetFlag(C) << 7) | (fetched >> 1);
  SetFlag(C, fetched & 0x01);
  SetFlag(Z, (temp & 0x00FF) == 0x00);
  SetFlag(N, temp & 0x0080);

  if (lookup[opcode].addrMode == &Processor6502::IMP) {
    a = temp & 0x00FF;
  } else {
    Write(addrAbs, temp & 0x00FF);
  }
  return 0;
}

uint8_t Processor6502::RTI() {
  stackPointer++;
  status = ReadFromStack(stackPointer);
  status &= ~B;
  status &= ~U;

  stackPointer++;
  pc = static_cast<uint16_t>(ReadFromStack(stackPointer));
  stackPointer++;
  pc |= static_cast<uint16_t>(ReadFromStack(stackPointer)) << 8;
  return 0;
}

uint8_t Processor6502::RTS() {
  stackPointer++;
  pc = static_cast<uint16_t>(ReadFromStack(stackPointer));
  stackPointer++;
  pc |= static_cast<uint16_t>(ReadFromStack(stackPointer)) << 8;

  pc++;
  return 0;
}

uint8_t Processor6502::SBC() {
  Fetch();

  uint16_t value = static_cast<uint16_t>(fetched) ^ 0x00FF;

  temp = static_cast<uint16_t>(a) + value + static_cast<uint16_t>(GetFlag(C));
  SetFlag(C, temp & 0xFF00);
  SetFlag(Z, (temp & 0x00FF) == 0);
  SetFlag(V, (temp ^ static_cast<uint16_t>(a)) & (temp ^ value) & 0x0080);
  SetFlag(N, temp & 0x0080);
  a = temp & 0x00FF;
  return 1;
}

uint8_t Processor6502::SEC() {
  SetFlag(C, true);
  return 0;
}

uint8_t Processor6502::SED() {
  SetFlag(D, true);
  return 0;
}

uint8_t Processor6502::SEI() {
  SetFlag(I, true);
  return 0;
}

uint8_t Processor6502::STA() {
  Write(addrAbs, a);
  return 0;
}

uint8_t Processor6502::STX() {
  Write(addrAbs, x);
  return 0;
}

uint8_t Processor6502::STY() {
  Write(addrAbs, y);
  return 0;
}

uint8_t Processor6502::TAX() {
  x = a;
  SetFlag(Z, x == 0x00);
  SetFlag(N, x & 0x80);
  return 0;
}

uint8_t Processor6502::TAY() {
  y = a;
  SetFlag(Z, y == 0x00);
  SetFlag(N, y & 0x80);
  return 0;
}

uint8_t Processor6502::TSX() {
  x = stackPointer;
  SetFlag(Z, x == 0x00);
  SetFlag(N, x & 0x80);
  return 0;
}

uint8_t Processor6502::TXA() {
  a = x;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 0;
}

uint8_t Processor6502::TXS() {
  stackPointer = x;
  return 0;
}

uint8_t Processor6502::TYA() {
  a = y;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 0;
}

uint8_t Processor6502::XXX() { return 0; }

void Processor6502::Interrupt(uint16_t address, uint8_t numCycles) {
  WriteToStack(stackPointer, (pc >> 8) & 0x00FF);
  stackPointer--;
  WriteToStack(stackPointer, pc & 0x00FF);
  stackPointer--;

  SetFlag(B, false);
  SetFlag(U, true);
  SetFlag(I, true);
  WriteToStack(stackPointer, status);
  stackPointer--;

  addrAbs = address;
  uint16_t lo = Read(addrAbs + 0);
  uint16_t hi = Read(addrAbs + 1);
  pc = (hi << 8) | lo;

  cycles = numCycles;
}

void Processor6502::Interrupt() {
  if (GetFlag(I) != 0) {
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
