#define GRID_WIDTH 31
#define GRID_HEIGHT 22

#define CELL_ALIVE 0 // cell is currently alive
#define CELL_BORN 1 // cell has just been born
#define CELL_DIES 2 // cell has just died
#define CELL_ACTIVE 3 // cell neighbours an updated cell

unsigned char getGridValue(unsigned char x, unsigned char y);
void setGridValue(unsigned char x, unsigned char y, unsigned char value);