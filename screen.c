#include <arch/zx.h>

extern uint8_t cell_sprite[];
static uint8_t *font = (uint8_t *)(15360); // point font into rom character set

static void print_chr_at(uint8_t x, uint8_t y, uint8_t *c)
{
    uint8_t *p;
    uint8_t i;
    p = zx_cxy2saddr(x, y);
    for (i = 0; i < 8; ++i)
    {
        *p = *c++;
        p += 256;
    }
}

void print_block_at(uint8_t x, uint8_t y, uint8_t ink)
{
    *zx_cxy2aaddr(x, y) = ink | (ink * 8) | BRIGHT;
}

void print_cell_at(uint8_t x, uint8_t y, uint8_t ink)
{
    *zx_cxy2aaddr(x, y) = ink | PAPER_WHITE;
    print_chr_at(x, y, cell_sprite);
}

void clear_cell_at(uint8_t x, uint8_t y)
{
    *zx_cxy2aaddr(x, y) = INK_WHITE | PAPER_WHITE;
}

uint8_t *get_glyph_from_chr(uint8_t c)
{
    return font + (c * 8);
}

void clear_screen()
{
    zx_border(INK_WHITE);
    zx_cls(PAPER_WHITE);
}