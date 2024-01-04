#include <z80.h>

void print_string(uint8_t *string);
void clear_screen();
void clear_cell_at(uint8_t x, uint8_t y);
void print_cell_at(uint8_t x, uint8_t y, uint8_t ink);
void print_block_at(uint8_t x, uint8_t y, uint8_t ink);
uint8_t *get_glyph_from_chr(uint8_t c);