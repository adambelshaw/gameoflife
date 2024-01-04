void print_string(unsigned char *string);
void clear_screen();
void clear_cell_at(unsigned char x, unsigned char y);
void print_cell_at(unsigned char x, unsigned char y, unsigned char ink);
void print_block_at(unsigned char x, unsigned char y, unsigned char ink);
unsigned char *get_glyph_from_chr(unsigned char c);