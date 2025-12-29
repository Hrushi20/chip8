#include "display.h"
#include <stdint.h>
#include <stdio.h>

#define MAX_ROM_SIZE 0xFFF
#define MAX_GPR 16 // General Purpose Register
#define STACK_SIZE 16
#define PROGRAM_START_ADDR 0x200
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define CPU_FREQUENCY 700

struct registers {
  uint16_t idx;
  uint8_t stack_ptr;
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint16_t pc;
  uint8_t gpr[MAX_GPR];
};

struct stack {
  uint16_t stk[STACK_SIZE];
  uint8_t idx;
};

typedef enum {
  // _0nnn,
  _00EE,
  _00E0,
  _1NNN,
  _2NNN,
  _3XKK,
  _4XKK,
  _5XY0,
  _6XKK,
  _7XKK,
  _8XY1,
  _8XY2,
  _8XY3,
  _8XY4,
  _8XY5,
  _8XY6,
  _8XY7,
  _8XYE,
  _8XY0,
  _9XY0,
  _ANNN,
  _BNNN,
  _CXKK,
  _DXYN,
  _EX9E,
  _EXA1,
  _FX07,
  _FX0A,
  _FX15,
  _FX18,
  _FX1E,
  _FX29,
  _FX33,
  _FX55,
  _FX65,
  UNKNOWN
} Instr;

void init_register();
uint16_t fetch_instr();
Instr decode_instr(uint16_t raw_instr);
void execute_instr(Instr instr, uint16_t raw_instr);
int init_memory(char *);
int init_cpu();
uint16_t pop_stack();
void push_stack(uint16_t addr);

const static uint8_t fonts[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
