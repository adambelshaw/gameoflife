// imported from game.asm
extern void draw_grid_asm(unsigned int ink) __z88dk_callee; // parameter uint16_t as crashes if odd number of 8 bits
extern unsigned char iterate_grid_asm() __z88dk_callee;
extern void draw_chr_at_asm(unsigned char x, unsigned char y, unsigned int c) __z88dk_callee; // last parameter uint16_t as crashes if odd number of 8 bits

void draw_grid(unsigned char ink)
{
    draw_grid_asm(ink);
}

unsigned char iterate_grid()
{
    return iterate_grid_asm();
}

void draw_chr_at(unsigned char x, unsigned char y, unsigned char c)
{
    return draw_chr_at_asm(x, y, c);
}