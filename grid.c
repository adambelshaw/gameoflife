// imported from grid.asm
extern unsigned char get_grid_value_asm(unsigned char x, unsigned char y) __z88dk_callee;
extern void set_grid_value_asm(unsigned char x, unsigned char y, unsigned int value) __z88dk_callee; // last parameter uint16_t as crashes if odd number of 8 bits
extern unsigned char get_cell_x_coord_asm(unsigned int cell_location) __z88dk_callee;
extern unsigned char get_cell_y_coord_asm(unsigned int cell_location) __z88dk_callee;

unsigned char get_grid_value(unsigned char x, unsigned char y)
{
    return get_grid_value_asm(x, y);
}

void set_grid_value(unsigned char x, unsigned char y, unsigned char value)
{
    set_grid_value_asm(x, y, value);
}

unsigned char get_cell_x_coord(unsigned int cell_location)
{
    return get_cell_x_coord_asm(cell_location);
}

unsigned char get_cell_y_coord(unsigned int cell_location)
{
    return get_cell_y_coord_asm(cell_location);
}