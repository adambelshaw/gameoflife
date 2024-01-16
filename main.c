// imported from main.asm
extern unsigned char message_prompt[];
extern void get_message_asm() __z88dk_callee;
extern unsigned char message[];
// imported grom game.asm
extern void draw_grid_asm(unsigned int ink) __z88dk_callee; // parameter uint16_t as crashes if odd number of 8 bits
extern unsigned char iterate_grid_asm() __z88dk_callee;
extern void draw_chr_at_asm(unsigned char x, unsigned char y, unsigned int c) __z88dk_callee; // last parameter uint16_t as crashes if odd number of 8 bits
// imported from screen.asm
extern void print_string_asm(unsigned char *string) __z88dk_fastcall;
extern void clear_screen_asm() __z88dk_callee;

void main()
{
    clear_screen_asm();
    print_string_asm(message_prompt);
    get_message_asm();

    unsigned char message_index = 0;
    unsigned char updated_cell_count = 0;
    unsigned char ink = 0;
    unsigned char count = 0;
    clear_screen_asm();
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
            draw_chr_at_asm(10, 8, c);
        }
        // cycle through colours but skip black and white
        ink = (ink + 1) % 8;
        if (ink == 7)
        {
            ink = 1;
        }

        draw_grid_asm(ink);
        updated_cell_count = iterate_grid_asm();
        count++;
    }
}