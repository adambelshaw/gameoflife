#include <arch/zx.h>

// imported from screen.asm
extern void load_graphics_asm() __z88dk_callee;
extern void print_string_asm(uint8_t *string) __z88dk_fastcall;
extern void clear_screen_asm() __z88dk_callee;
extern void clear_cell_at_asm(uint8_t x, uint8_t y) __z88dk_callee;
extern void print_block_at_asm(uint8_t x, uint8_t y, uint16_t ink) __z88dk_callee;
extern void print_cell_at_asm(uint8_t x, uint8_t y, uint16_t ink) __z88dk_callee;
extern uint8_t *get_glyph_from_chr_asm(uint8_t c) __z88dk_fastcall;

void load_graphics()
{
    load_graphics_asm();
}

void print_string(uint8_t *string)
{
    print_string_asm(string);
}

void clear_screen()
{
    clear_screen_asm();
}

void clear_cell_at(uint8_t x, uint8_t y)
{
    clear_cell_at_asm(x, y);
}

void print_cell_at(uint8_t x, uint8_t y, uint8_t ink)
{
    print_cell_at_asm(x, y, ink);
}

void print_block_at(uint8_t x, uint8_t y, uint8_t ink)
{
    print_block_at_asm(x, y, ink);
}

uint8_t *get_glyph_from_chr(uint8_t c)
{
    return get_glyph_from_chr_asm(c);
}