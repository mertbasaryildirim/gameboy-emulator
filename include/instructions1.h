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

#endif