#include "game.h"
#include "screen.h"

// imported from main.asm
extern unsigned char message_prompt[];
extern void get_message_asm() __z88dk_callee;
extern unsigned char message[];

void main()
{
    clear_screen();
    print_string(message_prompt);
    get_message_asm();

    unsigned char message_index = 0;
    unsigned char updated_cell_count = 0;
    unsigned char ink = 0;
    unsigned char count = 0;
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
            unsigned char c = message[message_index++];
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