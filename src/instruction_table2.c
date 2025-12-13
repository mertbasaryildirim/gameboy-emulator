#include "instruction_table2.h"
#include "instructions2.h"

const instruction2_handler_t instruction_table2[256] = {
    /* 0x00–0x0F : RLC / RRC */
    [0x00] = OP_CB_0b00000xxx, /* RLC B */
    [0x01] = OP_CB_0b00000xxx, /* RLC C */
    [0x02] = OP_CB_0b00000xxx, /* RLC D */
    [0x03] = OP_CB_0b00000xxx, /* RLC E */
    [0x04] = OP_CB_0b00000xxx, /* RLC H */
    [0x05] = OP_CB_0b00000xxx, /* RLC L */
    [0x06] = OP_CB_0b00000110, /* RLC (HL) */
    [0x07] = OP_CB_0b00000xxx, /* RLC A */
    [0x08] = OP_CB_0b00001xxx, /* RRC B */
    [0x09] = OP_CB_0b00001xxx, /* RRC C */
    [0x0A] = OP_CB_0b00001xxx, /* RRC D */
    [0x0B] = OP_CB_0b00001xxx, /* RRC E */
    [0x0C] = OP_CB_0b00001xxx, /* RRC H */
    [0x0D] = OP_CB_0b00001xxx, /* RRC L */
    [0x0E] = OP_CB_0b00001110, /* RRC (HL) */
    [0x0F] = OP_CB_0b00001xxx, /* RRC A */

    /* 0x10–0x1F : RL / RR */
    [0x10] = OP_CB_0b00010xxx, /* RL B */
    [0x11] = OP_CB_0b00010xxx, /* RL C */
    [0x12] = OP_CB_0b00010xxx, /* RL D */
    [0x13] = OP_CB_0b00010xxx, /* RL E */
    [0x14] = OP_CB_0b00010xxx, /* RL H */
    [0x15] = OP_CB_0b00010xxx, /* RL L */
    [0x16] = OP_CB_0b00010110, /* RL (HL) */
    [0x17] = OP_CB_0b00010xxx, /* RL A */
    [0x18] = OP_CB_0b00011xxx, /* RR B */
    [0x19] = OP_CB_0b00011xxx, /* RR C */
    [0x1A] = OP_CB_0b00011xxx, /* RR D */
    [0x1B] = OP_CB_0b00011xxx, /* RR E */
    [0x1C] = OP_CB_0b00011xxx, /* RR H */
    [0x1D] = OP_CB_0b00011xxx, /* RR L */
    [0x1E] = OP_CB_0b00011110, /* RR (HL) */
    [0x1F] = OP_CB_0b00011xxx, /* RR A */

    /* 0x20–0x2F : SLA / SRA */
    [0x20] = OP_CB_0b00100xxx, /* SLA B */
    [0x21] = OP_CB_0b00100xxx, /* SLA C */
    [0x22] = OP_CB_0b00100xxx, /* SLA D */
    [0x23] = OP_CB_0b00100xxx, /* SLA E */
    [0x24] = OP_CB_0b00100xxx, /* SLA H */
    [0x25] = OP_CB_0b00100xxx, /* SLA L */
    [0x26] = OP_CB_0b00100110, /* SLA (HL) */
    [0x27] = OP_CB_0b00100xxx, /* SLA A */
    [0x28] = OP_CB_0b00101xxx, /* SRA B */
    [0x29] = OP_CB_0b00101xxx, /* SRA C */
    [0x2A] = OP_CB_0b00101xxx, /* SRA D */
    [0x2B] = OP_CB_0b00101xxx, /* SRA E */
    [0x2C] = OP_CB_0b00101xxx, /* SRA H */
    [0x2D] = OP_CB_0b00101xxx, /* SRA L */
    [0x2E] = OP_CB_0b00101110, /* SRA (HL) */
    [0x2F] = OP_CB_0b00101xxx, /* SRA A */

    /* 0x30–0x3F : SWAP / SRL */
    [0x30] = OP_CB_0b00110xxx, /* SWAP B */
    [0x31] = OP_CB_0b00110xxx, /* SWAP C */
    [0x32] = OP_CB_0b00110xxx, /* SWAP D */
    [0x33] = OP_CB_0b00110xxx, /* SWAP E */
    [0x34] = OP_CB_0b00110xxx, /* SWAP H */
    [0x35] = OP_CB_0b00110xxx, /* SWAP L */
    [0x36] = OP_CB_0b00110110, /* SWAP (HL) */
    [0x37] = OP_CB_0b00110xxx, /* SWAP A */
    [0x38] = OP_CB_0b00111xxx, /* SRL B */
    [0x39] = OP_CB_0b00111xxx, /* SRL C */
    [0x3A] = OP_CB_0b00111xxx, /* SRL D */
    [0x3B] = OP_CB_0b00111xxx, /* SRL E */
    [0x3C] = OP_CB_0b00111xxx, /* SRL H */
    [0x3D] = OP_CB_0b00111xxx, /* SRL L */
    [0x3E] = OP_CB_0b00111110, /* SRL (HL) */
    [0x3F] = OP_CB_0b00111xxx, /* SRL A */

    /* 0x40–0x4F : BIT b,r */
    [0x40] = OP_CB_0b01xxxyyy, /* BIT 0,B */
    [0x41] = OP_CB_0b01xxxyyy, /* BIT 0,C */
    [0x42] = OP_CB_0b01xxxyyy, /* BIT 0,D */
    [0x43] = OP_CB_0b01xxxyyy, /* BIT 0,E */
    [0x44] = OP_CB_0b01xxxyyy, /* BIT 0,H */
    [0x45] = OP_CB_0b01xxxyyy, /* BIT 0,L */
    [0x46] = OP_CB_0b01xxx110, /* BIT 0,(HL) */
    [0x47] = OP_CB_0b01xxxyyy, /* BIT 0,A */
    [0x48] = OP_CB_0b01xxxyyy, /* BIT 1,B */
    [0x49] = OP_CB_0b01xxxyyy, /* BIT 1,C */
    [0x4A] = OP_CB_0b01xxxyyy, /* BIT 1,D */
    [0x4B] = OP_CB_0b01xxxyyy, /* BIT 1,E */
    [0x4C] = OP_CB_0b01xxxyyy, /* BIT 1,H */
    [0x4D] = OP_CB_0b01xxxyyy, /* BIT 1,L */
    [0x4E] = OP_CB_0b01xxx110, /* BIT 1,(HL) */
    [0x4F] = OP_CB_0b01xxxyyy, /* BIT 1,A */

    /* 0x50–0x5F : BIT b,r */
    [0x50] = OP_CB_0b01xxxyyy, /* BIT 2,B */
    [0x51] = OP_CB_0b01xxxyyy, /* BIT 2,C */
    [0x52] = OP_CB_0b01xxxyyy, /* BIT 2,D */
    [0x53] = OP_CB_0b01xxxyyy, /* BIT 2,E */
    [0x54] = OP_CB_0b01xxxyyy, /* BIT 2,H */
    [0x55] = OP_CB_0b01xxxyyy, /* BIT 2,L */
    [0x56] = OP_CB_0b01xxx110, /* BIT 2,(HL) */
    [0x57] = OP_CB_0b01xxxyyy, /* BIT 2,A */
    [0x58] = OP_CB_0b01xxxyyy, /* BIT 3,B */
    [0x59] = OP_CB_0b01xxxyyy, /* BIT 3,C */
    [0x5A] = OP_CB_0b01xxxyyy, /* BIT 3,D */
    [0x5B] = OP_CB_0b01xxxyyy, /* BIT 3,E */
    [0x5C] = OP_CB_0b01xxxyyy, /* BIT 3,H */
    [0x5D] = OP_CB_0b01xxxyyy, /* BIT 3,L */
    [0x5E] = OP_CB_0b01xxx110, /* BIT 3,(HL) */
    [0x5F] = OP_CB_0b01xxxyyy, /* BIT 3,A */

    /* 0x60–0x6F : BIT b,r */
    [0x60] = OP_CB_0b01xxxyyy, /* BIT 4,B */
    [0x61] = OP_CB_0b01xxxyyy, /* BIT 4,C */
    [0x62] = OP_CB_0b01xxxyyy, /* BIT 4,D */
    [0x63] = OP_CB_0b01xxxyyy, /* BIT 4,E */
    [0x64] = OP_CB_0b01xxxyyy, /* BIT 4,H */
    [0x65] = OP_CB_0b01xxxyyy, /* BIT 4,L */
    [0x66] = OP_CB_0b01xxx110, /* BIT 4,(HL) */
    [0x67] = OP_CB_0b01xxxyyy, /* BIT 4,A */
    [0x68] = OP_CB_0b01xxxyyy, /* BIT 5,B */
    [0x69] = OP_CB_0b01xxxyyy, /* BIT 5,C */
    [0x6A] = OP_CB_0b01xxxyyy, /* BIT 5,D */
    [0x6B] = OP_CB_0b01xxxyyy, /* BIT 5,E */
    [0x6C] = OP_CB_0b01xxxyyy, /* BIT 5,H */
    [0x6D] = OP_CB_0b01xxxyyy, /* BIT 5,L */
    [0x6E] = OP_CB_0b01xxx110, /* BIT 5,(HL) */
    [0x6F] = OP_CB_0b01xxxyyy, /* BIT 5,A */

    /* 0x70–0x7F : BIT b,r */
    [0x70] = OP_CB_0b01xxxyyy, /* BIT 6,B */
    [0x71] = OP_CB_0b01xxxyyy, /* BIT 6,C */
    [0x72] = OP_CB_0b01xxxyyy, /* BIT 6,D */
    [0x73] = OP_CB_0b01xxxyyy, /* BIT 6,E */
    [0x74] = OP_CB_0b01xxxyyy, /* BIT 6,H */
    [0x75] = OP_CB_0b01xxxyyy, /* BIT 6,L */
    [0x76] = OP_CB_0b01xxx110, /* BIT 6,(HL) */
    [0x77] = OP_CB_0b01xxxyyy, /* BIT 6,A */
    [0x78] = OP_CB_0b01xxxyyy, /* BIT 7,B */
    [0x79] = OP_CB_0b01xxxyyy, /* BIT 7,C */
    [0x7A] = OP_CB_0b01xxxyyy, /* BIT 7,D */
    [0x7B] = OP_CB_0b01xxxyyy, /* BIT 7,E */
    [0x7C] = OP_CB_0b01xxxyyy, /* BIT 7,H */
    [0x7D] = OP_CB_0b01xxxyyy, /* BIT 7,L */
    [0x7E] = OP_CB_0b01xxx110, /* BIT 7,(HL) */
    [0x7F] = OP_CB_0b01xxxyyy, /* BIT 7,A */

    /* 0x80–0x8F : RES b,r */
    [0x80] = OP_CB_0b10xxxyyy, /* RES 0,B */
    [0x81] = OP_CB_0b10xxxyyy, /* RES 0,C */
    [0x82] = OP_CB_0b10xxxyyy, /* RES 0,D */
    [0x83] = OP_CB_0b10xxxyyy, /* RES 0,E */
    [0x84] = OP_CB_0b10xxxyyy, /* RES 0,H */
    [0x85] = OP_CB_0b10xxxyyy, /* RES 0,L */
    [0x86] = OP_CB_0b10xxx110, /* RES 0,(HL) */
    [0x87] = OP_CB_0b10xxxyyy, /* RES 0,A */
    [0x88] = OP_CB_0b10xxxyyy, /* RES 1,B */
    [0x89] = OP_CB_0b10xxxyyy, /* RES 1,C */
    [0x8A] = OP_CB_0b10xxxyyy, /* RES 1,D */
    [0x8B] = OP_CB_0b10xxxyyy, /* RES 1,E */
    [0x8C] = OP_CB_0b10xxxyyy, /* RES 1,H */
    [0x8D] = OP_CB_0b10xxxyyy, /* RES 1,L */
    [0x8E] = OP_CB_0b10xxx110, /* RES 1,(HL) */
    [0x8F] = OP_CB_0b10xxxyyy, /* RES 1,A */

    /* 0x90–0x9F : RES b,r */
    [0x90] = OP_CB_0b10xxxyyy, /* RES 2,B */
    [0x91] = OP_CB_0b10xxxyyy, /* RES 2,C */
    [0x92] = OP_CB_0b10xxxyyy, /* RES 2,D */
    [0x93] = OP_CB_0b10xxxyyy, /* RES 2,E */
    [0x94] = OP_CB_0b10xxxyyy, /* RES 2,H */
    [0x95] = OP_CB_0b10xxxyyy, /* RES 2,L */
    [0x96] = OP_CB_0b10xxx110, /* RES 2,(HL) */
    [0x97] = OP_CB_0b10xxxyyy, /* RES 2,A */
    [0x98] = OP_CB_0b10xxxyyy, /* RES 3,B */
    [0x99] = OP_CB_0b10xxxyyy, /* RES 3,C */
    [0x9A] = OP_CB_0b10xxxyyy, /* RES 3,D */
    [0x9B] = OP_CB_0b10xxxyyy, /* RES 3,E */
    [0x9C] = OP_CB_0b10xxxyyy, /* RES 3,H */
    [0x9D] = OP_CB_0b10xxxyyy, /* RES 3,L */
    [0x9E] = OP_CB_0b10xxx110, /* RES 3,(HL) */
    [0x9F] = OP_CB_0b10xxxyyy, /* RES 3,A */

    /* 0xA0–0xAF : RES b,r */
    [0xA0] = OP_CB_0b10xxxyyy, /* RES 4,B */
    [0xA1] = OP_CB_0b10xxxyyy, /* RES 4,C */
    [0xA2] = OP_CB_0b10xxxyyy, /* RES 4,D */
    [0xA3] = OP_CB_0b10xxxyyy, /* RES 4,E */
    [0xA4] = OP_CB_0b10xxxyyy, /* RES 4,H */
    [0xA5] = OP_CB_0b10xxxyyy, /* RES 4,L */
    [0xA6] = OP_CB_0b10xxx110, /* RES 4,(HL) */
    [0xA7] = OP_CB_0b10xxxyyy, /* RES 4,A */
    [0xA8] = OP_CB_0b10xxxyyy, /* RES 5,B */
    [0xA9] = OP_CB_0b10xxxyyy, /* RES 5,C */
    [0xAA] = OP_CB_0b10xxxyyy, /* RES 5,D */
    [0xAB] = OP_CB_0b10xxxyyy, /* RES 5,E */
    [0xAC] = OP_CB_0b10xxxyyy, /* RES 5,H */
    [0xAD] = OP_CB_0b10xxxyyy, /* RES 5,L */
    [0xAE] = OP_CB_0b10xxx110, /* RES 5,(HL) */
    [0xAF] = OP_CB_0b10xxxyyy, /* RES 5,A */

    /* 0xB0–0xBF : RES b,r */
    [0xB0] = OP_CB_0b10xxxyyy, /* RES 6,B */
    [0xB1] = OP_CB_0b10xxxyyy, /* RES 6,C */
    [0xB2] = OP_CB_0b10xxxyyy, /* RES 6,D */
    [0xB3] = OP_CB_0b10xxxyyy, /* RES 6,E */
    [0xB4] = OP_CB_0b10xxxyyy, /* RES 6,H */
    [0xB5] = OP_CB_0b10xxxyyy, /* RES 6,L */
    [0xB6] = OP_CB_0b10xxx110, /* RES 6,(HL) */
    [0xB7] = OP_CB_0b10xxxyyy, /* RES 6,A */
    [0xB8] = OP_CB_0b10xxxyyy, /* RES 7,B */
    [0xB9] = OP_CB_0b10xxxyyy, /* RES 7,C */
    [0xBA] = OP_CB_0b10xxxyyy, /* RES 7,D */
    [0xBB] = OP_CB_0b10xxxyyy, /* RES 7,E */
    [0xBC] = OP_CB_0b10xxxyyy, /* RES 7,H */
    [0xBD] = OP_CB_0b10xxxyyy, /* RES 7,L */
    [0xBE] = OP_CB_0b10xxx110, /* RES 7,(HL) */
    [0xBF] = OP_CB_0b10xxxyyy, /* RES 7,A */

    /* 0xC0–0xCF : SET b,r */
    [0xC0] = OP_CB_0b11xxxyyy, /* SET 0,B */
    [0xC1] = OP_CB_0b11xxxyyy, /* SET 0,C */
    [0xC2] = OP_CB_0b11xxxyyy, /* SET 0,D */
    [0xC3] = OP_CB_0b11xxxyyy, /* SET 0,E */
    [0xC4] = OP_CB_0b11xxxyyy, /* SET 0,H */
    [0xC5] = OP_CB_0b11xxxyyy, /* SET 0,L */
    [0xC6] = OP_CB_0b11xxx110, /* SET 0,(HL) */
    [0xC7] = OP_CB_0b11xxxyyy, /* SET 0,A */
    [0xC8] = OP_CB_0b11xxxyyy, /* SET 1,B */
    [0xC9] = OP_CB_0b11xxxyyy, /* SET 1,C */
    [0xCA] = OP_CB_0b11xxxyyy, /* SET 1,D */
    [0xCB] = OP_CB_0b11xxxyyy, /* SET 1,E */
    [0xCC] = OP_CB_0b11xxxyyy, /* SET 1,H */
    [0xCD] = OP_CB_0b11xxxyyy, /* SET 1,L */
    [0xCE] = OP_CB_0b11xxx110, /* SET 1,(HL) */
    [0xCF] = OP_CB_0b11xxxyyy, /* SET 1,A */

    /* 0xD0–0xDF : SET b,r */
    [0xD0] = OP_CB_0b11xxxyyy, /* SET 2,B */
    [0xD1] = OP_CB_0b11xxxyyy, /* SET 2,C */
    [0xD2] = OP_CB_0b11xxxyyy, /* SET 2,D */
    [0xD3] = OP_CB_0b11xxxyyy, /* SET 2,E */
    [0xD4] = OP_CB_0b11xxxyyy, /* SET 2,H */
    [0xD5] = OP_CB_0b11xxxyyy, /* SET 2,L */
    [0xD6] = OP_CB_0b11xxx110, /* SET 2,(HL) */
    [0xD7] = OP_CB_0b11xxxyyy, /* SET 2,A */
    [0xD8] = OP_CB_0b11xxxyyy, /* SET 3,B */
    [0xD9] = OP_CB_0b11xxxyyy, /* SET 3,C */
    [0xDA] = OP_CB_0b11xxxyyy, /* SET 3,D */
    [0xDB] = OP_CB_0b11xxxyyy, /* SET 3,E */
    [0xDC] = OP_CB_0b11xxxyyy, /* SET 3,H */
    [0xDD] = OP_CB_0b11xxxyyy, /* SET 3,L */
    [0xDE] = OP_CB_0b11xxx110, /* SET 3,(HL) */
    [0xDF] = OP_CB_0b11xxxyyy, /* SET 3,A */

    /* 0xE0–0xEF : SET b,r */
    [0xE0] = OP_CB_0b11xxxyyy, /* SET 4,B */
    [0xE1] = OP_CB_0b11xxxyyy, /* SET 4,C */
    [0xE2] = OP_CB_0b11xxxyyy, /* SET 4,D */
    [0xE3] = OP_CB_0b11xxxyyy, /* SET 4,E */
    [0xE4] = OP_CB_0b11xxxyyy, /* SET 4,H */
    [0xE5] = OP_CB_0b11xxxyyy, /* SET 4,L */
    [0xE6] = OP_CB_0b11xxx110, /* SET 4,(HL) */
    [0xE7] = OP_CB_0b11xxxyyy, /* SET 4,A */
    [0xE8] = OP_CB_0b11xxxyyy, /* SET 5,B */
    [0xE9] = OP_CB_0b11xxxyyy, /* SET 5,C */
    [0xEA] = OP_CB_0b11xxxyyy, /* SET 5,D */
    [0xEB] = OP_CB_0b11xxxyyy, /* SET 5,E */
    [0xEC] = OP_CB_0b11xxxyyy, /* SET 5,H */
    [0xED] = OP_CB_0b11xxxyyy, /* SET 5,L */
    [0xEE] = OP_CB_0b11xxx110, /* SET 5,(HL) */
    [0xEF] = OP_CB_0b11xxxyyy, /* SET 5,A */

    /* 0xF0–0xFF : SET b,r */
    [0xF0] = OP_CB_0b11xxxyyy, /* SET 6,B */
    [0xF1] = OP_CB_0b11xxxyyy, /* SET 6,C */
    [0xF2] = OP_CB_0b11xxxyyy, /* SET 6,D */
    [0xF3] = OP_CB_0b11xxxyyy, /* SET 6,E */
    [0xF4] = OP_CB_0b11xxxyyy, /* SET 6,H */
    [0xF5] = OP_CB_0b11xxxyyy, /* SET 6,L */
    [0xF6] = OP_CB_0b11xxx110, /* SET 6,(HL) */
    [0xF7] = OP_CB_0b11xxxyyy, /* SET 6,A */
    [0xF8] = OP_CB_0b11xxxyyy, /* SET 7,B */
    [0xF9] = OP_CB_0b11xxxyyy, /* SET 7,C */
    [0xFA] = OP_CB_0b11xxxyyy, /* SET 7,D */
    [0xFB] = OP_CB_0b11xxxyyy, /* SET 7,E */
    [0xFC] = OP_CB_0b11xxxyyy, /* SET 7,H */
    [0xFD] = OP_CB_0b11xxxyyy, /* SET 7,L */
    [0xFE] = OP_CB_0b11xxx110, /* SET 7,(HL) */
    [0xFF] = OP_CB_0b11xxxyyy, /* SET 7,A */
};
