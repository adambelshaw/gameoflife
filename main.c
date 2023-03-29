#include "game.h"
#include "screen.h"

static const unsigned char _message[] = "Welcome To ZX-Life!";

void main()
{
    unsigned char messageIndex = 0;
    unsigned int updatedCellCount = 0;
    unsigned char ink = 0;
    unsigned char count = 0;
    clearScreen();
    while (1) {
        // if grid has low activity or it's been more than 20 iterations since a new letter
        if (updatedCellCount < 10 || count % 20 == 0) {
            count = 0;
            if (!_message[messageIndex]) {
                messageIndex = 0;
                ink = 1;
            }
            unsigned char c = _message[messageIndex++];
            if (c == ' ') {
                c = '_';
            }
            drawChrAt(10, 8, c);
        }
        // cycle through colours but skip black and white
        ink = (ink + 1) % 8;
        if (ink == 7) {            
            ink = 1;
        }
        
        drawGrid(ink);        
        updatedCellCount = iterateGrid();
        count++;
    }
}