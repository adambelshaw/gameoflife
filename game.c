#include "screen.h"
#include "grid.h"

#define MAX_ACTIVE_CELLS 255

static uint16_t updated_cell_count = 0;
static uint16_t updated_cells[MAX_ACTIVE_CELLS];
static uint16_t added_cells[MAX_ACTIVE_CELLS];
static uint16_t active_cell_count = 0;
static uint16_t active_cells[MAX_ACTIVE_CELLS];

static inline uint16_t get_cell_location(uint8_t x, uint8_t y)
{
    return ((int)y * GRID_WIDTH) + x + 1;
}

static inline uint8_t get_cell_x_coord(uint16_t cell_location)
{
    return (cell_location % GRID_WIDTH) - 1;
}

static inline uint8_t get_cell_y_coord(uint16_t cell_location)
{
    return cell_location / GRID_WIDTH;
}

static inline uint8_t is_bit_set(uint8_t ch, uint8_t i)
{
    uint8_t mask = 1 << i;
    return mask & ch;
}

static inline uint8_t set_bit(uint8_t ch, uint8_t i)
{
    uint8_t mask = 1 << i;
    return ch | mask;
}

static inline uint8_t clear_bit(uint8_t ch, uint8_t i)
{
    uint8_t mask = 1 << i;
    mask = ~mask;
    return ch & mask;
}

void draw_grid(uint8_t ink)
{
    // draw grid and update active cells for update method
    active_cell_count = 0;
    uint16_t i = 0;
    for (i = 0; i < updated_cell_count; i++)
    {

        uint16_t cell_location = updated_cells[i];
        uint8_t x = get_cell_x_coord(cell_location);
        uint8_t y = get_cell_y_coord(cell_location);
        uint8_t grid_value = get_grid_value(x, y);
        if (is_bit_set(grid_value, CELL_ALIVE))
        {
            if (added_cells[i] == cell_location)
            {
                print_block_at(x, y, ink); // added cells are bright blocks to distinguish them
            }
            else
            {
                print_cell_at(x, y, ink);
            }
            grid_value = clear_bit(grid_value, CELL_BORN);
        }
        else
        {
            clear_cell_at(x, y);
            grid_value = clear_bit(grid_value, CELL_DIES);
        }
        set_grid_value(x, y, grid_value);
        added_cells[i] = 0;

        int x2 = 0;
        int y2 = 0;
        for (x2 = x - 1; x2 <= x + 1; x2++)
        {
            if (x2 >= 0 && x2 < GRID_WIDTH)
            {
                for (y2 = y - 1; y2 <= y + 1; y2++)
                {
                    uint8_t grid_value = get_grid_value(x2, y2);
                    if (y2 >= 0 && y2 < GRID_HEIGHT && (x2 != x || y2 != y) && !is_bit_set(grid_value, CELL_ACTIVE) && active_cell_count < MAX_ACTIVE_CELLS)
                    {
                        active_cells[active_cell_count++] = get_cell_location(x2, y2);
                        grid_value = set_bit(grid_value, CELL_ACTIVE);
                        set_grid_value(x2, y2, grid_value);
                    }
                }
            }
        }
    }
}

static inline uint8_t was_cell_alive(uint8_t x, uint8_t y)
{
    uint8_t grid_value = get_grid_value(x, y);
    if ((is_bit_set(grid_value, CELL_ALIVE) && !is_bit_set(grid_value, CELL_BORN)) || is_bit_set(grid_value, CELL_DIES))
    {
        return 1;
    }
    return 0;
}

static uint8_t update_cell_state(uint8_t x, uint8_t y)
{
    uint8_t total = 0;
    int x2 = 0;
    int y2 = 0;
    for (x2 = x - 1; x2 <= x + 1; x2++)
    {
        if (x2 >= 0 && x2 < GRID_WIDTH)
        {
            for (y2 = y - 1; y2 <= y + 1; y2++)
            {
                if (y2 >= 0 && y2 < GRID_HEIGHT && (x2 != x || y2 != y) && was_cell_alive(x2, y2))
                {
                    total++;
                }
            }
        }
    }

    uint8_t was_alive = was_cell_alive(x, y);

    uint8_t grid_value = 0;
    if (was_alive == 1 && total > 1 && total < 4)
    {
        grid_value = set_bit(grid_value, CELL_ALIVE);
    }
    else if (was_alive == 0 && total == 3)
    {
        grid_value = set_bit(grid_value, CELL_ALIVE);
        grid_value = set_bit(grid_value, CELL_BORN);
    }
    else if (was_alive == 1)
    {
        grid_value = set_bit(grid_value, CELL_DIES);
    }
    set_grid_value(x, y, grid_value);
    return grid_value;
}

uint16_t iterate_grid()
{
    // update grid and populate updated cells for draw method
    updated_cell_count = 0;
    uint16_t i = 0;
    for (i = 0; i < active_cell_count; i++)
    {
        uint16_t cell_location = active_cells[i];
        uint8_t x = get_cell_x_coord(cell_location);
        uint8_t y = get_cell_y_coord(cell_location);
        uint8_t grid_value = update_cell_state(x, y);
        if ((is_bit_set(grid_value, CELL_BORN) || is_bit_set(grid_value, CELL_DIES)) && updated_cell_count < MAX_ACTIVE_CELLS)
        {
            updated_cells[updated_cell_count++] = cell_location;
        }
    }

    return updated_cell_count;
}

static inline void add_cell(uint8_t x, uint8_t y)
{
    if (updated_cell_count < MAX_ACTIVE_CELLS && x < GRID_WIDTH && y < GRID_HEIGHT)
    {
        uint8_t grid_value = set_bit(0, CELL_ALIVE);
        grid_value = set_bit(grid_value, CELL_BORN);
        set_grid_value(x, y, grid_value);
        uint16_t cell_location = get_cell_location(x, y);
        updated_cells[updated_cell_count] = cell_location;
        added_cells[updated_cell_count++] = cell_location;
    }
}

void draw_chr_at(uint8_t x, uint8_t y, uint8_t c)
{
    uint8_t *glyph = get_glyph_from_chr(c);
    uint8_t y2;
    uint8_t x2;
    for (y2 = 0; y2 < 8; y2++)
    {
        uint8_t row = *glyph++;
        for (x2 = 0; x2 < 8; x2++)
        {
            if (row & 0x01)
            {
                add_cell(x + 8 - x2, y + y2);
            }
            row = row >> 1;
        }
    }
}