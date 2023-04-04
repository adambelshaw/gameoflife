#include <z80.h>

void print_block_at(uint8_t x, uint8_t y, uint8_t ink);
void print_cell_at(uint8_t x, uint8_t y, uint8_t ink);
void clear_cell_at(uint8_t x, uint8_t y);
void clear_screen();
uint8_t *get_glyph_from_chr(uint8_t c);