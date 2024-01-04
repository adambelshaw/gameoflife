// imported from screen.asm
extern void print_string_asm(unsigned char *string) __z88dk_fastcall;
extern void clear_screen_asm() __z88dk_callee;
extern void clear_cell_at_asm(unsigned char x, unsigned char y) __z88dk_callee;
extern void print_block_at_asm(unsigned char x, unsigned char y, unsigned int ink) __z88dk_callee;
extern void print_cell_at_asm(unsigned char x, unsigned char y, unsigned int ink) __z88dk_callee;

void print_string(unsigned char *string)
{
    print_string_asm(string);
}

void clear_screen()
{
    clear_screen_asm();
}

void clear_cell_at(unsigned char x, unsigned char y)
{
    clear_cell_at_asm(x, y);
}

void print_cell_at(unsigned char x, unsigned char y, unsigned char ink)
{
    print_cell_at_asm(x, y, ink);
}

void print_block_at(unsigned char x, unsigned char y, unsigned char ink)
{
    print_block_at_asm(x, y, ink);
}