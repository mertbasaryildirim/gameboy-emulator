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

#endif
