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