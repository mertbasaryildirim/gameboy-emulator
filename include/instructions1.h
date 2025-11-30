#ifndef INSTRUCTIONS1_H
#define INSTRUCTIONS1_H

/*
 * 0b00000000 / 0x00: NOP
 *
 * No Operation; this instruction intentionally leaves all CPU registers,
 * flags, and memory unchanged. It is typically used as a timing padding
 * and has a duration of 1 machine cycle.
 */
void OP_0b00000000(void);

/*
 * 0b00xx0001: LD rr,nn
 *
 * Loads a 16-bit immediate value nn into the register pair rr, where the
 * bits xx in the opcode select one of BC, DE, HL, or SP. The low and high
 * bytes of nn are read from consecutive memory locations and combined into
 * a single 16-bit value. The instruction completes in 3 machine cycles.
 */
void OP_0b00xx0001(void);

/*
 * 0b00000010 / 0x02: LD (BC),A
 *
 * Stores the value of register A into the memory location addressed by BC.
 * The 16-bit value in BC is treated as an absolute address, and the current
 * contents of A are written to that location in memory. The instruction
 * duration is 2 machine cycles.
 */
void OP_0b00000010(void);

/*
 * 0b00xx0011: INC rr
 *
 * Increments the 16-bit register pair rr by 1, where the bits xx in the
 * opcode select one of BC, DE, HL, or SP. The operation does not modify
 * any flag bits and has a duration of 2 machine cycles.
 */
void OP_0b00xx0011(void);

/*
 * 0b00xxx100: INC r
 *
 * Increments the 8-bit register r by 1, where the bits xxx in the opcode
 * select one of B, C, D, E, H, L, or A (the (HL) memory form is handled
 * separately). Z is set if the result is zero, H is set on carry from bit 3,
 * N is cleared, and C is left unchanged. Duration: 1 machine cycle.
 */
void OP_0b00xxx100(void);

/*
 * 0b00xxx101: DEC r
 *
 * Decrements the 8-bit register r by 1, where the bits xxx in the opcode
 * select one of B, C, D, E, H, L, or A (the (HL) memory form is handled
 * separately). Z is set if the result is zero, H is set when a borrow
 * occurs from bit 4 (low nibble transitions from 0x0 to 0xF), N is set,
 * and C is left unchanged. Duration: 1 machine cycle.
 */
void OP_0b00xxx101(void);

/*
 * 0b00xxx110: LD r,n
 *
 * Loads the 8-bit immediate value n into register r, where the bits xxx in
 * the opcode select one of B, C, D, E, H, L, or A (the (HL) form is handled
 * separately). Flags are not affected. Duration: 2 machine cycles.
 */
void OP_0b00xxx110(void);

/*
 * 0b00000111 / 0x07: RLCA
 *
 * Rotates the A register left by one bit in a circular manner. Bit 7 is
 * copied into both bit 0 and the carry flag. Z is cleared, N is cleared,
 * H is cleared, and C is set to the previous value of bit 7. Duration:
 * 1 machine cycle.
 */
void OP_0b00000111(void);

/*
 * 0b00001000 / 0x08: LD (nn),SP
 *
 * Stores the current value of the stack pointer (SP) into two consecutive
 * memory locations starting at the 16-bit immediate address nn. The low
 * byte of SP is written to nn, and the high byte of SP is written to nn+1.
 * Flags are not affected. Duration: 5 machine cycles.
 */
void OP_0b00001000(void);

/*
 * 0b00xx1001: ADD HL,rr
 *
 * Adds the 16-bit register pair rr to HL, where the bits xx in the opcode
 * select one of BC, DE, HL, or SP, and stores the result back in HL.
 * Z is unaffected, N is cleared, H is set if there is a carry from bit 11,
 * and C is set if there is a carry from bit 15. Duration: 2 machine cycles.
 */
void OP_0b00xx1001(void);

/*
 * 0b00001010 / 0x0A: LD A,(BC)
 *
 * Loads the 8-bit value from memory at the address held in the BC register
 * pair into register A. Flags are not affected. Duration: 2 machine cycles.
 */
void OP_0b00001010(void);

/*
 * 0b00xx1011: DEC rr
 *
 * Decrements the 16-bit register pair rr by 1, where the bits xx in the
 * opcode select one of BC, DE, HL, or SP. This instruction does not modify
 * any flag bits. Duration: 2 machine cycles.
 */
void OP_0b00xx1011(void);

/*
 * 0b00001111 / 0x0F: RRCA
 *
 * Rotates the A register right by one bit in a circular manner. Bit 0 is
 * copied into both bit 7 and the carry flag.
 *
 * Z is cleared, N is cleared, H is cleared, and C is set to the previous
 * value of bit 0. Duration: 1 machine cycle.
 */
void OP_0b00001111(void);

/*
 * 0b00010000 / 0x10: STOP
 *
 * Enters very low-power stop mode. The main CPU stops executing instructions,
 * but internal state (registers, flags, IME, etc.) is left unchanged. The
 * second byte of the instruction is fetched and ignored so that the program
 * counter advances by 2 bytes in total.
 */
void OP_0b00010000(void);

/*
 * 0b00010010 / 0x12: LD (DE),A
 *
 * Stores the value of register A into the memory location addressed by DE.
 * The 16-bit value in DE is treated as an absolute address, and the current
 * contents of A are written to that location in memory. Flags are not
 * affected. Duration: 2 machine cycles.
 */
void OP_0b00010010(void);

/* 0b00010111 / 0x17: RLA
 *
 * Rotates the A register left by one bit through the carry flag.
 *
 * Bit 7 of A is moved into the carry flag, and the previous value of the
 * carry flag is shifted into bit 0 of A. Z is cleared, N is cleared, H is
 * cleared, and C is set to the previous value of bit 7. Duration: 1 machine
 * cycle.
 */
void OP_0b00010111(void);

/*
 * 0b00011000 / 0x18: JR e
 *
 * Performs an unconditional relative jump using the signed 8-bit offset e.
 * The offset is added to the address following this instruction to form the
 * new program counter. Flags are not affected. Duration: 3 machine cycles.
 */
void OP_0b00011000(void);

/*
 * 0b00011010 / 0x1A: LD A,(DE)
 *
 * Loads the 8-bit value from memory at the address held in the DE register
 * pair into register A. Flags are not affected. Duration: 2 machine cycles.
 */
void OP_0b00011010(void);

/*
 * 0b00011111 / 0x1F: RRA
 *
 * Rotates the A register right by one bit through the carry flag.
 *
 * Bit 0 of A is moved into the carry flag, and the previous value of the
 * carry flag is shifted into bit 7 of A. Z is cleared, N is cleared, H is
 * cleared, and C is set to the previous value of bit 0. Duration:
 * 1 machine cycle.
 */
void OP_0b00011111(void);

/*
 * 0b001xx000: JR cc,e
 *
 * Conditional relative jump using the signed 8-bit offset e. The bits xx in
 * the opcode select one of four conditions based on the Z and C flags:
 *
 *   xx = 0 → JR NZ,e  (jump if Z == 0)
 *   xx = 1 → JR Z,e   (jump if Z == 1)
 *   xx = 2 → JR NC,e  (jump if C == 0)
 *   xx = 3 → JR C,e   (jump if C == 1)
 *
 * The displacement byte e is always fetched. If the condition is true, the
 * offset is applied to the address immediately following the instruction;
 * otherwise execution continues with the next instruction. Flags are not
 * modified. Duration: 3 machine cycles when the branch is taken, 2 when it
 * is not taken.
 */
void OP_0b001xx000(void);

/*
 * 0b00100010 / 0x22: LD (HL+),A
 *
 * Stores the value of register A into the memory location addressed by HL
 * and then increments HL. Flags are not affected. Duration: 2 machine cycles.
 */
void OP_0b00100010(void);

/*
 * 0b00100111 / 0x27: DAA
 *
 * Decimal-adjusts register A after a previous BCD add or subtract.
 * Uses the N, H and C flags from the prior operation to decide how
 * much to correct A by, so that it becomes a valid packed BCD value.
 *
 * After execution A holds the adjusted result; Z is set if A is zero,
 * H is cleared, and C may be updated to reflect a carry out of the
 * high decimal digit.
 */
void OP_0b00100111(void);

/*
 * 0b00101010 / 0x2A: LD A,(HL+)
 *
 * Loads the 8-bit value from memory at the address held in HL into register A,
 * then increments HL. Flags are not affected. Duration: 2 machine cycles.
 */
void OP_0b00101010(void);

#endif
