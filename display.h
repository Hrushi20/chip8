#include <ncurses.h>
#include <stdint.h>

// Adding 2 extra rows for border
#define MAX_WIDTH 66
#define MAX_HEIGHT 34

void init_screen();
uint8_t draw(uint8_t row, uint8_t col, uint8_t *sprite, uint8_t size);
void clear_screen();
