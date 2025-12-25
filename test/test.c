#include "../chip8.h"
#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_YELLOW "\e[0;33m"

#define TEST_FILE "/home/pc/chip8/test/test.txt"
void test_decode_instr() {
  // printf(ANSI_COLOR_YELLOW "Start test_decode_instr()" ANSI_COLOR_RESET
  // "\n");
  //
  assert(decode_instr(0x00E0) == _00E0);
  assert(decode_instr(0x00EE) == _00EE);
  assert(decode_instr(0x1123) == _1NNN);
  assert(decode_instr(0x2456) == _2NNN);
  assert(decode_instr(0x3123) == _3XKK);
  assert(decode_instr(0x4A7F) == _4XKK);
  assert(decode_instr(0x5AB0) == _5XY0);
  assert(decode_instr(0x6123) == _6XKK);
  assert(decode_instr(0x7A05) == _7XKK);
  assert(decode_instr(0x8120) == _8XY0);
  assert(decode_instr(0x8121) == _8XY1);
  assert(decode_instr(0x8122) == _8XY2);
  assert(decode_instr(0x8123) == _8XY3);
  assert(decode_instr(0x8124) == _8XY4);
  assert(decode_instr(0x8125) == _8XY5);
  assert(decode_instr(0x8126) == _8XY6);
  assert(decode_instr(0x8127) == _8XY7);
  assert(decode_instr(0x9AB0) == _9XY0);
  assert(decode_instr(0xA345) == _ANNN);
  assert(decode_instr(0xB456) == _BNNN);
  assert(decode_instr(0xC1FF) == _CXKK);
  assert(decode_instr(0xD125) == _DXYN);
  assert(decode_instr(0xE19E) == _EX9E);
  assert(decode_instr(0xE1A1) == _EXA1);
  assert(decode_instr(0xF107) == _FX07);
  assert(decode_instr(0xF115) == _FX15);
  assert(decode_instr(0xF118) == _FX18);
  assert(decode_instr(0xF11E) == _FX1E);
  assert(decode_instr(0xF129) == _FX29);
  assert(decode_instr(0xF133) == _FX33);
  assert(decode_instr(0xF155) == _FX55);
  assert(decode_instr(0xF165) == _FX65);
  printf(ANSI_COLOR_GREEN "Pass %s" ANSI_COLOR_RESET "\n", __func__);
}

void test_init_memory() {
  int bytes = init_memory(TEST_FILE);
  assert(bytes == (MAX_ROM_SIZE - PROGRAM_START_ADDR));

  uint8_t *memory = get_memory();

  // Check fonts position matches.
  for (int i = 0; i < ARRAY_SIZE(fonts); i++, memory++) {
    assert(*memory == fonts[i]);
  }

  // Check rom position matches.
  memory = get_memory();
  memory += PROGRAM_START_ADDR;

  FILE *file = fopen(TEST_FILE, "r");
  uint8_t buf[MAX_ROM_SIZE - PROGRAM_START_ADDR];
  bytes = fread(buf, sizeof(*buf), MAX_ROM_SIZE - PROGRAM_START_ADDR, file);
  assert(bytes != -1);

  for (int i = 0; i < (MAX_ROM_SIZE - PROGRAM_START_ADDR); i++, memory++) {
    assert(*memory == buf[i]);
  }
  fclose(file);
  printf(ANSI_COLOR_GREEN "Pass %s" ANSI_COLOR_RESET "\n", __func__);
}

void test_init_cpu() {
  assert(init_cpu() == 0);
  printf(ANSI_COLOR_GREEN "Pass %s" ANSI_COLOR_RESET "\n", __func__);
}

void test_stack_fn() {

  struct stack *s = get_stack();

  push_stack(0xFFF);
  push_stack(0xEEE);
  assert(s->idx == 2);

  assert(pop_stack() == 0xEEE);
  assert(pop_stack() == 0xFFF);
  assert(s->idx == 0);

  printf(ANSI_COLOR_GREEN "Pass %s" ANSI_COLOR_RESET "\n", __func__);
}

void test_fetch_instr() {
  int bytes = init_memory(TEST_FILE);
  init_cpu();

  uint16_t instr = fetch_instr(); // Test its' a fixed byte
  // TODO assert here
}

void test_execute_instr() {

  execute_instr(_00E0, 0xE0);
  // 0x00E0
  // 0x00EE
  // 0x1000
  // 0x1FFF
  // 0x2000
  // 0x2FFF
  //
  //
  // 0x3F00
  // 0x4F00
  // 0x5FF0
  // 0x9FF0
  //
  //
  // 0x6FF0
  // 0x6FFF
  // 0x7F01
  // 0x7FFF
  //
  //
  // 0x8FF0
  // 0x8FF1
  // 0x8FF2
  // 0x8FF3
  // 0x8FF4
  // 0x8FF5
  // 0x8FF7
  // 0x8F06
  // 0x8F0E
  //
  //
  // 0xA000
  // 0xAFFF
  // 0xB000
  // 0xBFFF
  // 0xFF1E
  //
  //
  // 0xC000
  // 0xCFFF
  //
  //
  // 0xD005
  // 0xDFF1
  // 0xDXY0
  //
  //
  // 0xEF9E
  // 0xEFA1
  //
  //
  // 0xFF07
  // 0xFF0A
  // 0xFF15
  // 0xFF18
  //
  //
  // 0xFF29
  // 0xFF33
  //
  //
  // 0xFF55
  // 0xFF65
  printf(ANSI_COLOR_GREEN "Pass %s" ANSI_COLOR_RESET "\n", __func__);
}

int main() {
  test_decode_instr();
  test_init_memory();
  test_init_cpu();
  test_fetch_instr();
  test_stack_fn();
  test_execute_instr();
  printf(ANSI_COLOR_GREEN "Executed all tests" ANSI_COLOR_RESET "\n");
  return 0;
}
