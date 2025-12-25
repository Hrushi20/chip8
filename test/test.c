#include "../chip8.h"
#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_YELLOW "\e[0;33m"

#define TEST_PASS                                                              \
  printf(ANSI_COLOR_GREEN "Pass %s" ANSI_COLOR_RESET "\n", __func__);
#define EXEC_INSTR(Instr, raw_instr) execute_instr(Instr, raw_instr)

#define TEST_FILE "/home/pc/chip8/test/test.txt"

extern uint8_t frame_buffer[MAX_Y][MAX_X];
extern uint8_t memory[MAX_ROM_SIZE];

extern struct registers reg;
extern struct stack stack;
extern bool is_draw;

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
  TEST_PASS
}

void test_init_memory() {
  int bytes = init_memory(TEST_FILE);
  assert(bytes == (MAX_ROM_SIZE - PROGRAM_START_ADDR));

  // Check fonts position matches.
  for (int i = 0; i < ARRAY_SIZE(fonts); i++) {
    assert(memory[i] == fonts[i]);
  }

  // Check rom position matches.
  // memory = get_memory();
  // memory += PROGRAM_START_ADDR;

  FILE *file = fopen(TEST_FILE, "r");
  uint8_t buf[MAX_ROM_SIZE - PROGRAM_START_ADDR];
  bytes = fread(buf, sizeof(*buf), MAX_ROM_SIZE - PROGRAM_START_ADDR, file);
  assert(bytes != -1);

  for (int i = 0; i < (MAX_ROM_SIZE - PROGRAM_START_ADDR); i++) {
    assert(memory[PROGRAM_START_ADDR + i] == buf[i]);
  }
  fclose(file);

  TEST_PASS
}

void test_init_cpu() {
  assert(init_cpu() == 0);

  TEST_PASS
}

void test_stack_fn() {

  push_stack(0xFFF);
  push_stack(0xEEE);
  assert(stack.idx == 2);

  assert(pop_stack() == 0xEEE);
  assert(pop_stack() == 0xFFF);
  assert(stack.idx == 0);

  TEST_PASS
}

void test_fetch_instr() {
  int bytes = init_memory(TEST_FILE);
  init_cpu();

  uint16_t instr = fetch_instr(); // Test its' a fixed byte
  // TODO assert here
}

void test_execute_instr_00E0() {

  EXEC_INSTR(_00E0, 0xE0);

  for (uint8_t i = 0; i < MAX_Y; i++) {
    for (uint8_t j = 0; j < MAX_X; j++) {
      assert(frame_buffer[i][j] == 0);
    }
  }

  assert(is_draw);
  TEST_PASS
}

// Test Stack
void test_execute_instr_00EE() {
  printf("TODO. Implement: %s\n", __func__);
  exit(1);

  TEST_PASS
}

void test_execute_instr_1NNN() {
  EXEC_INSTR(_1NNN, 0x1000);
  assert(reg.pc == 0x000);

  EXEC_INSTR(_1NNN, 0x1FFF);
  assert(reg.pc == 0xFFF);

  TEST_PASS
}

void test_execute_instr_2NNN() {
  reg.pc = 0x30;
  EXEC_INSTR(_2NNN, 0x2000);
  uint16_t pc = pop_stack();
  assert(pc == 0x30);
  assert(reg.pc == 0x0);

  reg.pc = 0x50;
  EXEC_INSTR(_2NNN, 0x2FFF);
  pc = pop_stack();
  assert(pc == 0x50);
  assert(reg.pc == 0xFFF);

  TEST_PASS
}

void test_execute_instr_3XKK() {

  reg.pc = 0x50;
  reg.gpr[0xF] = 0x0;
  EXEC_INSTR(_3XKK, 0x3F00);
  assert(reg.pc == 0x52);

  reg.pc = 0x60;
  EXEC_INSTR(_3XKK, 0x3F01);
  assert(reg.pc == 0x60);

  TEST_PASS
}

void test_execute_instr_4XKK() {
  reg.pc = 0x50;
  reg.gpr[0xF] = 0x20;
  EXEC_INSTR(_4XKK, 0x4F00);
  assert(reg.pc == 0x52);

  reg.pc = 0x70;
  EXEC_INSTR(_4XKK, 0x4F20);
  assert(reg.pc == 0x70);

  TEST_PASS
}

void test_execute_instr_5XY0() {
  reg.pc = 0x70;
  reg.gpr[0xF] = 0x20;
  reg.gpr[0xE] = 0x20;
  EXEC_INSTR(_5XY0, 0x5FE0);
  assert(reg.pc == 0x72);

  reg.pc = 0x70;
  reg.gpr[0xE] = 0x19;
  EXEC_INSTR(_5XY0, 0x5FE0);
  assert(reg.pc == 0x70);

  TEST_PASS
}

void test_execute_instr_6XKK() {
  EXEC_INSTR(_6XKK, 0x6FAE);
  assert(reg.gpr[0xF] == 0xAE);

  TEST_PASS
}

void test_execute_instr_7XKK() {
  reg.gpr[0xF] = 0x3;
  EXEC_INSTR(_7XKK, 0x7FFE);
  assert(reg.gpr[0xF] == 0x1);

  TEST_PASS
}

void test_execute_instr_9XY0() {
  reg.pc = 0x70;
  reg.gpr[0xF] = 0x20;
  reg.gpr[0xE] = 0x20;
  EXEC_INSTR(_9XY0, 0x9FE0);
  assert(reg.pc == 0x70);

  reg.pc = 0x70;
  reg.gpr[0xE] = 0x19;
  EXEC_INSTR(_9XY0, 0x9FE0);
  assert(reg.pc == 0x72);

  TEST_PASS
}

void test_execute_instr_ANNN() {
  EXEC_INSTR(_ANNN, 0xAFEF);
  assert(reg.idx == 0xFEF);
  TEST_PASS
}

void test_execute_instr_BNNN() {
  reg.gpr[0x0] = 4;
  EXEC_INSTR(_BNNN, 0xBFEF);
  assert(reg.pc == 0xFF3);
  TEST_PASS
}

void test_execute_instr_CXKK() {
  EXEC_INSTR(_CXKK, 0xCF3F);
  TEST_PASS
}

void test_execute_instr_DXYN() {
  //
  printf("TODO. Implement: %s\n", __func__);
  exit(1);
}

void test_execute_instr_8XY0() {
  reg.gpr[0xF] = 30;
  reg.gpr[0x3] = 40;
  EXEC_INSTR(_8XY0, 0x8F30);
  assert(reg.gpr[0xF] == reg.gpr[0x3]);
  TEST_PASS
}

void test_execute_instr_8XY1() {
  uint8_t data = 30;
  reg.gpr[0xF] = data;
  reg.gpr[0x3] = 40;
  EXEC_INSTR(_8XY1, 0x8F31);
  assert((data | reg.gpr[0x3]) == reg.gpr[0xF]);
  TEST_PASS
}

void test_execute_instr_8XY2() {
  uint8_t data = 30;
  reg.gpr[0xF] = data;
  reg.gpr[0x3] = 40;
  EXEC_INSTR(_8XY2, 0x8F32);
  assert((data & reg.gpr[0x3]) == reg.gpr[0xF]);
  TEST_PASS
}

void test_execute_instr_8XY3() {
  uint8_t data = 30;
  reg.gpr[0xF] = data;
  reg.gpr[0x3] = 40;
  EXEC_INSTR(_8XY3, 0x8F33);
  assert((data ^ reg.gpr[0x3]) == reg.gpr[0xF]);
  TEST_PASS
}

void test_execute_instr_8XY4() {
  // Test Carry bit.
  reg.gpr[0xA] = 0xFF;
  reg.gpr[0xB] = 0x2;
  EXEC_INSTR(_8XY4, 0x8AB4);
  assert(reg.gpr[0xA] == 0x1);
  assert(reg.gpr[0xF] == 0x1);

  // Without Carry bit.
  reg.gpr[0xA] = 0xF0;
  reg.gpr[0xB] = 0x2;
  EXEC_INSTR(_8XY4, 0x8AB4);
  assert(reg.gpr[0xA] == 0xF2);
  assert(reg.gpr[0xF] == 0x0);
  TEST_PASS
}

void test_execute_instr_8XY5() {
  uint8_t data = 0xE;
  reg.gpr[0xA] = data;
  reg.gpr[0xB] = 0xF;
  EXEC_INSTR(_8XY5, 0x8AB5);
  uint8_t res = data - reg.gpr[0xB];
  assert(reg.gpr[0xF] == 0);
  assert(reg.gpr[0xA] == res);

  data = 0xF;
  reg.gpr[0xA] = data;
  reg.gpr[0xB] = 0xE;
  EXEC_INSTR(_8XY5, 0x8AB5);
  assert(reg.gpr[0xF] == 1);
  assert(reg.gpr[0xA] == data - reg.gpr[0xB]);
  TEST_PASS
}

void test_execute_instr_8XY6() {
  // Least sig bit set.
  uint8_t data = 0x5;
  reg.gpr[0x1] = data;
  EXEC_INSTR(_8XY6, 0x8136);
  assert((data >> 1) == reg.gpr[0x1]);
  assert(reg.gpr[0xF] == 1);

  // Least sig bit unset.
  data = 0x4;
  reg.gpr[0x1] = data;
  EXEC_INSTR(_8XY6, 0x8136);
  assert((data >> 1) == reg.gpr[0x1]);
  assert(reg.gpr[0xF] == 0);
  TEST_PASS
}

void test_execute_instr_8XY7() {

  uint8_t data = 0xE;
  reg.gpr[0xA] = data;
  reg.gpr[0xB] = 0xF;
  EXEC_INSTR(_8XY7, 0x8AB7);
  assert(reg.gpr[0xF] == 1);
  assert(reg.gpr[0xA] == reg.gpr[0xB] - data);

  data = 0xF;
  reg.gpr[0xA] = data;
  reg.gpr[0xB] = 0xE;
  EXEC_INSTR(_8XY7, 0x8AB7);
  assert(reg.gpr[0xF] == 0);
  uint8_t res = reg.gpr[0xB] - data;
  assert(reg.gpr[0xA] == res);
  TEST_PASS
}
void test_execute_instr_8XYE() {
  uint8_t data = 0xFF;
  reg.gpr[0x1] = data;
  EXEC_INSTR(_8XYE, 0x813E);
  assert((uint8_t)(data << 1) == reg.gpr[0x1]);
  assert(reg.gpr[0xF] == 1);
  TEST_PASS
}

int main() {
  test_decode_instr();
  test_init_memory();
  test_init_cpu();
  test_fetch_instr();
  test_stack_fn();

  // Execute Instr
  test_execute_instr_00E0();
  test_execute_instr_1NNN();
  test_execute_instr_2NNN();
  test_execute_instr_3XKK();
  test_execute_instr_4XKK();
  test_execute_instr_5XY0();
  test_execute_instr_6XKK();
  test_execute_instr_7XKK();
  test_execute_instr_9XY0();
  test_execute_instr_ANNN();
  test_execute_instr_BNNN();
  test_execute_instr_CXKK();
  // test_execute_instr_DXYN()
  test_execute_instr_8XY0();
  test_execute_instr_8XY1();
  test_execute_instr_8XY2();
  test_execute_instr_8XY3();
  test_execute_instr_8XY4();
  test_execute_instr_8XY5();
  test_execute_instr_8XY6();
  test_execute_instr_8XY7();
  test_execute_instr_8XYE();
  printf(ANSI_COLOR_GREEN "Executed all tests" ANSI_COLOR_RESET "\n");
  return 0;
}
