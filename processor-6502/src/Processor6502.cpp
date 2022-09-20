#include "Processor6502.h"
#include "Bus.h"
#include "Definitions.h"
#include "Util.h"
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

void Processor6502::ConnectBus(Bus* bus) { mBus = bus; }

uint8_t Processor6502::Read(uint16_t addr) { return mBus->Read(addr); }

void Processor6502::Write(uint16_t addr, uint8_t data) { mBus->Write(addr, data); }

uint8_t Processor6502::BranchIf(std::function<bool(void)> condition) {
  auto additionalCycle = uint8_t{0};
  if (condition()) {
    additionalCycle = Branch();
  }
  return additionalCycle;
}

uint8_t Processor6502::BranchIf(bool condition) {
  return BranchIf([condition]() { return condition; });
}

uint8_t Processor6502::Branch() {
  addr_abs = pc + addr_rel;
  cycles += 1 + (addr_abs & 0xFF00 != pc & 0xFF00);
  pc = addr_abs;
  return 0;
}

uint8_t Processor6502::Fetch() {
  if (!(lookup[opcode].addrMode == &Processor6502::IMP)) {
    fetched = Read(addr_abs);
  }
  return fetched;
}

void Processor6502::Clock() {
  if (cycles == 0) {
    opcode = Read(pc);
    pc++;
    cycles = lookup[opcode].cycles;

    auto additionalCycle1 = (this->*lookup[opcode].addrMode)();
    auto additionalCycle2 = (this->*lookup[opcode].operate)();

    cycles += (additionalCycle1 & additionalCycle2);
  }
  cycles--;
}

uint8_t Processor6502::GetFlag(FLAGS6502 flag) { return (status & flag) ? 1 : 0; }

void Processor6502::SetFlag(FLAGS6502 flag, std::function<bool(void)> value) { Util::SetFlag(status, flag, value); }

void Processor6502::SetFlag(FLAGS6502 flag, bool value) { Util::SetFlag(status, flag, value); }

// Addressing Modes =============================================
uint8_t Processor6502::IMP() {
  fetched = a;
  return 0;
}

uint8_t Processor6502::IMM() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::ZP0() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::ZPX() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::ZPY() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::REL() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::ABS() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::ABX() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::ABY() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::IND() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::IZX() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::IZY() { throw NOT_IMPLEMENTED_EXCEPTION; }

// Opcodes ======================================================
uint8_t Processor6502::ADC() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::AND() {
  Fetch();
  a = a & fetched;
  SetFlag(Z, a == 0);
  SetFlag(N, a & (1 << 6));
  return 1;
}

uint8_t Processor6502::ASL() {
  Fetch();
  const auto temp = static_cast<uint16_t>(fetched << 1);
  SetFlag(N, temp & (1 << 6));
  SetFlag(Z, temp == 0);
  SetFlag(C, false);

  if (lookup[opcode].addrMode == &Processor6502::IMP) {
    a = temp & 0x00FF;
  } else {
    Write(addr_abs, temp & 0x00FF);
  }
  return 0;
}

uint8_t Processor6502::BCC() { return BranchIf(GetFlag(C) == 0); }

uint8_t Processor6502::BCS() { return BranchIf(GetFlag(C) == 1); }

uint8_t Processor6502::BEQ() { return BranchIf(GetFlag(Z) == 1); }

uint8_t Processor6502::BIT() {
  Fetch();
  const uint8_t temp = fetched & a;
  SetFlag(N, fetched & (1 << 6));
  SetFlag(V, fetched & (1 << 5));
  SetFlag(Z, temp == 0);
  return 0;
}

uint8_t Processor6502::BMI() { return BranchIf(GetFlag(N) == 1); }

uint8_t Processor6502::BNE() { return BranchIf(GetFlag(Z) == 0); }

uint8_t Processor6502::BPL() { return BranchIf(GetFlag(N) == 0); }

uint8_t Processor6502::BRK() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::BVC() { return BranchIf(GetFlag(V) == 0); }

uint8_t Processor6502::BVS() { return BranchIf(GetFlag(V) == 1); }

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
  const uint16_t temp = a - fetched;
  SetFlag(C, a >= fetched);
  SetFlag(N, (temp & (1 << 6)));
  SetFlag(Z, x == fetched);
  return 1;
}

uint8_t Processor6502::CPX() {
  Fetch();
  const uint16_t temp = x - fetched;
  SetFlag(C, x >= fetched);
  SetFlag(N, temp & (1 << 6));
  SetFlag(Z, x == fetched);
  return 0;
}

uint8_t Processor6502::CPY() {
  Fetch();
  const uint16_t temp = y - fetched;
  SetFlag(C, y >= fetched);
  SetFlag(N, temp & (1 << 6));
  SetFlag(Z, y == fetched);
  return 0;
}

uint8_t Processor6502::DEC() {
  Fetch();
  const auto temp = fetched - uint8_t{1};
  Write(addr_abs, temp);
  SetFlag(N, temp & (1 << 6));
  SetFlag(Z, temp == 0);
  return 0;
}

uint8_t Processor6502::DEX() {
  x--;
  SetFlag(N, x & (1 << 6));
  SetFlag(Z, x == 0);
  return 0;
}

uint8_t Processor6502::DEY() {
  y--;
  SetFlag(N, y & (1 << 6));
  SetFlag(Z, y == 0);
  return 0;
}

uint8_t Processor6502::EOR() {
  Fetch();
  a = a ^ fetched;
  SetFlag(Z, a == 0);
  SetFlag(N, y & (1 << 6));
  return 1;
}

uint8_t Processor6502::INC() {
  Fetch();
  auto incremented = fetched;
  incremented++;
  Write(addr_abs, incremented);
  SetFlag(Z, a == 0);
  SetFlag(N, y & (1 << 6));
  return 0;
}

uint8_t Processor6502::INX() {
  x++;
  SetFlag(Z, x == 0);
  SetFlag(N, x & (1 << 6));
  return 0;
}

uint8_t Processor6502::INY() {
  y++;
  SetFlag(Z, y == 0);
  SetFlag(N, y & (1 << 6));
  return 0;
}

uint8_t Processor6502::JMP() {
  pc = addr_abs;
  return 0;
}

uint8_t Processor6502::JSR() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::LDA() {
  Fetch();
  a = fetched;
  SetFlag(Z, a == 0);
  SetFlag(N, a & (1 << 6));
  return 1;
}

uint8_t Processor6502::LDX() {
  Fetch();
  x = fetched;
  SetFlag(Z, x == 0);
  SetFlag(N, x & (1 << 6));
  return 1;
}

uint8_t Processor6502::LDY() {
  Fetch();
  y = fetched;
  SetFlag(Z, y == 0);
  SetFlag(N, y & (1 << 6));
  return 1;
}

uint8_t Processor6502::LSR() {
  Fetch();
  SetFlag(C, fetched & 1);
  uint8_t temp = fetched >> 1;
  SetFlag(Z, temp == 0);
  SetFlag(N, temp & (1 << 6));

  if (lookup[opcode].addrMode == &Processor6502::IMP) {
    a = temp;
  } else {
    Write(addr_abs, temp);
  }
  return 0;
}

uint8_t Processor6502::NOP() { return 0; }

uint8_t Processor6502::ORA() {
  Fetch();
  a = a | fetched;
  SetFlag(Z, a == 0);
  SetFlag(N, a & (1 << 6));
  return 1;
}

uint8_t Processor6502::PHA() {
  Write(stackPointer + 0x0100, a);
  stackPointer--;
  return 0;
}

uint8_t Processor6502::PHP() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::PLA() {
  stackPointer++;
  a = Read(stackPointer + 0x0100);
  SetFlag(Z, a == 0);
  SetFlag(N, a & (1 << 6));
  return 0;
}

uint8_t Processor6502::PLP() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::ROL() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::ROR() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::RTI() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::RTS() { throw NOT_IMPLEMENTED_EXCEPTION; }

uint8_t Processor6502::SBC() { throw NOT_IMPLEMENTED_EXCEPTION; }

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
  Write(addr_abs, a);
  return 0;
}

uint8_t Processor6502::STX() {
  Write(addr_abs, x);
  return 0;
}

uint8_t Processor6502::STY() {
  Write(addr_abs, y);
  return 0;
}

uint8_t Processor6502::TAX() {
  x = a;
  SetFlag(Z, x == 0);
  SetFlag(N, x & (1 << 6));
  return 0;
}

uint8_t Processor6502::TAY() {
  y = a;
  SetFlag(Z, y == 0);
  SetFlag(N, y & (1 << 6));
  return 0;
}

uint8_t Processor6502::TSX() {
  x = stackPointer;
  SetFlag(Z, x == 0);
  SetFlag(N, x & (1 << 6));
  return 0;
}

uint8_t Processor6502::TXA() {
  a = x;
  SetFlag(Z, a == 0);
  SetFlag(N, a & (1 << 6));
  return 0;
}

uint8_t Processor6502::TXS() {
  stackPointer = x;
  return 0;
}

uint8_t Processor6502::TYA() {
  a = y;
  SetFlag(Z, a == 0);
  SetFlag(N, a & (1 << 6));
  return 0;
}

uint8_t Processor6502::XXX() { return 0; }
