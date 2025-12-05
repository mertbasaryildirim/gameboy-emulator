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

/*
 * 0b00101111 / 0x2F: CPL
 *
 * Complements (bitwise inverts) the value in register A. The N and H flags
 * are set; Z and C are left unchanged. Duration: 1 machine cycle.
 */
void OP_0b00101111(void);

/*
 * 0b00110010 / 0x32: LD (HL-),A
 *
 * Stores the value of register A into the memory location addressed by HL
 * and then decrements HL. Flags are not affected. Duration: 2 machine cycles.
 */
void OP_0b00110010(void);

/*
 * 0b00110110 / 0x36: LD (HL),n
 *
 * Loads the 8-bit immediate value n into the memory location addressed by HL.
 * The HL register pair itself is not modified and no flags are affected.
 * Duration: 3 machine cycles.
 */
void OP_0b00110110(void);

/*
 * 0b00110111 / 0x37: SCF
 *
 * Sets the carry flag and clears the N and H flags. The Z flag is left
 * unchanged. Duration: 1 machine cycle.
 */
void OP_0b00110111(void);

/*
 * 0b00111010 / 0x3A: LD A,(HL-)
 *
 * Loads the 8-bit value from memory at the address held in HL into register A,
 * then decrements HL. Flags are not affected. Duration: 2 machine cycles.
 */
void OP_0b00111010(void);

/*
 * 0b00111111 / 0x3F: CCF
 *
 * Complements (toggles) the carry flag and clears the N and H flags.
 * The Z flag is left unchanged. Duration: 1 machine cycle.
 */
void OP_0b00111111(void);

/*
 * 0b01xxxxyy: LD r,r'
 *
 * Copies the value from 8-bit register r' into register r. Neither source
 * nor destination is (HL); those cases are handled by separate LD r,(HL)
 * and LD (HL),r instructions. Flags are not affected. Duration: 1 machine
 * cycle.
 */
void OP_0b01xxxxyy(void);

/*
 * 0b01xxx110: LD r,(HL)
 *
 * Loads the 8-bit value from memory at address HL into register r. The
 * register index r is taken from bits xxx of the opcode and is never (HL).
 * Flags are not affected. Duration: 2 machine cycles.
 */
void OP_0b01xxx110(void);

/*
 * 0b01110xxx: LD (HL),r
 *
 * Stores the value of register r into memory at address HL. The register
 * index r is taken from bits xxx of the opcode and is never (HL). HL itself
 * is not modified and flags are unchanged. Duration: 2 machine cycles.
 */
void OP_0b01110xxx(void);

/*
 * 0b01110110 / 0x76: HALT
 *
 * Puts the CPU into the halted state until an interrupt occurs or
 * the halt condition is otherwise cleared. Registers and flags are
 * not modified. Duration: 1 machine cycle to enter the halt state.
 */
void OP_0b01110110(void);

/*
 * 0b10000xxx: ADD A,r
 *
 * Adds the 8-bit register r to A and writes the sum back to A.
 *
 * Bits xxx select one of B, C, D, E, H, L or A (never (HL)). The Z flag is
 * set if the result is zero, N is cleared, H is set on carry out of bit 3,
 * and C is set on carry out of bit 7. Duration: 1 machine cycle.
 */
void OP_0b10000xxx(void);

/*
 * 0b10000110 / 0x86: ADD A,(HL)
 *
 * Adds the byte at address HL to A and writes the sum back to A.
 *
 * The operand is fetched from memory pointed to by HL. The Z flag is set if
 * the result is zero, N is cleared, H is set on carry out of bit 3, and C is
 * set on carry out of bit 7. Duration: 2 machine cycles.
 */
void OP_0b10000110(void);

/*
 * 0b10001xxx: ADC A,r
 *
 * Adds register r and the carry flag to A and writes the result back to A.
 *
 * Bits xxx select one of B, C, D, E, H, L or A (as source). Z is set if the
 * result is zero, N is cleared, H is set on carry out of bit 3, and C is set
 * on carry out of bit 7. Duration: 1 machine cycle.
 */
void OP_0b10001xxx(void);

/*
 * 0b10001110 / 0x8E: ADC A,(HL)
 *
 * Adds the byte at address HL and the carry flag to A and writes the result
 * back to A.
 *
 * The operand is read from memory pointed to by HL. Z is set if the result is
 * zero, N is cleared, H is set on carry out of bit 3, and C is set on carry
 * out of bit 7. Duration: 2 machine cycles.
 */
void OP_0b10001110(void);

/*
 * 0b10010xxx: SUB A,r
 *
 * A ← A - r.
 *
 * Bits xxx select B, C, D, E, H, L or A as the source register (excluding
 * the (HL) case). The result is written back to A. Z is set if the result
 * is zero, N is set, H is set if there is a borrow from bit 4, and C is set
 * if there is a borrow from bit 7. Duration: 1 machine cycle.
 */
void OP_0b10010xxx(void);

/*
 * 0b10010110 / 0x96: SUB A,(HL)
 *
 * A ← A - (HL).
 *
 * The operand is read from memory at the address in HL. The result is written
 * back to A. Z is set if the result is zero, N is set, H is set if there is a
 * borrow from bit 4, and C is set if there is a borrow from bit 7. Duration:
 * 2 machine cycles.
 */
void OP_0b10010110(void);

/*
 * 0b10011xxx: SBC A,r
 *
 * A ← A - r - C.
 *
 * Bits xxx select B, C, D, E, H, L or A as the source register
 * (excluding the (HL) case). The result is written back to A.
 * Z is set if the result is zero, N is set, H is set if there is a
 * borrow from bit 4, and C is set if there is a borrow from bit 7.
 * Duration: 1 machine cycle.
 */
void OP_0b10011xxx(void);

/*
 * 0b10011110 / 0x9E: SBC A,(HL)
 *
 * A ← A - (HL) - C.
 *
 * The operand is read from memory at the address in HL and the result
 * is written back to A. Z is set if the result is zero, N is set, H is
 * set if there is a borrow from bit 4, and C is set if there is a borrow
 * from bit 7. Duration: 2 machine cycles.
 */
void OP_0b10011110(void);

/*
 * 0b10100xxx: AND A,r
 *
 * Performs a bitwise AND between A and register r, then stores the
 * result back into A. Bits xxx select B, C, D, E, H, L or A as the
 * source register (excluding the (HL) case).
 *
 * Z is set if the result is zero; N is cleared; H is set; C is
 * cleared. Duration: 1 machine cycle.
 */
void OP_0b10100xxx(void);

/*
 * 0b10100110 / 0xA6: AND A,(HL)
 *
 * Performs a bitwise AND between A and the byte at address HL, then
 * stores the result back into A.
 *
 * Z is set if the result is zero; N is cleared; H is set; C is
 * cleared. Duration: 2 machine cycles.
 */
void OP_0b10100110(void);

/*
 * 0b10101xxx: XOR r
 *
 * Bitwise XOR between A and register r, storing the result in A.
 * Bits xxx select B, C, D, E, H, L or A as the source register
 * (the (HL) case is handled separately).
 *
 * Flags:
 *   Z = 1 if result == 0, else 0
 *   N = 0
 *   H = 0
 *   C = 0
 */
void OP_0b10101xxx(void);

/*
 * 0b10101110 / 0xAE: XOR (HL)
 *
 * Bitwise XOR between A and the value at address HL, storing the
 * result in A.
 *
 * Flags:
 *   Z = 1 if result == 0, else 0
 *   N = 0
 *   H = 0
 *   C = 0
 */
void OP_0b10101110(void);

/*
 * 0b10110xxx: OR r
 *
 * Bitwise OR between A and register r, storing the result in A.
 * Bits xxx select B, C, D, E, H, L or A as the source register
 * (the (HL) case is handled separately).
 *
 * Flags:
 *   Z = 1 if result == 0, else 0
 *   N = 0
 *   H = 0
 *   C = 0
 */
void OP_0b10110xxx(void);

/*
 * 0b10110110 / 0xB6: OR (HL)
 *
 * Bitwise OR between A and the value at address HL, storing the
 * result in A.
 *
 * Flags:
 *   Z = 1 if result == 0, else 0
 *   N = 0
 *   H = 0
 *   C = 0
 */
void OP_0b10110110(void);

/*
 * 0b10111xxx: CP r
 *
 * Compares register r with A by computing A - r and updating flags
 * based on the result, without modifying A. Bits xxx select B, C, D,
 * E, H, L or A as the source register (excluding the (HL) case).
 *
 * Z is set if the subtraction result is zero. N is set. H is set if
 * there is a borrow from bit 4. C is set if there is a borrow from
 * bit 7. Duration: 1 machine cycle.
 */
void OP_0b10111xxx(void);

/*
 * 0b10111110 / 0xBE: CP (HL)
 *
 * Compares the value at address HL with A by computing A - (HL) and
 * updating flags based on the result, without modifying A.
 *
 * Z is set if the subtraction result is zero. N is set. H is set if
 * there is a borrow from bit 4. C is set if there is a borrow from
 * bit 7. Duration: 2 machine cycles.
 */
void OP_0b10111110(void);

/*
 * 0b110xx000: RET cc
 *
 * Conditional return from subroutine. The bits xx in the opcode select
 * one of the four SM83 conditions:
 *
 *   00: NZ  (return if Z flag is 0)
 *   01: Z   (return if Z flag is 1)
 *   10: NC  (return if C flag is 0)
 *   11: C   (return if C flag is 1)
 *
 * When the condition is true, a 16-bit return address is popped from
 * the stack (low byte first, then high byte) and loaded into PC.
 * If the condition is false, execution simply continues at the next
 * instruction with no stack access.
 *
 * Timing:
 *   cc == true  -> 5 machine cycles (stack pop + jump)
 *   cc == false -> 2 machine cycles (condition test only)
 */
void OP_0b110xx000(void);

/*
 * 0b11xx0001: POP rr
 *
 * Pops a 16-bit value from the stack into register pair rr, where the bits
 * xx in the opcode select one of BC, DE, HL, or AF.
 *
 * Two bytes are read from memory at SP (low, then high), SP is advanced by
 * 2, and the combined 16-bit value is written into the selected register
 * pair. When popping into AF, the lower four bits of F are cleared to keep
 * the flag register in the Game Boy–compatible format. The operation
 * consumes 3 machine cycles.
 */
void OP_0b11xx0001(void);

#endif
