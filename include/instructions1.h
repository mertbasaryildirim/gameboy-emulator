#ifndef INSTRUCTIONS1_H
#define INSTRUCTIONS1_H

/*
 * 0b00000000 / 0x00: NOP
 *
 * No Operation; this instruction intentionally leaves all CPU registers,
 * flags, and memory unchanged.
 *
 * The opcode fetch and PC increment are handled by the dispatcher, so this
 * handler only accounts for timing by adding 1 machine cycle to gb_proc.cycles.
 */
void OP_0b00000000(void);

/*
 * 0b00xx0001: LD rr,nn
 *
 * Loads a 16-bit immediate value nn into the register pair rr, where the
 * bits xx in the opcode select one of BC, DE, HL, or SP.
 *
 * The instruction implementation reads the low and high bytes of nn from
 * memory immediately following the opcode, combines them into a 16-bit
 * value, and stores the result in the selected register pair. It advances
 * the program counter past both immediate bytes and accounts for the total
 * execution time by adding 3 machine cycles to gb_proc.cycles.
 */
void OP_0b00xx0001(void);

#endif