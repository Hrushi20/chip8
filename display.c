#include "display.h"
#include "assert.h"
#include <ncurses.h>
#include <ncursesw/ncurses.h>
#include <stdint.h>
#include <unistd.h>

// WINDOW *win;

uint8_t frame_buffer[MAX_Y][MAX_X];

void init_screen() {

  initscr();
  noecho(); // User input not printed on scr
  raw();
  keypad(stdscr, true); // For arrows

  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);

  attron(COLOR_PAIR(1));
  refresh();
}

bool render(bool is_draw) {
  is_draw &= true;
  if (!is_draw) {
    return is_draw;
  }
  // iterate frame buffer and add to screen.
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      mvaddstr(i, j, frame_buffer[i][j] ? "#" : " ");
    }
  }
  refresh();
  return is_draw;
}

// Erase Frame Buffer
void clear_screen() {
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      frame_buffer[i][j] = 0;
    }
  }
}

uint8_t draw_framebuffer(uint8_t _x, uint8_t _y, uint8_t *sprite,
                         uint8_t size) {

  uint8_t vf_flag = 0;

  uint8_t x = WRAP_X(_x);
  // uint8_t y = WRAP_Y(_y);
  uint8_t y = _y;
  // Iterate through bytes
  for (int i = 0; i < size; i++) {

    uint8_t itr = 0x80;
    while (itr > 0) {
      uint8_t curr_sprite = (sprite[i] & itr) != 0;
      uint8_t screen_sprite = frame_buffer[y][x];
      uint8_t new_sprite = curr_sprite ^ screen_sprite;

      frame_buffer[y][x] = new_sprite;

      if (curr_sprite && screen_sprite) {
        vf_flag = 1;
      }
      itr = itr >> 1;
      x = WRAP_X(x + 1);
    }

    x = WRAP_X(_x);
    y = y + 1;
  }

  return vf_flag;
}

uint8_t get_keypress() { return getch(); }

// int main() {
//   init_screen();
//
//   // Stupid logic, but setting '\n' as off and * as on.
//   uint8_t row = 1;
//   uint8_t col = 1;
//   uint8_t vf_flag = 0;
//
//   char c;
//   while ((c = wgetch(win))) {
//     printf("Char: %c\n", c);
//   }
//
//   printf("Char: %c\n", c);
//   sleep(5);
//   endwin();
//   return 0;
// }
