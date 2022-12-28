#include "Processor6502Proxy.h"

Processor6502Proxy::Processor6502Proxy() : Processor6502() {}

uint16_t Processor6502Proxy::GetAddrAbs() { return addrAbs; }
void Processor6502Proxy::SetAddrAbs(uint16_t theAddrAbs) { addrAbs = theAddrAbs; }
void Processor6502Proxy::SetOpcode(uint8_t theOpcode) { opcode = theOpcode; }

// Addressing Modes =============================================
uint8_t Processor6502Proxy::IMP() { return Processor6502::IMP(); }
uint8_t Processor6502Proxy::IMM() { return Processor6502::IMM(); }
uint8_t Processor6502Proxy::ZP0() { return Processor6502::ZP0(); }
uint8_t Processor6502Proxy::ZPX() { return Processor6502::ZPX(); }
uint8_t Processor6502Proxy::ZPY() { return Processor6502::ZPY(); }
uint8_t Processor6502Proxy::REL() { return Processor6502::REL(); }
uint8_t Processor6502Proxy::ABS() { return Processor6502::ABS(); }
uint8_t Processor6502Proxy::ABX() { return Processor6502::ABX(); }
uint8_t Processor6502Proxy::ABY() { return Processor6502::ABY(); }
uint8_t Processor6502Proxy::IND() { return Processor6502::IND(); }
uint8_t Processor6502Proxy::IZX() { return Processor6502::IZX(); }
uint8_t Processor6502Proxy::IZY() { return Processor6502::IZY(); }

// Opcodes ======================================================
uint8_t Processor6502Proxy::ADC() { return Processor6502::ADC(); }
uint8_t Processor6502Proxy::AND() { return Processor6502::AND(); }
uint8_t Processor6502Proxy::ASL() { return Processor6502::ASL(); }
uint8_t Processor6502Proxy::BCC() { return Processor6502::BCC(); }
uint8_t Processor6502Proxy::BCS() { return Processor6502::BCS(); }
uint8_t Processor6502Proxy::BEQ() { return Processor6502::BEQ(); }
uint8_t Processor6502Proxy::BIT() { return Processor6502::BIT(); }
uint8_t Processor6502Proxy::BMI() { return Processor6502::BMI(); }
uint8_t Processor6502Proxy::BNE() { return Processor6502::BNE(); }
uint8_t Processor6502Proxy::BPL() { return Processor6502::BPL(); }
uint8_t Processor6502Proxy::BRK() { return Processor6502::BRK(); }
uint8_t Processor6502Proxy::BVC() { return Processor6502::BVC(); }
uint8_t Processor6502Proxy::BVS() { return Processor6502::BVS(); }
uint8_t Processor6502Proxy::CLC() { return Processor6502::CLC(); }
uint8_t Processor6502Proxy::CLD() { return Processor6502::CLD(); }
uint8_t Processor6502Proxy::CLI() { return Processor6502::CLI(); }
uint8_t Processor6502Proxy::CLV() { return Processor6502::CLV(); }
uint8_t Processor6502Proxy::CMP() { return Processor6502::CMP(); }
uint8_t Processor6502Proxy::CPX() { return Processor6502::CPX(); }
uint8_t Processor6502Proxy::CPY() { return Processor6502::CPY(); }
uint8_t Processor6502Proxy::DEC() { return Processor6502::DEC(); }
uint8_t Processor6502Proxy::DEX() { return Processor6502::DEX(); }
uint8_t Processor6502Proxy::DEY() { return Processor6502::DEY(); }
uint8_t Processor6502Proxy::EOR() { return Processor6502::EOR(); }
uint8_t Processor6502Proxy::INC() { return Processor6502::INC(); }
uint8_t Processor6502Proxy::INX() { return Processor6502::INX(); }
uint8_t Processor6502Proxy::INY() { return Processor6502::INY(); }
uint8_t Processor6502Proxy::JMP() { return Processor6502::JMP(); }
uint8_t Processor6502Proxy::JSR() { return Processor6502::JSR(); }
uint8_t Processor6502Proxy::LDA() { return Processor6502::LDA(); }
uint8_t Processor6502Proxy::LDX() { return Processor6502::LDX(); }
uint8_t Processor6502Proxy::LDY() { return Processor6502::LDY(); }
uint8_t Processor6502Proxy::LSR() { return Processor6502::LSR(); }
uint8_t Processor6502Proxy::NOP() { return Processor6502::NOP(); }
uint8_t Processor6502Proxy::ORA() { return Processor6502::ORA(); }
uint8_t Processor6502Proxy::PHA() { return Processor6502::PHA(); }
uint8_t Processor6502Proxy::PHP() { return Processor6502::PHP(); }
uint8_t Processor6502Proxy::PLA() { return Processor6502::PLA(); }
uint8_t Processor6502Proxy::PLP() { return Processor6502::PLP(); }
uint8_t Processor6502Proxy::ROL() { return Processor6502::ROL(); }
uint8_t Processor6502Proxy::ROR() { return Processor6502::ROR(); }
uint8_t Processor6502Proxy::RTI() { return Processor6502::RTI(); }
uint8_t Processor6502Proxy::RTS() { return Processor6502::RTS(); }
uint8_t Processor6502Proxy::SBC() { return Processor6502::SBC(); }
uint8_t Processor6502Proxy::SEC() { return Processor6502::SEC(); }
uint8_t Processor6502Proxy::SED() { return Processor6502::SED(); }
uint8_t Processor6502Proxy::SEI() { return Processor6502::SEI(); }
uint8_t Processor6502Proxy::STA() { return Processor6502::STA(); }
uint8_t Processor6502Proxy::STX() { return Processor6502::STX(); }
uint8_t Processor6502Proxy::STY() { return Processor6502::STY(); }
uint8_t Processor6502Proxy::TAX() { return Processor6502::TAX(); }
uint8_t Processor6502Proxy::TAY() { return Processor6502::TAY(); }
uint8_t Processor6502Proxy::TSX() { return Processor6502::TSX(); }
uint8_t Processor6502Proxy::TXA() { return Processor6502::TXA(); }
uint8_t Processor6502Proxy::TXS() { return Processor6502::TXS(); }
uint8_t Processor6502Proxy::TYA() { return Processor6502::TYA(); }
uint8_t Processor6502Proxy::XXX() { return Processor6502::XXX(); }
