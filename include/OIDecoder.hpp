#ifndef OIDECODER_HPP
#define OIDECODER_HPP

#include <memory>
#include <machine.hpp>

namespace dbt {
  namespace OIDecoder {
    enum OIInstType
    { Absd, Add, And, Or, Ldi, Ldihi, Ldw, Addi, Call, Jumpr, Stw, Sltiu, Slti, Jeq, Jne, Jump, Mul, Syscall, Nop, 
      Shr, Null };

    struct OIInst {
      OIInstType Type;
      uint8_t RS, RT, RD, RV;
      int16_t Imm;
      uint32_t Addrs;
    };

    constexpr uint8_t getRS(Word W) {
      return (W.asI_ >> 6) & 0x3F;
    }

    constexpr uint8_t getRT(Word W) {
      return W.asI_ & 0x3F;
    }

    constexpr uint8_t getRD(Word W) {
      return (W.asI_ >> 12) & 0x3F;
    }

    constexpr uint8_t getRV(Word W) {
      return (W.asI_ >> 18) & 0x3F;
    }

    static int16_t getImm(Word W) {
      uint16_t x = (W.asI_ >> 6) & 0x3FFF;
      return ((x >= (1 << 13))
          ? -(16384 - x) 
          : x);
    }

    static int16_t getImm1(Word W) {
      uint16_t x = (W.asI_ >> 12) & 0x3FFF;
      return ((x >= (1 << 13))
          ? -(16384 - x) 
          : x);
    }

    static int16_t getImm2(Word W) {
      uint16_t x = (W.asI_ >> 26) & 0x3FFF;
      return ((x >= (1 << 13))
          ? -(16384 - x) 
          : x);
    }

    constexpr int32_t getPL18(Word W) {
      return W.asI_ & 0x3FFFF; 
    }

    constexpr uint32_t getAddr(Word W) {
      return W.asI_ & 0xFFFFF;
    }

    constexpr uint32_t getLAddr(Word W) {
      return W.asI_ & 0x3FFFFFF;
    }

    static OIInst decode(uint32_t CodedInst) {
      Word W;
      W.asI_ = CodedInst;
      uint8_t Op = W.asI_ >> 26;
      constexpr unsigned OpMask = 0x3FFFFFF;

      OIInst I;

      if (CodedInst == 0) {
        I.Type = OIInstType::Nop;
        return I;
      }

      uint8_t Ext;
      switch(Op) {
      case 0b100010:
        Ext = (W.asI_ & OpMask) >> 12;
        if (Ext == 0b101) I.Type = OIInstType::Absd;
        I.RS = getRS(W);
        I.RT = getRT(W);
        break;
      case 0b100000:
        Ext = W.asI_ >> 18;
        if (Ext == 0b0) I.Type = OIInstType::Add;
        if (Ext == 0b1) {
          I.Type = OIInstType::Ldihi;
          I.Addrs = getPL18(W);
        }
        if (Ext == 0b110) I.Type = OIInstType::And;
        if (Ext == 0b111) I.Type = OIInstType::Or;
        if (Ext == 0b1011) I.Type = OIInstType::Shr;
        I.RS = getRS(W);
        I.RT = getRT(W);
        I.RD = getRD(W);
        break;
      case 0b11111: 
        Ext = (W.asI_ & OpMask) >> 20;
        if (Ext == 0b100) I.Type = OIInstType::Ldi;
        I.RT = getRT(W);
        I.Imm = getImm(W);
        break;
      case 0b110:
        I.Type = OIInstType::Ldw;
        I.RS = getRS(W);
        I.RT = getRT(W);
        I.Imm = getImm1(W);
        break;
      case 0b1110:
        I.Type = OIInstType::Addi;
        I.RT = getRT(W);
        I.RS = getRS(W);
        I.Imm = getImm1(W);
        break;
      case 0b1:
        I.Type = OIInstType::Call;
        I.Addrs = getAddr(W);
        break;
      case 0b100011:
        I.Type = OIInstType::Jumpr;
        I.RT = getRT(W);
        break;
      case 0b1011:
        I.Type = OIInstType::Stw;
        I.RS = getRS(W);
        I.RT = getRT(W);
        I.Imm = getImm1(W);
        break;
      case 0b10001:
        I.Type = OIInstType::Sltiu;
        I.RS = getRS(W);
        I.RT = getRT(W);
        I.Imm = getImm2(W);
        break;
      case 0b10000:
        I.Type = OIInstType::Slti;
        I.RS = getRS(W);
        I.RT = getRT(W);
        I.Imm = getImm2(W);
        break;
      case 0b10101:
        I.Type = OIInstType::Jeq;
        I.RS = getRS(W);
        I.RT = getRT(W);
        I.Imm = getImm1(W);
        break;
      case 0b10110:
        I.Type = OIInstType::Jne;
        I.RS = getRS(W);
        I.RT = getRT(W);
        I.Imm = getImm1(W);
        break;
      case 0b0:
        I.Type = OIInstType::Jump;
        I.Addrs = getLAddr(W);
        break;
      case 0b011101:
        I.Type = OIInstType::Mul;
        I.RV = getRV(W);
        I.RD = getRD(W);
        I.RS = getRS(W);
        I.RT = getRT(W);
        break;
      case 0b100100:
        I.Type = OIInstType::Syscall;
        break;
      default:
        I.Type = OIInstType::Null;
      }
      return I;
    }
  }
}

#endif
