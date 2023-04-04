#include "game.h"
#include "screen.h"
#include <stdio.h>

void main()
{
    uint8_t message[50];

    clear_screen();
    printf("Welcome To ZX Life!\n\n");
    printf("Please enter message to display,maximum 50 characters:\n\n");
    fgets(message, 50, stdin);

    uint8_t message_index = 0;
    uint16_t updated_cell_count = 0;
    uint8_t ink = 0;
    uint8_t count = 0;
    clear_screen();
    while (1)
    {
        // if grid has low activity or it's been more than 20 iterations since a new letter
        if (updated_cell_count < 10 || count % 20 == 0)
        {
            count = 0;
            if (!message[message_index])
            {
                message_index = 0;
                ink = 1;
            }
            uint8_t c = message[message_index++];
            if (c == ' ')
            {
                c = '_';
            }
            draw_chr_at(10, 8, c);
        }
        // cycle through colours but skip black and white
        ink = (ink + 1) % 8;
        if (ink == 7)
        {
            ink = 1;
        }

        draw_grid(ink);
        updated_cell_count = iterate_grid();
        count++;
    }
}