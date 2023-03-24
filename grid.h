#define GRID_WIDTH 31
#define GRID_HEIGHT 22

unsigned char getGridValue(unsigned char x, unsigned char y);
void setGridValue(unsigned char x, unsigned char y, unsigned char value);
unsigned int getCellLocation(unsigned char x, unsigned char y);
unsigned char getCellXCoord(unsigned int cellLocation);
unsigned char getCellYCoord(unsigned int cellLocation);