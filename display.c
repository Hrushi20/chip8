#include "display.h"
#include "assert.h"
#include <ncurses.h>
#include <stdint.h>
#include <unistd.h>

WINDOW *win;

void init_screen() {

  initscr();
  noecho(); // User input not printed on scr
  raw();
  keypad(stdscr, true); // For arrows

  // Within This window chip8 renders
  start_color();
  win = newwin(MAX_HEIGHT, MAX_WIDTH, 0, 0);
  box(win, 0, 0);
  wrefresh(win);
}

uint8_t draw(uint8_t row, uint8_t col, uint8_t *memory, uint8_t size) {
  assert(row >= 0 && col >= 0 && size > 0);
  assert(win != NULL);
  // endwin();

  uint8_t vf_flag = 0;

  uint8_t x = col + 1;
  uint8_t y = row + 1;
  // Iterate Each Byte
  for (int i = 0; i < size; i++) {
    uint16_t itr = 256;
    wmove(win, y, x);
    // Iterate each bit in byte
    while (itr != 0) {
      uint8_t screen_sprite = winch(win) != ' ';        // 0 or 1
      uint8_t curr_sprite = (*(memory + i) & itr) != 0; // 0 or 1
      uint8_t new_sprite = screen_sprite ^ curr_sprite;

      if (new_sprite) {
        waddch(win, '*');
      } else {
        waddch(win, ' ');
      }

      if (screen_sprite && curr_sprite) {
        vf_flag = 1; // Erasing a bit
      }

      itr = itr >> 1;
    }
    y++;
  }

  wrefresh(win);
  return vf_flag;
}

void clear_screen() { wclear(win); }

// 10 means empty.
// int main() {
//   init_screen();
//
//   // Stupid logic, but setting '\n' as off and * as on.
//   uint8_t row = 1;
//   uint8_t col = 1;
//   uint8_t vf_flag = 0;
//   // Iterate Each Byte
//   for (int i = 0; i < 30; i++) {
//     uint16_t itr = 256;
//     wmove(win, row, col);
//     // Iterate each bit in byte
//     while (itr != 0) {
//       uint8_t screen_sprite = winch(win) != ' ';   // 0 or 1
//       uint8_t curr_sprite = (fonts[i] & itr) != 0; // 0 or 1
//       uint8_t new_sprite = screen_sprite ^ curr_sprite;
//
//       if (new_sprite) {
//         waddch(win, '*');
//       } else {
//         waddch(win, ' ');
//       }
//
//       if (screen_sprite && curr_sprite) {
//         vf_flag = 1; // Erasing a bit
//       }
//
//       itr = itr >> 1;
//     }
//     row++;
//     row = 1;
//   }
//
//   //
//   // waddstr(win, " Lol Brother.");
//   // char s[30];
//   // mvwinstr(win, 1, 1, s);
//   // mvwaddstr(win, 2, 1, s);
//
//   sleep(5);
//   endwin();
//   return 0;
// }
