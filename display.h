#include <ncurses.h>
#include <stdint.h>

// Adding 2 extra rows for border
#define MAX_X 64
#define MAX_Y 32

#define WRAP_X(x) x % MAX_X
#define WRAP_Y(y) y % MAX_Y

void init_screen();
void render();
void clear_screen();
uint8_t draw_framebuffer(uint8_t row, uint8_t col, uint8_t *sprite,
                         uint8_t size);
