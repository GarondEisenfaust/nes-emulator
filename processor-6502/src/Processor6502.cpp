#include "Processor6502.h"
#include "Bus.h"
#include "Definitions.h"
#include "Util.h"
#include "fmt/format.h"
#include <cstdint>
#include <iostream>

// Constructor
Processor6502::Processor6502() : addressingModes(this), opcodes(this) {
  auto& addrM = addressingModes;

  lookup = {{"BRK", &opcodes.brk, &addrM.imm, 7}, {"ORA", &opcodes.ora, &addrM.izx, 6},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"???", &opcodes.nop, &addrM.imp, 3}, {"ORA", &opcodes.ora, &addrM.zp0, 3},
            {"ASL", &opcodes.asl, &addrM.zp0, 5}, {"???", &opcodes.xxx, &addrM.imp, 5},
            {"PHP", &opcodes.php, &addrM.imp, 3}, {"ORA", &opcodes.ora, &addrM.imm, 2},
            {"ASL", &opcodes.asl, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 2},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"ORA", &opcodes.ora, &addrM.abs, 4},
            {"ASL", &opcodes.asl, &addrM.abs, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"BPL", &opcodes.bpl, &addrM.rel, 2}, {"ORA", &opcodes.ora, &addrM.izy, 5},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"ORA", &opcodes.ora, &addrM.zpx, 4},
            {"ASL", &opcodes.asl, &addrM.zpx, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"CLC", &opcodes.clc, &addrM.imp, 2}, {"ORA", &opcodes.ora, &addrM.aby, 4},
            {"???", &opcodes.nop, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"ORA", &opcodes.ora, &addrM.abx, 4},
            {"ASL", &opcodes.asl, &addrM.abx, 7}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"JSR", &opcodes.jsr, &addrM.abs, 6}, {"AND", &opcodes.andd, &addrM.izx, 6},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"BIT", &opcodes.bit, &addrM.zp0, 3}, {"AND", &opcodes.andd, &addrM.zp0, 3},
            {"ROL", &opcodes.rol, &addrM.zp0, 5}, {"???", &opcodes.xxx, &addrM.imp, 5},
            {"PLP", &opcodes.plp, &addrM.imp, 4}, {"AND", &opcodes.andd, &addrM.imm, 2},
            {"ROL", &opcodes.rol, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 2},
            {"BIT", &opcodes.bit, &addrM.abs, 4}, {"AND", &opcodes.andd, &addrM.abs, 4},
            {"ROL", &opcodes.rol, &addrM.abs, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"BMI", &opcodes.bmi, &addrM.rel, 2}, {"AND", &opcodes.andd, &addrM.izy, 5},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"AND", &opcodes.andd, &addrM.zpx, 4},
            {"ROL", &opcodes.rol, &addrM.zpx, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"SEC", &opcodes.sec, &addrM.imp, 2}, {"AND", &opcodes.andd, &addrM.aby, 4},
            {"???", &opcodes.nop, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"AND", &opcodes.andd, &addrM.abx, 4},
            {"ROL", &opcodes.rol, &addrM.abx, 7}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"RTI", &opcodes.rti, &addrM.imp, 6}, {"EOR", &opcodes.eor, &addrM.izx, 6},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"???", &opcodes.nop, &addrM.imp, 3}, {"EOR", &opcodes.eor, &addrM.zp0, 3},
            {"LSR", &opcodes.lsr, &addrM.zp0, 5}, {"???", &opcodes.xxx, &addrM.imp, 5},
            {"PHA", &opcodes.pha, &addrM.imp, 3}, {"EOR", &opcodes.eor, &addrM.imm, 2},
            {"LSR", &opcodes.lsr, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 2},
            {"JMP", &opcodes.jmp, &addrM.abs, 3}, {"EOR", &opcodes.eor, &addrM.abs, 4},
            {"LSR", &opcodes.lsr, &addrM.abs, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"BVC", &opcodes.bvc, &addrM.rel, 2}, {"EOR", &opcodes.eor, &addrM.izy, 5},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"EOR", &opcodes.eor, &addrM.zpx, 4},
            {"LSR", &opcodes.lsr, &addrM.zpx, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"CLI", &opcodes.cli, &addrM.imp, 2}, {"EOR", &opcodes.eor, &addrM.aby, 4},
            {"???", &opcodes.nop, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"EOR", &opcodes.eor, &addrM.abx, 4},
            {"LSR", &opcodes.lsr, &addrM.abx, 7}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"RTS", &opcodes.rts, &addrM.imp, 6}, {"ADC", &opcodes.adc, &addrM.izx, 6},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"???", &opcodes.nop, &addrM.imp, 3}, {"ADC", &opcodes.adc, &addrM.zp0, 3},
            {"ROR", &opcodes.ror, &addrM.zp0, 5}, {"???", &opcodes.xxx, &addrM.imp, 5},
            {"PLA", &opcodes.pla, &addrM.imp, 4}, {"ADC", &opcodes.adc, &addrM.imm, 2},
            {"ROR", &opcodes.ror, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 2},
            {"JMP", &opcodes.jmp, &addrM.ind, 5}, {"ADC", &opcodes.adc, &addrM.abs, 4},
            {"ROR", &opcodes.ror, &addrM.abs, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"BVS", &opcodes.bvs, &addrM.rel, 2}, {"ADC", &opcodes.adc, &addrM.izy, 5},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"ADC", &opcodes.adc, &addrM.zpx, 4},
            {"ROR", &opcodes.ror, &addrM.zpx, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"SEI", &opcodes.sei, &addrM.imp, 2}, {"ADC", &opcodes.adc, &addrM.aby, 4},
            {"???", &opcodes.nop, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"ADC", &opcodes.adc, &addrM.abx, 4},
            {"ROR", &opcodes.ror, &addrM.abx, 7}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"???", &opcodes.nop, &addrM.imp, 2}, {"STA", &opcodes.sta, &addrM.izx, 6},
            {"???", &opcodes.nop, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"STY", &opcodes.sty, &addrM.zp0, 3}, {"STA", &opcodes.sta, &addrM.zp0, 3},
            {"STX", &opcodes.stx, &addrM.zp0, 3}, {"???", &opcodes.xxx, &addrM.imp, 3},
            {"DEY", &opcodes.dey, &addrM.imp, 2}, {"???", &opcodes.nop, &addrM.imp, 2},
            {"TXA", &opcodes.txa, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 2},
            {"STY", &opcodes.sty, &addrM.abs, 4}, {"STA", &opcodes.sta, &addrM.abs, 4},
            {"STX", &opcodes.stx, &addrM.abs, 4}, {"???", &opcodes.xxx, &addrM.imp, 4},
            {"BCC", &opcodes.bcc, &addrM.rel, 2}, {"STA", &opcodes.sta, &addrM.izy, 6},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"STY", &opcodes.sty, &addrM.zpx, 4}, {"STA", &opcodes.sta, &addrM.zpx, 4},
            {"STX", &opcodes.stx, &addrM.zpy, 4}, {"???", &opcodes.xxx, &addrM.imp, 4},
            {"TYA", &opcodes.tya, &addrM.imp, 2}, {"STA", &opcodes.sta, &addrM.aby, 5},
            {"TXS", &opcodes.txs, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 5},
            {"???", &opcodes.nop, &addrM.imp, 5}, {"STA", &opcodes.sta, &addrM.abx, 5},
            {"???", &opcodes.xxx, &addrM.imp, 5}, {"???", &opcodes.xxx, &addrM.imp, 5},
            {"LDY", &opcodes.ldy, &addrM.imm, 2}, {"LDA", &opcodes.lda, &addrM.izx, 6},
            {"LDX", &opcodes.ldx, &addrM.imm, 2}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"LDY", &opcodes.ldy, &addrM.zp0, 3}, {"LDA", &opcodes.lda, &addrM.zp0, 3},
            {"LDX", &opcodes.ldx, &addrM.zp0, 3}, {"???", &opcodes.xxx, &addrM.imp, 3},
            {"TAY", &opcodes.tay, &addrM.imp, 2}, {"LDA", &opcodes.lda, &addrM.imm, 2},
            {"TAX", &opcodes.tax, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 2},
            {"LDY", &opcodes.ldy, &addrM.abs, 4}, {"LDA", &opcodes.lda, &addrM.abs, 4},
            {"LDX", &opcodes.ldx, &addrM.abs, 4}, {"???", &opcodes.xxx, &addrM.imp, 4},
            {"BCS", &opcodes.bcs, &addrM.rel, 2}, {"LDA", &opcodes.lda, &addrM.izy, 5},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 5},
            {"LDY", &opcodes.ldy, &addrM.zpx, 4}, {"LDA", &opcodes.lda, &addrM.zpx, 4},
            {"LDX", &opcodes.ldx, &addrM.zpy, 4}, {"???", &opcodes.xxx, &addrM.imp, 4},
            {"CLV", &opcodes.clv, &addrM.imp, 2}, {"LDA", &opcodes.lda, &addrM.aby, 4},
            {"TSX", &opcodes.tsx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 4},
            {"LDY", &opcodes.ldy, &addrM.abx, 4}, {"LDA", &opcodes.lda, &addrM.abx, 4},
            {"LDX", &opcodes.ldx, &addrM.aby, 4}, {"???", &opcodes.xxx, &addrM.imp, 4},
            {"CPY", &opcodes.cpy, &addrM.imm, 2}, {"CMP", &opcodes.cmp, &addrM.izx, 6},
            {"???", &opcodes.nop, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"CPY", &opcodes.cpy, &addrM.zp0, 3}, {"CMP", &opcodes.cmp, &addrM.zp0, 3},
            {"DEC", &opcodes.dec, &addrM.zp0, 5}, {"???", &opcodes.xxx, &addrM.imp, 5},
            {"INY", &opcodes.iny, &addrM.imp, 2}, {"CMP", &opcodes.cmp, &addrM.imm, 2},
            {"DEX", &opcodes.dex, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 2},
            {"CPY", &opcodes.cpy, &addrM.abs, 4}, {"CMP", &opcodes.cmp, &addrM.abs, 4},
            {"DEC", &opcodes.dec, &addrM.abs, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"BNE", &opcodes.bne, &addrM.rel, 2}, {"CMP", &opcodes.cmp, &addrM.izy, 5},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"CMP", &opcodes.cmp, &addrM.zpx, 4},
            {"DEC", &opcodes.dec, &addrM.zpx, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"CLD", &opcodes.cld, &addrM.imp, 2}, {"CMP", &opcodes.cmp, &addrM.aby, 4},
            {"NOP", &opcodes.nop, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"CMP", &opcodes.cmp, &addrM.abx, 4},
            {"DEC", &opcodes.dec, &addrM.abx, 7}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"CPX", &opcodes.cpx, &addrM.imm, 2}, {"SBC", &opcodes.sbc, &addrM.izx, 6},
            {"???", &opcodes.nop, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"CPX", &opcodes.cpx, &addrM.zp0, 3}, {"SBC", &opcodes.sbc, &addrM.zp0, 3},
            {"INC", &opcodes.inc, &addrM.zp0, 5}, {"???", &opcodes.xxx, &addrM.imp, 5},
            {"INX", &opcodes.inx, &addrM.imp, 2}, {"SBC", &opcodes.sbc, &addrM.imm, 2},
            {"NOP", &opcodes.nop, &addrM.imp, 2}, {"???", &opcodes.sbc, &addrM.imp, 2},
            {"CPX", &opcodes.cpx, &addrM.abs, 4}, {"SBC", &opcodes.sbc, &addrM.abs, 4},
            {"INC", &opcodes.inc, &addrM.abs, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"BEQ", &opcodes.beq, &addrM.rel, 2}, {"SBC", &opcodes.sbc, &addrM.izy, 5},
            {"???", &opcodes.xxx, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 8},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"SBC", &opcodes.sbc, &addrM.zpx, 4},
            {"INC", &opcodes.inc, &addrM.zpx, 6}, {"???", &opcodes.xxx, &addrM.imp, 6},
            {"SED", &opcodes.sed, &addrM.imp, 2}, {"SBC", &opcodes.sbc, &addrM.aby, 4},
            {"NOP", &opcodes.nop, &addrM.imp, 2}, {"???", &opcodes.xxx, &addrM.imp, 7},
            {"???", &opcodes.nop, &addrM.imp, 4}, {"SBC", &opcodes.sbc, &addrM.abx, 4},
            {"INC", &opcodes.inc, &addrM.abx, 7}, {"???", &opcodes.xxx, &addrM.imp, 7}};
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
    auto additionalCycle2 = (*lookup[opcode].opcode)();

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
