#include "chip8.h"
#include <assert.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

uint8_t memory[MAX_ROM_SIZE];

struct registers reg;
struct stack stack;

void sync_cpu_cycle() {
  int ms_to_sleep = 1 / CPU_FREQUENCY;
  sleep(ms_to_sleep);
}
uint16_t fetch_instr() {
  // printf("Pc: %d\n", reg.pc);

  uint16_t top_bit = memory[reg.pc];
  uint16_t bottom_bit = memory[reg.pc + 1];
  uint16_t instr = (memory[reg.pc] << 8) | memory[reg.pc + 1];
  // printf("Top: %x, Bottom:  %x, Instr: %x\n", top_bit, bottom_bit, instr);
  // printf("Top Bit: %b, Bottom Bit: %b, Instr: %b\n", top_bit, bottom_bit,
  //        instr);
  reg.pc += 2; // Bug if PC is used after this. Check thoroughly.
  return instr;
}

Instr decode_instr(uint16_t raw_instr) {
  int idx;

  uint16_t first_nibble = raw_instr & 0xF000;
  uint16_t second_nibble = raw_instr & 0x0F00;
  uint16_t thrid_nibble = raw_instr & 0x00F0;
  uint16_t fourth_nibble = raw_instr & 0x000F;

  if (raw_instr == 0x00EE) {
    return _00EE;
  } else if (raw_instr == 0x00E0) {
    return _00E0;
  } else if (first_nibble == 0x1000) {
    return _1NNN;
  } else if (first_nibble == 0x2000) {
    return _2NNN;
  } else if (first_nibble == 0x3000) {
    return _3XKK;
  } else if (first_nibble == 0x4000) {
    return _4XKK;
  } else if ((first_nibble | fourth_nibble) == 0x5000) {
    return _5XY0;
  } else if (first_nibble == 0x6000) {
    return _6XKK;
  } else if (first_nibble == 0x7000) {
    return _7XKK;
  } else if ((first_nibble | fourth_nibble) == 0x8000) {
    return _8XY0;
  } else if ((first_nibble | fourth_nibble) == 0x8001) {
    return _8XY1;
  } else if ((first_nibble | fourth_nibble) == 0x8002) {
    return _8XY2;
  } else if ((first_nibble | fourth_nibble) == 0x8003) {
    return _8XY3;
  } else if ((first_nibble | fourth_nibble) == 0x8004) {
    return _8XY4;
  } else if ((first_nibble | fourth_nibble) == 0x8005) {
    return _8XY5;
  } else if ((first_nibble | fourth_nibble) == 0x8006) {
    return _8XY6;
  } else if ((first_nibble | fourth_nibble) == 0x8007) {
    return _8XY7;
  } else if ((first_nibble | fourth_nibble) == 0x800E) {
    return _8XYE;
  } else if ((first_nibble | fourth_nibble) == 0x9000) {
    return _9XY0;
  } else if (first_nibble == 0xA000) {
    return _ANNN;
  } else if (first_nibble == 0xB000) {
    return _BNNN;
  } else if (first_nibble == 0xC000) {
    return _CXKK;
  } else if (first_nibble == 0xD000) {
    return _DXYN;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xE0A1) {
    return _EXA1;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xE09E) {
    return _EX9E;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xF007) {
    return _FX07;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xF00A) {
    return _FX0A;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xF015) {
    return _FX15;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xF018) {
    return _FX18;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xF01E) {
    return _FX1E;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xF029) {
    return _FX29;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xF033) {
    return _FX33;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xF055) {
    return _FX55;
  } else if ((first_nibble | thrid_nibble | fourth_nibble) == 0xF065) {
    return _FX65;
  }

  return UNKNOWN;
}

void panic(char *msg) {
  printf("%s", msg);
  _exit(-1);
}

void execute_instr(Instr instr, uint16_t raw_instr) {
  uint16_t NNN = raw_instr & 0xFFF;
  uint8_t X = (raw_instr & 0xF00) >> 8;
  uint8_t Y = (raw_instr & 0xF0) >> 4;
  uint16_t KK = raw_instr & 0xFF;
  uint8_t N = raw_instr & 0xF;

  // printf("Instr: %d, Raw Instr: %x\n", instr, raw_instr);
  switch (instr) {
  case UNKNOWN:
    panic("Invalid Instruction. Terminating Program!!!\n");
    break;
  case _00E0:
    clear_screen();
    break;
  case _00EE:
    break;
  case _1NNN:
    reg.pc = NNN;
    break;
  case _2NNN:
    break;
  case _3XKK:
    break;
  case _4XKK:
    break;
  case _5XY0:
    break;
  case _6XKK:
    reg.gpr[X] = KK;
    break;
  case _7XKK:
    reg.gpr[X] += KK;
    break;
  case _8XY0:
    break;
  case _8XY1:
    break;
  case _8XY2:
    break;
  case _8XY3:
    break;
  case _8XY4:
    break;
  case _8XY5:
    break;
  case _8XY6:
    break;
  case _8XY7:
    break;
  case _8XYE:
    break;
  case _9XY0:
    break;
  case _ANNN:
    reg.idx = NNN;
    break;
  case _BNNN:
    break;
  case _CXKK:
    break;
  case _DXYN:
    uint8_t x = reg.gpr[X];
    uint8_t y = reg.gpr[Y];
    uint8_t *sprite = malloc(sizeof(uint8_t) * N);
    memmove(sprite, memory + reg.idx, N);
    draw_framebuffer(x, y, sprite, N);
    free(sprite);
    break;
  case _EX9E:
    break;
  case _EXA1:
    break;
  }
}

void start_program() {
  // init_screen();
  while (true) {
    uint16_t raw_instr = fetch_instr();
    Instr instr = decode_instr(raw_instr);
    execute_instr(instr, raw_instr);
    render();
    sync_cpu_cycle();
  }
}

int init_memory(char *file_name) {
  FILE *file = fopen(file_name, "r");
  assert(file != NULL);

  size_t no_of_fonts = ARRAY_SIZE(fonts);

  // Check this. Pref to use memmove
  memcpy(memory, fonts, no_of_fonts);
  // Might have bug
  int bytes = fread(memory + PROGRAM_START_ADDR, sizeof(*memory),
                    (MAX_ROM_SIZE - PROGRAM_START_ADDR),
                    file); // Check Return value
  assert(bytes != -1);

  // Init stack
  stack.idx = 0;
  fclose(file);
  return bytes;
}

int init_cpu() {
  reg.pc = PROGRAM_START_ADDR;
  reg.stack_ptr = 0;
  return 0;
};

void push_stack(uint16_t addr) { stack.stk[stack.idx++] = addr; }

uint16_t pop_stack() {
  if (stack.idx == 0) {
    panic("Stack Empty, Nothing to PoP from stack\n");
  }

  return stack.stk[--stack.idx];
}

#ifndef TESTING
int main(int argc, char *argv[]) {

  assert(argc == 2);

  init_cpu();
  init_memory(argv[1]);
  start_program();
  return 0;
}
#endif

#ifdef TESTING
uint8_t *get_memory() { return memory; }

struct registers get_registers() { return reg; }
struct stack *get_stack() { return &stack; }
#endif
