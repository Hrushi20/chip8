#include "display.h"
#include "assert.h"
#include <ncurses.h>
#include <ncursesw/ncurses.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

WINDOW *win;

uint8_t frame_buffer[MAX_Y][MAX_X];
extern bool is_draw;

void init_screen() {

  initscr();
  noecho(); // User input not printed on scr
  raw();
  keypad(stdscr, true); // For arrows

  // Within This window chip8 renders
  start_color();
  win = newwin(MAX_Y, MAX_X, 0, 0);
  wrefresh(win);
}

void render() {
  if (!is_draw)
    return;
  for (int i = 0; i < MAX_Y; i++) {
    for (int j = 0; j < MAX_X; j++) {
      mvwaddstr(win, i, j, frame_buffer[i][j] ? "#" : " ");
      // printf("%s", frame_buffer[i][j] ? "##" : "  ");
    }

    // printf("\n");
  }
  // iterate frame buffer and add to screen.
  wrefresh(win);
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
