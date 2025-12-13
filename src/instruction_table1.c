#include "instruction_table1.h"
#include "instructions1.h"

const instruction1_handler_t instruction_table1[256] = {
    /* 0x00–0x0F */
    [0x00] = OP_0b00000000, /* NOP */
    [0x01] = OP_0b00xx0001, /* LD BC,nn */
    [0x02] = OP_0b00000010, /* LD (BC),A */
    [0x03] = OP_0b00xx0011, /* INC BC */
    [0x04] = OP_0b00xxx100, /* INC B */
    [0x05] = OP_0b00xxx101, /* DEC B */
    [0x06] = OP_0b00xxx110, /* LD B,n */
    [0x07] = OP_0b00000111, /* RLCA */
    [0x08] = OP_0b00001000, /* LD (nn),SP */
    [0x09] = OP_0b00xx1001, /* ADD HL,BC */
    [0x0A] = OP_0b00001010, /* LD A,(BC) */
    [0x0B] = OP_0b00xx1011, /* DEC BC */
    [0x0C] = OP_0b00xxx100, /* INC C */
    [0x0D] = OP_0b00xxx101, /* DEC C */
    [0x0E] = OP_0b00xxx110, /* LD C,n */
    [0x0F] = OP_0b00001111, /* RRCA */

    /* 0x10–0x1F */
    [0x10] = OP_0b00010000, /* STOP */
    [0x11] = OP_0b00xx0001, /* LD DE,nn */
    [0x12] = OP_0b00010010, /* LD (DE),A */
    [0x13] = OP_0b00xx0011, /* INC DE */
    [0x14] = OP_0b00xxx100, /* INC D */
    [0x15] = OP_0b00xxx101, /* DEC D */
    [0x16] = OP_0b00xxx110, /* LD D,n */
    [0x17] = OP_0b00010111, /* RLA */
    [0x18] = OP_0b00011000, /* JR e */
    [0x19] = OP_0b00xx1001, /* ADD HL,DE */
    [0x1A] = OP_0b00011010, /* LD A,(DE) */
    [0x1B] = OP_0b00xx1011, /* DEC DE */
    [0x1C] = OP_0b00xxx100, /* INC E */
    [0x1D] = OP_0b00xxx101, /* DEC E */
    [0x1E] = OP_0b00xxx110, /* LD E,n */
    [0x1F] = OP_0b00011111, /* RRA */

    /* 0x20–0x2F */
    [0x20] = OP_0b001xx000, /* JR NZ,e */
    [0x21] = OP_0b00xx0001, /* LD HL,nn */
    [0x22] = OP_0b00100010, /* LD (HL+),A */
    [0x23] = OP_0b00xx0011, /* INC HL */
    [0x24] = OP_0b00xxx100, /* INC H */
    [0x25] = OP_0b00xxx101, /* DEC H */
    [0x26] = OP_0b00xxx110, /* LD H,n */
    [0x27] = OP_0b00100111, /* DAA */
    [0x28] = OP_0b001xx000, /* JR Z,e */
    [0x29] = OP_0b00xx1001, /* ADD HL,HL */
    [0x2A] = OP_0b00101010, /* LD A,(HL+) */
    [0x2B] = OP_0b00xx1011, /* DEC HL */
    [0x2C] = OP_0b00xxx100, /* INC L */
    [0x2D] = OP_0b00xxx101, /* DEC L */
    [0x2E] = OP_0b00xxx110, /* LD L,n */
    [0x2F] = OP_0b00101111, /* CPL */

    /* 0x30–0x3F */
    [0x30] = OP_0b001xx000, /* JR NC,e */
    [0x31] = OP_0b00xx0001, /* LD SP,nn */
    [0x32] = OP_0b00110010, /* LD (HL-),A */
    [0x33] = OP_0b00xx0011, /* INC SP */
    [0x34] = OP_0b00110100, /* INC (HL) */
    [0x35] = OP_0b00110101, /* DEC (HL) */
    [0x36] = OP_0b00110110, /* LD (HL),n */
    [0x37] = OP_0b00110111, /* SCF */
    [0x38] = OP_0b001xx000, /* JR C,e */
    [0x39] = OP_0b00xx1001, /* ADD HL,SP */
    [0x3A] = OP_0b00111010, /* LD A,(HL-) */
    [0x3B] = OP_0b00xx1011, /* DEC SP */
    [0x3C] = OP_0b00xxx100, /* INC A */
    [0x3D] = OP_0b00xxx101, /* DEC A */
    [0x3E] = OP_0b00xxx110, /* LD A,n */
    [0x3F] = OP_0b00111111, /* CCF */

    /* 0x40–0x4F : LD r,r' / LD r,(HL) */
    [0x40] = OP_0b01xxxyyy, /* LD B,B */
    [0x41] = OP_0b01xxxyyy, /* LD B,C */
    [0x42] = OP_0b01xxxyyy, /* LD B,D */
    [0x43] = OP_0b01xxxyyy, /* LD B,E */
    [0x44] = OP_0b01xxxyyy, /* LD B,H */
    [0x45] = OP_0b01xxxyyy, /* LD B,L */
    [0x46] = OP_0b01xxx110, /* LD B,(HL) */
    [0x47] = OP_0b01xxxyyy, /* LD B,A */
    [0x48] = OP_0b01xxxyyy, /* LD C,B */
    [0x49] = OP_0b01xxxyyy, /* LD C,C */
    [0x4A] = OP_0b01xxxyyy, /* LD C,D */
    [0x4B] = OP_0b01xxxyyy, /* LD C,E */
    [0x4C] = OP_0b01xxxyyy, /* LD C,H */
    [0x4D] = OP_0b01xxxyyy, /* LD C,L */
    [0x4E] = OP_0b01xxx110, /* LD C,(HL) */
    [0x4F] = OP_0b01xxxyyy, /* LD C,A */

    /* 0x50–0x5F */
    [0x50] = OP_0b01xxxyyy, /* LD D,B */
    [0x51] = OP_0b01xxxyyy, /* LD D,C */
    [0x52] = OP_0b01xxxyyy, /* LD D,D */
    [0x53] = OP_0b01xxxyyy, /* LD D,E */
    [0x54] = OP_0b01xxxyyy, /* LD D,H */
    [0x55] = OP_0b01xxxyyy, /* LD D,L */
    [0x56] = OP_0b01xxx110, /* LD D,(HL) */
    [0x57] = OP_0b01xxxyyy, /* LD D,A */
    [0x58] = OP_0b01xxxyyy, /* LD E,B */
    [0x59] = OP_0b01xxxyyy, /* LD E,C */
    [0x5A] = OP_0b01xxxyyy, /* LD E,D */
    [0x5B] = OP_0b01xxxyyy, /* LD E,E */
    [0x5C] = OP_0b01xxxyyy, /* LD E,H */
    [0x5D] = OP_0b01xxxyyy, /* LD E,L */
    [0x5E] = OP_0b01xxx110, /* LD E,(HL) */
    [0x5F] = OP_0b01xxxyyy, /* LD E,A */

    /* 0x60–0x6F */
    [0x60] = OP_0b01xxxyyy, /* LD H,B */
    [0x61] = OP_0b01xxxyyy, /* LD H,C */
    [0x62] = OP_0b01xxxyyy, /* LD H,D */
    [0x63] = OP_0b01xxxyyy, /* LD H,E */
    [0x64] = OP_0b01xxxyyy, /* LD H,H */
    [0x65] = OP_0b01xxxyyy, /* LD H,L */
    [0x66] = OP_0b01xxx110, /* LD H,(HL) */
    [0x67] = OP_0b01xxxyyy, /* LD H,A */
    [0x68] = OP_0b01xxxyyy, /* LD L,B */
    [0x69] = OP_0b01xxxyyy, /* LD L,C */
    [0x6A] = OP_0b01xxxyyy, /* LD L,D */
    [0x6B] = OP_0b01xxxyyy, /* LD L,E */
    [0x6C] = OP_0b01xxxyyy, /* LD L,H */
    [0x6D] = OP_0b01xxxyyy, /* LD L,L */
    [0x6E] = OP_0b01xxx110, /* LD L,(HL) */
    [0x6F] = OP_0b01xxxyyy, /* LD L,A */

    /* 0x70–0x7F */
    [0x70] = OP_0b01110xxx, /* LD (HL),B */
    [0x71] = OP_0b01110xxx, /* LD (HL),C */
    [0x72] = OP_0b01110xxx, /* LD (HL),D */
    [0x73] = OP_0b01110xxx, /* LD (HL),E */
    [0x74] = OP_0b01110xxx, /* LD (HL),H */
    [0x75] = OP_0b01110xxx, /* LD (HL),L */
    [0x76] = OP_0b01110110, /* HALT */
    [0x77] = OP_0b01110xxx, /* LD (HL),A */
    [0x78] = OP_0b01xxxyyy, /* LD A,B */
    [0x79] = OP_0b01xxxyyy, /* LD A,C */
    [0x7A] = OP_0b01xxxyyy, /* LD A,D */
    [0x7B] = OP_0b01xxxyyy, /* LD A,E */
    [0x7C] = OP_0b01xxxyyy, /* LD A,H */
    [0x7D] = OP_0b01xxxyyy, /* LD A,L */
    [0x7E] = OP_0b01xxx110, /* LD A,(HL) */
    [0x7F] = OP_0b01xxxyyy, /* LD A,A */

    /* 0x80–0x8F : ADD / ADC */
    [0x80] = OP_0b10000xxx, /* ADD A,B */
    [0x81] = OP_0b10000xxx, /* ADD A,C */
    [0x82] = OP_0b10000xxx, /* ADD A,D */
    [0x83] = OP_0b10000xxx, /* ADD A,E */
    [0x84] = OP_0b10000xxx, /* ADD A,H */
    [0x85] = OP_0b10000xxx, /* ADD A,L */
    [0x86] = OP_0b10000110, /* ADD A,(HL) */
    [0x87] = OP_0b10000xxx, /* ADD A,A */
    [0x88] = OP_0b10001xxx, /* ADC A,B */
    [0x89] = OP_0b10001xxx, /* ADC A,C */
    [0x8A] = OP_0b10001xxx, /* ADC A,D */
    [0x8B] = OP_0b10001xxx, /* ADC A,E */
    [0x8C] = OP_0b10001xxx, /* ADC A,H */
    [0x8D] = OP_0b10001xxx, /* ADC A,L */
    [0x8E] = OP_0b10001110, /* ADC A,(HL) */
    [0x8F] = OP_0b10001xxx, /* ADC A,A */

    /* 0x90–0x9F : SUB / SBC */
    [0x90] = OP_0b10010xxx, /* SUB B */
    [0x91] = OP_0b10010xxx, /* SUB C */
    [0x92] = OP_0b10010xxx, /* SUB D */
    [0x93] = OP_0b10010xxx, /* SUB E */
    [0x94] = OP_0b10010xxx, /* SUB H */
    [0x95] = OP_0b10010xxx, /* SUB L */
    [0x96] = OP_0b10010110, /* SUB (HL) */
    [0x97] = OP_0b10010xxx, /* SUB A */
    [0x98] = OP_0b10011xxx, /* SBC B */
    [0x99] = OP_0b10011xxx, /* SBC C */
    [0x9A] = OP_0b10011xxx, /* SBC D */
    [0x9B] = OP_0b10011xxx, /* SBC E */
    [0x9C] = OP_0b10011xxx, /* SBC H */
    [0x9D] = OP_0b10011xxx, /* SBC L */
    [0x9E] = OP_0b10011110, /* SBC (HL) */
    [0x9F] = OP_0b10011xxx, /* SBC A */

    /* 0xA0–0xAF : AND / XOR */
    [0xA0] = OP_0b10100xxx, /* AND B */
    [0xA1] = OP_0b10100xxx, /* AND C */
    [0xA2] = OP_0b10100xxx, /* AND D */
    [0xA3] = OP_0b10100xxx, /* AND E */
    [0xA4] = OP_0b10100xxx, /* AND H */
    [0xA5] = OP_0b10100xxx, /* AND L */
    [0xA6] = OP_0b10100110, /* AND (HL) */
    [0xA7] = OP_0b10100xxx, /* AND A */
    [0xA8] = OP_0b10101xxx, /* XOR B */
    [0xA9] = OP_0b10101xxx, /* XOR C */
    [0xAA] = OP_0b10101xxx, /* XOR D */
    [0xAB] = OP_0b10101xxx, /* XOR E */
    [0xAC] = OP_0b10101xxx, /* XOR H */
    [0xAD] = OP_0b10101xxx, /* XOR L */
    [0xAE] = OP_0b10101110, /* XOR (HL) */
    [0xAF] = OP_0b10101xxx, /* XOR A */

    /* 0xB0–0xBF : OR / CP */
    [0xB0] = OP_0b10110xxx, /* OR B */
    [0xB1] = OP_0b10110xxx, /* OR C */
    [0xB2] = OP_0b10110xxx, /* OR D */
    [0xB3] = OP_0b10110xxx, /* OR E */
    [0xB4] = OP_0b10110xxx, /* OR H */
    [0xB5] = OP_0b10110xxx, /* OR L */
    [0xB6] = OP_0b10110110, /* OR (HL) */
    [0xB7] = OP_0b10110xxx, /* OR A */
    [0xB8] = OP_0b10111xxx, /* CP B */
    [0xB9] = OP_0b10111xxx, /* CP C */
    [0xBA] = OP_0b10111xxx, /* CP D */
    [0xBB] = OP_0b10111xxx, /* CP E */
    [0xBC] = OP_0b10111xxx, /* CP H */
    [0xBD] = OP_0b10111xxx, /* CP L */
    [0xBE] = OP_0b10111110, /* CP (HL) */
    [0xBF] = OP_0b10111xxx, /* CP A */

    /* 0xC0–0xCF */
    [0xC0] = OP_0b110xx000, /* RET NZ */
    [0xC1] = OP_0b11xx0001, /* POP BC */
    [0xC2] = OP_0b110xx010, /* JP NZ,nn */
    [0xC3] = OP_0b11000011, /* JP nn */
    [0xC4] = OP_0b110xx100, /* CALL NZ,nn */
    [0xC5] = OP_0b11xx0101, /* PUSH BC */
    [0xC6] = OP_0b11000110, /* ADD n */
    [0xC7] = OP_0b11xxx111, /* RST 00h */
    [0xC8] = OP_0b110xx000, /* RET Z */
    [0xC9] = OP_0b11001001, /* RET */
    [0xCA] = OP_0b110xx010, /* JP Z,nn */
    [0xCB] = OP_0b11001011, /* CB op */
    [0xCC] = OP_0b110xx100, /* CALL Z,nn */
    [0xCD] = OP_0b11001101, /* CALL nn */
    [0xCE] = OP_0b11001110, /* ADC n */
    [0xCF] = OP_0b11xxx111, /* RST 08h */

    /* 0xD0–0xDF */
    [0xD0] = OP_0b110xx000, /* RET NC */
    [0xD1] = OP_0b11xx0001, /* POP DE */
    [0xD2] = OP_0b110xx010, /* JP NC,nn */
    [0xD3] = NULL,          /* Undefined */
    [0xD4] = OP_0b110xx100, /* CALL NC,nn */
    [0xD5] = OP_0b11xx0101, /* PUSH DE */
    [0xD6] = OP_0b11010110, /* SUB n */
    [0xD7] = OP_0b11xxx111, /* RST 10h */
    [0xD8] = OP_0b110xx000, /* RET C */
    [0xD9] = OP_0b11011001, /* RETI */
    [0xDA] = OP_0b110xx010, /* JP C,nn */
    [0xDB] = NULL,          /* Undefined */
    [0xDC] = OP_0b110xx100, /* CALL C,nn */
    [0xDD] = NULL,          /* Undefined */
    [0xDE] = OP_0b11011110, /* SBC n */
    [0xDF] = OP_0b11xxx111, /* RST 18h */

    /* 0xE0–0xEF */
    [0xE0] = OP_0b11100000, /* LDH (n),A */
    [0xE1] = OP_0b11xx0001, /* POP HL */
    [0xE2] = OP_0b11100010, /* LDH (C),A */
    [0xE3] = NULL,          /* Undefined */
    [0xE4] = NULL,          /* Undefined */
    [0xE5] = OP_0b11xx0101, /* PUSH HL */
    [0xE6] = OP_0b11100110, /* AND n */
    [0xE7] = OP_0b11xxx111, /* RST 20h */
    [0xE8] = OP_0b11101000, /* ADD SP,e */
    [0xE9] = OP_0b11101001, /* JP HL */
    [0xEA] = OP_0b11101010, /* LD (nn),A */
    [0xEB] = NULL,          /* Undefined */
    [0xEC] = NULL,          /* Undefined */
    [0xED] = NULL,          /* Undefined */
    [0xEE] = OP_0b11101110, /* XOR n */
    [0xEF] = OP_0b11xxx111, /* RST 28h */

    /* 0xF0–0xFF */
    [0xF0] = OP_0b11110000, /* LDH A,(n) */
    [0xF1] = OP_0b11xx0001, /* POP AF */
    [0xF2] = OP_0b11110010, /* LDH A,(C) */
    [0xF3] = OP_0b11110011, /* DI */
    [0xF4] = NULL,          /* Undefined */
    [0xF5] = OP_0b11xx0101, /* PUSH AF */
    [0xF6] = OP_0b11110110, /* OR n */
    [0xF7] = OP_0b11xxx111, /* RST 30h */
    [0xF8] = OP_0b11111000, /* LD HL,SP+e */
    [0xF9] = OP_0b11111001, /* LD SP,HL */
    [0xFA] = OP_0b11111010, /* LD A,(nn) */
    [0xFB] = OP_0b11111011, /* EI */
    [0xFC] = NULL,          /* Undefined */
    [0xFD] = NULL,          /* Undefined */
    [0xFE] = OP_0b11111110, /* CP n */
    [0xFF] = OP_0b11xxx111, /* RST 38h */
};
