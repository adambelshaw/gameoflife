#include <arch/zx.h>

static const unsigned char _cell_udg[] = {
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
};
static unsigned char *_font = (unsigned char *)(15360); // point font into rom character set

static void printChrAt(unsigned char x, unsigned char y, unsigned char* c)
{
    unsigned char *p;
    unsigned char i;
    p = zx_cxy2saddr(x, y);
    for (i = 0; i < 8; ++i)
    {
        *p = *c++;
        p += 256;
    }
}

void printBlockAt(unsigned char x, unsigned char y, unsigned char ink)
{
    *zx_cxy2aaddr(x, y) = ink | (ink * 8) | BRIGHT;
}

void printCellAt(unsigned char x, unsigned char y, unsigned char ink)
{
    *zx_cxy2aaddr(x, y) = ink | PAPER_WHITE;
    printChrAt(x, y, _cell_udg);
}

void clearCellAt(unsigned char x, unsigned char y)
{
    *zx_cxy2aaddr(x, y) = INK_WHITE | PAPER_WHITE;
}

unsigned char* getGlyphFromChr(unsigned char c)
{
    return _font + (c * 8);
}

void clearScreen()
{
    zx_cls(PAPER_WHITE);
}