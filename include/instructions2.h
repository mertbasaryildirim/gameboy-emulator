/*
 * CB 0b00000xxx: RLC r
 *
 * Rotates the selected 8-bit register left in a circular manner. Bits xxx
 * select B, C, D, E, H, L, or A (the (HL) form is handled separately). Bit 7
 * is copied into bit 0 and the carry flag. Z is set if the result is zero,
 * N and H are cleared, and C is set if the original bit 7 was 1. Duration:
 * 2 machine cycles.
 */
void OP_CB_0b00000xxx(void);

/*
 * CB 0b00000110: RLC (HL)
 *
 * Same operation as RLC r but applied to the byte at address HL instead of
 * a register. Z, N, H, and C are affected as in RLC r. Duration: 4 machine
 * cycles.
 */
void OP_CB_0b00000110(void);

/*
 * CB 0b00001xxx: RRC r
 *
 * Rotates the selected 8-bit register right in a circular manner. Bits xxx
 * select B, C, D, E, H, L, or A (the (HL) form is handled separately). Bit 0
 * is copied into bit 7 and the carry flag. Z is set if the result is zero,
 * N and H are cleared, and C is set if the original bit 0 was 1. Duration:
 * 2 machine cycles.
 */
void OP_CB_0b00001xxx(void);

/*
 * CB 0b00001110: RRC (HL)
 *
 * Same operation as RRC r but applied to the byte at address HL instead of
 * a register. Z, N, H, and C are affected as in RRC r. Duration: 4 machine
 * cycles.
 */
void OP_CB_0b00001110(void);

/*
 * CB 0b00010xxx: RL r
 *
 * Rotates the selected 8-bit register left through the carry flag. Bits xxx
 * select B, C, D, E, H, L, or A (the (HL) form is handled separately). Bit 7
 * is copied into the carry flag and the previous carry is copied into bit 0.
 * Z is set if the result is zero, N and H are cleared, and C is set to the
 * original bit 7. Duration: 2 machine cycles.
 */
void OP_CB_0b00010xxx(void);

/*
 * CB 0b00010110: RL (HL)
 *
 * Same operation as RL r but applied to the byte at address HL instead of a
 * register. Z, N, H, and C are affected as in RL r. Duration: 4 machine
 * cycles.
 */
void OP_CB_0b00010110(void);

/*
 * CB 0b00011xxx: RR r
 *
 * Rotates the selected 8-bit register right through the carry flag. Bits xxx
 * select B, C, D, E, H, L, or A (the (HL) form is handled separately). Bit 0
 * is copied into the carry flag and the previous carry is copied into bit 7.
 * Z is set if the result is zero, N and H are cleared, and C is set to the
 * original bit 0. Duration: 2 machine cycles.
 */
void OP_CB_0b00011xxx(void);

/*
 * CB 0b00011110: RR (HL)
 *
 * Same operation as RR r but applied to the byte at address HL instead of a
 * register. Z, N, H, and C are affected as in RR r. Duration: 4 machine
 * cycles.
 */
void OP_CB_0b00011110(void);

/*
 * CB 0b00100xxx: SLA r
 *
 * Shifts the selected 8-bit register left by one bit using an arithmetic
 * shift. Bits xxx select B, C, D, E, H, L, or A (the (HL) form is handled
 * separately). Bit 7 is shifted into the carry flag and bit 0 is set to 0.
 * Z is set if the result is zero, N and H are cleared, and C is set to the
 * original bit 7. Duration: 2 machine cycles.
 */
void OP_CB_0b00100xxx(void);

/*
 * CB 0b00100110: SLA (HL)
 *
 * Same operation as SLA r but applied to the byte at address HL instead of
 * a register. Z, N, H, and C are affected as in SLA r. Duration: 4 machine
 * cycles.
 */
void OP_CB_0b00100110(void);

/*
 * CB 0b00101xxx: SRA r
 *
 * Shifts the selected 8-bit register right by one bit using an arithmetic
 * shift. Bits xxx select B, C, D, E, H, L, or A (the (HL) form is handled
 * separately). Bit 0 is shifted into the carry flag and bit 7 retains its
 * value. Z is set if the result is zero, N and H are cleared, and C is set
 * to the original bit 0. Duration: 2 machine cycles.
 */
void OP_CB_0b00101xxx(void);