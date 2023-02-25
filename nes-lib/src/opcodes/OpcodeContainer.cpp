#include "opcodes/OpcodeContainer.h"
#include "Processor6502.h"

OpcodeContainer::OpcodeContainer(Processor6502* cpu)
    : adc(cpu),
      andd(cpu),
      asl(cpu),
      bcc(cpu),
      bcs(cpu),
      beq(cpu),
      bit(cpu),
      bmi(cpu),
      bne(cpu),
      bpl(cpu),
      brk(cpu),
      bvc(cpu),
      bvs(cpu),
      clc(cpu),
      cld(cpu),
      cli(cpu),
      clv(cpu),
      cmp(cpu),
      cpx(cpu),
      cpy(cpu),
      dec(cpu),
      dex(cpu),
      dey(cpu),
      eor(cpu),
      inc(cpu),
      inx(cpu),
      iny(cpu),
      jmp(cpu),
      jsr(cpu),
      lda(cpu),
      ldx(cpu),
      ldy(cpu),
      lsr(cpu),
      nop(cpu),
      ora(cpu),
      pha(cpu),
      php(cpu),
      pla(cpu),
      plp(cpu),
      rol(cpu),
      ror(cpu),
      rti(cpu),
      rts(cpu),
      sbc(cpu),
      sec(cpu),
      sed(cpu),
      sei(cpu),
      sta(cpu),
      stx(cpu),
      sty(cpu),
      tax(cpu),
      tay(cpu),
      tsx(cpu),
      txa(cpu),
      txs(cpu),
      tya(cpu),
      xxx(cpu) {}