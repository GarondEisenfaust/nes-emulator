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

  lookup = {{&opcodes.brk, &addrM.imm, 7},  {&opcodes.ora, &addrM.izx, 6},  {&opcodes.xxx, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 8},  {&opcodes.nop, &addrM.imp, 3},  {&opcodes.ora, &addrM.zp0, 3},
            {&opcodes.asl, &addrM.zp0, 5},  {&opcodes.xxx, &addrM.imp, 5},  {&opcodes.php, &addrM.imp, 3},
            {&opcodes.ora, &addrM.imm, 2},  {&opcodes.asl, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 2},
            {&opcodes.nop, &addrM.imp, 4},  {&opcodes.ora, &addrM.abs, 4},  {&opcodes.asl, &addrM.abs, 6},
            {&opcodes.xxx, &addrM.imp, 6},  {&opcodes.bpl, &addrM.rel, 2},  {&opcodes.ora, &addrM.izy, 5},
            {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 8},  {&opcodes.nop, &addrM.imp, 4},
            {&opcodes.ora, &addrM.zpx, 4},  {&opcodes.asl, &addrM.zpx, 6},  {&opcodes.xxx, &addrM.imp, 6},
            {&opcodes.clc, &addrM.imp, 2},  {&opcodes.ora, &addrM.aby, 4},  {&opcodes.nop, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 7},  {&opcodes.nop, &addrM.imp, 4},  {&opcodes.ora, &addrM.abx, 4},
            {&opcodes.asl, &addrM.abx, 7},  {&opcodes.xxx, &addrM.imp, 7},  {&opcodes.jsr, &addrM.abs, 6},
            {&opcodes.andd, &addrM.izx, 6}, {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 8},
            {&opcodes.bit, &addrM.zp0, 3},  {&opcodes.andd, &addrM.zp0, 3}, {&opcodes.rol, &addrM.zp0, 5},
            {&opcodes.xxx, &addrM.imp, 5},  {&opcodes.plp, &addrM.imp, 4},  {&opcodes.andd, &addrM.imm, 2},
            {&opcodes.rol, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.bit, &addrM.abs, 4},
            {&opcodes.andd, &addrM.abs, 4}, {&opcodes.rol, &addrM.abs, 6},  {&opcodes.xxx, &addrM.imp, 6},
            {&opcodes.bmi, &addrM.rel, 2},  {&opcodes.andd, &addrM.izy, 5}, {&opcodes.xxx, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 8},  {&opcodes.nop, &addrM.imp, 4},  {&opcodes.andd, &addrM.zpx, 4},
            {&opcodes.rol, &addrM.zpx, 6},  {&opcodes.xxx, &addrM.imp, 6},  {&opcodes.sec, &addrM.imp, 2},
            {&opcodes.andd, &addrM.aby, 4}, {&opcodes.nop, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 7},
            {&opcodes.nop, &addrM.imp, 4},  {&opcodes.andd, &addrM.abx, 4}, {&opcodes.rol, &addrM.abx, 7},
            {&opcodes.xxx, &addrM.imp, 7},  {&opcodes.rti, &addrM.imp, 6},  {&opcodes.eor, &addrM.izx, 6},
            {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 8},  {&opcodes.nop, &addrM.imp, 3},
            {&opcodes.eor, &addrM.zp0, 3},  {&opcodes.lsr, &addrM.zp0, 5},  {&opcodes.xxx, &addrM.imp, 5},
            {&opcodes.pha, &addrM.imp, 3},  {&opcodes.eor, &addrM.imm, 2},  {&opcodes.lsr, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.jmp, &addrM.abs, 3},  {&opcodes.eor, &addrM.abs, 4},
            {&opcodes.lsr, &addrM.abs, 6},  {&opcodes.xxx, &addrM.imp, 6},  {&opcodes.bvc, &addrM.rel, 2},
            {&opcodes.eor, &addrM.izy, 5},  {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 8},
            {&opcodes.nop, &addrM.imp, 4},  {&opcodes.eor, &addrM.zpx, 4},  {&opcodes.lsr, &addrM.zpx, 6},
            {&opcodes.xxx, &addrM.imp, 6},  {&opcodes.cli, &addrM.imp, 2},  {&opcodes.eor, &addrM.aby, 4},
            {&opcodes.nop, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 7},  {&opcodes.nop, &addrM.imp, 4},
            {&opcodes.eor, &addrM.abx, 4},  {&opcodes.lsr, &addrM.abx, 7},  {&opcodes.xxx, &addrM.imp, 7},
            {&opcodes.rts, &addrM.imp, 6},  {&opcodes.adc, &addrM.izx, 6},  {&opcodes.xxx, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 8},  {&opcodes.nop, &addrM.imp, 3},  {&opcodes.adc, &addrM.zp0, 3},
            {&opcodes.ror, &addrM.zp0, 5},  {&opcodes.xxx, &addrM.imp, 5},  {&opcodes.pla, &addrM.imp, 4},
            {&opcodes.adc, &addrM.imm, 2},  {&opcodes.ror, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 2},
            {&opcodes.jmp, &addrM.ind, 5},  {&opcodes.adc, &addrM.abs, 4},  {&opcodes.ror, &addrM.abs, 6},
            {&opcodes.xxx, &addrM.imp, 6},  {&opcodes.bvs, &addrM.rel, 2},  {&opcodes.adc, &addrM.izy, 5},
            {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 8},  {&opcodes.nop, &addrM.imp, 4},
            {&opcodes.adc, &addrM.zpx, 4},  {&opcodes.ror, &addrM.zpx, 6},  {&opcodes.xxx, &addrM.imp, 6},
            {&opcodes.sei, &addrM.imp, 2},  {&opcodes.adc, &addrM.aby, 4},  {&opcodes.nop, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 7},  {&opcodes.nop, &addrM.imp, 4},  {&opcodes.adc, &addrM.abx, 4},
            {&opcodes.ror, &addrM.abx, 7},  {&opcodes.xxx, &addrM.imp, 7},  {&opcodes.nop, &addrM.imp, 2},
            {&opcodes.sta, &addrM.izx, 6},  {&opcodes.nop, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 6},
            {&opcodes.sty, &addrM.zp0, 3},  {&opcodes.sta, &addrM.zp0, 3},  {&opcodes.stx, &addrM.zp0, 3},
            {&opcodes.xxx, &addrM.imp, 3},  {&opcodes.dey, &addrM.imp, 2},  {&opcodes.nop, &addrM.imp, 2},
            {&opcodes.txa, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.sty, &addrM.abs, 4},
            {&opcodes.sta, &addrM.abs, 4},  {&opcodes.stx, &addrM.abs, 4},  {&opcodes.xxx, &addrM.imp, 4},
            {&opcodes.bcc, &addrM.rel, 2},  {&opcodes.sta, &addrM.izy, 6},  {&opcodes.xxx, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 6},  {&opcodes.sty, &addrM.zpx, 4},  {&opcodes.sta, &addrM.zpx, 4},
            {&opcodes.stx, &addrM.zpy, 4},  {&opcodes.xxx, &addrM.imp, 4},  {&opcodes.tya, &addrM.imp, 2},
            {&opcodes.sta, &addrM.aby, 5},  {&opcodes.txs, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 5},
            {&opcodes.nop, &addrM.imp, 5},  {&opcodes.sta, &addrM.abx, 5},  {&opcodes.xxx, &addrM.imp, 5},
            {&opcodes.xxx, &addrM.imp, 5},  {&opcodes.ldy, &addrM.imm, 2},  {&opcodes.lda, &addrM.izx, 6},
            {&opcodes.ldx, &addrM.imm, 2},  {&opcodes.xxx, &addrM.imp, 6},  {&opcodes.ldy, &addrM.zp0, 3},
            {&opcodes.lda, &addrM.zp0, 3},  {&opcodes.ldx, &addrM.zp0, 3},  {&opcodes.xxx, &addrM.imp, 3},
            {&opcodes.tay, &addrM.imp, 2},  {&opcodes.lda, &addrM.imm, 2},  {&opcodes.tax, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.ldy, &addrM.abs, 4},  {&opcodes.lda, &addrM.abs, 4},
            {&opcodes.ldx, &addrM.abs, 4},  {&opcodes.xxx, &addrM.imp, 4},  {&opcodes.bcs, &addrM.rel, 2},
            {&opcodes.lda, &addrM.izy, 5},  {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 5},
            {&opcodes.ldy, &addrM.zpx, 4},  {&opcodes.lda, &addrM.zpx, 4},  {&opcodes.ldx, &addrM.zpy, 4},
            {&opcodes.xxx, &addrM.imp, 4},  {&opcodes.clv, &addrM.imp, 2},  {&opcodes.lda, &addrM.aby, 4},
            {&opcodes.tsx, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 4},  {&opcodes.ldy, &addrM.abx, 4},
            {&opcodes.lda, &addrM.abx, 4},  {&opcodes.ldx, &addrM.aby, 4},  {&opcodes.xxx, &addrM.imp, 4},
            {&opcodes.cpy, &addrM.imm, 2},  {&opcodes.cmp, &addrM.izx, 6},  {&opcodes.nop, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 8},  {&opcodes.cpy, &addrM.zp0, 3},  {&opcodes.cmp, &addrM.zp0, 3},
            {&opcodes.dec, &addrM.zp0, 5},  {&opcodes.xxx, &addrM.imp, 5},  {&opcodes.iny, &addrM.imp, 2},
            {&opcodes.cmp, &addrM.imm, 2},  {&opcodes.dex, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 2},
            {&opcodes.cpy, &addrM.abs, 4},  {&opcodes.cmp, &addrM.abs, 4},  {&opcodes.dec, &addrM.abs, 6},
            {&opcodes.xxx, &addrM.imp, 6},  {&opcodes.bne, &addrM.rel, 2},  {&opcodes.cmp, &addrM.izy, 5},
            {&opcodes.xxx, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 8},  {&opcodes.nop, &addrM.imp, 4},
            {&opcodes.cmp, &addrM.zpx, 4},  {&opcodes.dec, &addrM.zpx, 6},  {&opcodes.xxx, &addrM.imp, 6},
            {&opcodes.cld, &addrM.imp, 2},  {&opcodes.cmp, &addrM.aby, 4},  {&opcodes.nop, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 7},  {&opcodes.nop, &addrM.imp, 4},  {&opcodes.cmp, &addrM.abx, 4},
            {&opcodes.dec, &addrM.abx, 7},  {&opcodes.xxx, &addrM.imp, 7},  {&opcodes.cpx, &addrM.imm, 2},
            {&opcodes.sbc, &addrM.izx, 6},  {&opcodes.nop, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 8},
            {&opcodes.cpx, &addrM.zp0, 3},  {&opcodes.sbc, &addrM.zp0, 3},  {&opcodes.inc, &addrM.zp0, 5},
            {&opcodes.xxx, &addrM.imp, 5},  {&opcodes.inx, &addrM.imp, 2},  {&opcodes.sbc, &addrM.imm, 2},
            {&opcodes.nop, &addrM.imp, 2},  {&opcodes.sbc, &addrM.imp, 2},  {&opcodes.cpx, &addrM.abs, 4},
            {&opcodes.sbc, &addrM.abs, 4},  {&opcodes.inc, &addrM.abs, 6},  {&opcodes.xxx, &addrM.imp, 6},
            {&opcodes.beq, &addrM.rel, 2},  {&opcodes.sbc, &addrM.izy, 5},  {&opcodes.xxx, &addrM.imp, 2},
            {&opcodes.xxx, &addrM.imp, 8},  {&opcodes.nop, &addrM.imp, 4},  {&opcodes.sbc, &addrM.zpx, 4},
            {&opcodes.inc, &addrM.zpx, 6},  {&opcodes.xxx, &addrM.imp, 6},  {&opcodes.sed, &addrM.imp, 2},
            {&opcodes.sbc, &addrM.aby, 4},  {&opcodes.nop, &addrM.imp, 2},  {&opcodes.xxx, &addrM.imp, 7},
            {&opcodes.nop, &addrM.imp, 4},  {&opcodes.sbc, &addrM.abx, 4},  {&opcodes.inc, &addrM.abx, 7},
            {&opcodes.xxx, &addrM.imp, 7}};
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
    auto opcode = mBus->CpuRead(current, true);

    current++;
    auto& instruction = lookup[opcode];
    auto name = instruction.opcode->Name();
    auto addressingMode = instruction.addrMode->Disassemble(current);
    auto instructionAsString = fmt::format("{:#06x}: {} {}", line, name, addressingMode);
    disassembledCode[line] = instructionAsString;
  }
  return disassembledCode;
}
