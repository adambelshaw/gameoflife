#include <stdio.h>
#include <arch/zx.h>

#define GRID_WIDTH 31
#define GRID_HEIGHT 22

// possible cell states stored in _grid
const unsigned char CELL_DEAD = 0; // cell was dead, still is dead
const unsigned char CELL_LIVES = 1; // cell was alive, still is alive
const unsigned char CELL_BORN = 2; // cell was dead, is now alive
const unsigned char CELL_DIES = 3; // cell was alive, is now dead
// adding this to a cell state marks it as active
const unsigned char CELL_ACTIVE = 5;

unsigned char _grid[GRID_WIDTH][GRID_HEIGHT];

void drawGrid()
{
    unsigned char x = 0;
    unsigned char y = 0;
    int x2 = 0;
    int y2 = 0;
    for (x = 0; x < GRID_WIDTH; x++) {
        for (y = 0; y < GRID_HEIGHT; y++) {
            if (_grid[x][y] % CELL_ACTIVE == CELL_BORN || _grid[x][y] % CELL_ACTIVE == CELL_DIES) {
                if (_grid[x][y] % CELL_ACTIVE == CELL_BORN) {
                    printf("\x16\%c\%c0", x + 1, y + 1);
                    _grid[x][y] = CELL_LIVES;
                } else if (_grid[x][y] % CELL_ACTIVE == CELL_DIES) {
                    printf("\x16\%c\%c ", x + 1, y + 1);
                    _grid[x][y] = CELL_DEAD;
                }
                for (x2 = x - 1; x2 <= x + 1; x2++) {
                    if (x2 >= 0 && x2 < GRID_WIDTH) {
                        for (y2 = y - 1; y2 <= y + 1; y2++) {
                            if (y2 >= 0 && y2 < GRID_HEIGHT && (x2 != x || y2 != y) && _grid[x2][y2] < CELL_ACTIVE) {
                                _grid[x2][y2] += CELL_ACTIVE;
                            }
                        }
                    }
                }
            }
        }
    }
}

unsigned char wasCellAlive(unsigned char x, unsigned char y)
{
    if (_grid[x][y] % CELL_ACTIVE == CELL_LIVES || _grid[x][y] % CELL_ACTIVE == CELL_DIES) {
        return 1;
    }
    return 0;
}

unsigned char getCellState(unsigned char x, unsigned char y)
{
    unsigned char total = 0;
    int x2 = 0;
    int y2 = 0;
    for (x2 = x - 1; x2 <= x + 1; x2++) {
        if (x2 >= 0 && x2 < GRID_WIDTH) {
            for (y2 = y - 1; y2 <= y + 1; y2++) {
                if (y2 >= 0 && y2 < GRID_HEIGHT && (x2 != x || y2 != y) && wasCellAlive(x2, y2)) {
                    total++;
                }
            }
        }
    }

    unsigned char wasAlive = wasCellAlive(x, y);
    if (wasAlive == 1 && total > 1 && total < 4) {
        return CELL_LIVES;
    } else if (wasAlive == 0 && total == 3) {
        return CELL_BORN;
    } else if (wasAlive == 0) {
        return CELL_DEAD;
    }
    return CELL_DIES;
}

void iterateGrid()
{
    unsigned char x = 0;
    unsigned char y = 0;
    for (x = 0; x < GRID_WIDTH; x++) {
        for (y = 0; y < GRID_HEIGHT; y++) {
            if (_grid[x][y] >= CELL_ACTIVE) {
                _grid[x][y] = getCellState(x, y);
            }
        }
    }
}

void createGliderAt(unsigned char x, unsigned char y)
{
    _grid[x + 1][y] = CELL_BORN;
    _grid[x + 2][y + 1] = CELL_BORN;
    _grid[x][y + 2] = CELL_BORN;
    _grid[x + 1][y + 2] = CELL_BORN;
    _grid[x + 2][y + 2] = CELL_BORN;
}

void main()
{   
    zx_cls(PAPER_WHITE);
    createGliderAt(0, 0);
    createGliderAt(5, 5);
    createGliderAt(10, 10);
    createGliderAt(15, 15);
    while (1) {
        drawGrid();
        iterateGrid();
    }
}