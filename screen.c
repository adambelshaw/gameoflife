#include <arch/zx.h>

// graphics
const unsigned char _block_udg[] = {
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111
};
const unsigned char _blank_udg[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char *_font = (unsigned char *)(15360); // point font into rom character set

void printChr(unsigned char x, unsigned char y, unsigned char* c)
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

void printBlock(unsigned char x, unsigned char y)
{
    printChr(x, y, _block_udg);
}

void clearBlock(unsigned char x, unsigned char y)
{
    printChr(x, y, _blank_udg);
}

void printStr(unsigned char x, unsigned char y, unsigned char *s)
{
   unsigned char c;
   while (c = *s++)
   {
      printChr(x, y, _font + c*8);
      if (++x == 32)
      {
         x = 0;
         y++;
      }
   }
}

void clearScreen()
{
    zx_cls(PAPER_WHITE);
}