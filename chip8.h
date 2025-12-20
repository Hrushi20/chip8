#include <stdint.h>

#define MAX_ROM_SIZE 0xFFF
#define MAX_GPR 16 // General Purpose Register
#define STACK_SIZE 16
#define PROGRAM_START_ADDR 0x200

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

static uint16_t instr_identifier[] = {
    0x00EE, 0x00E0, 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000,
    0x8001, 0x8002, 0x8003, 0x8004, 0x8005, 0x8006, 0x8007, 0x800E, 0x8000,
    0x9000, 0xA000, 0xB000, 0xC000, 0xD000, 0xE09E, 0xE0A1, 0xF007, 0xF00A,
    0xF015, 0xF018, 0xF01E, 0xF029, 0xF033, 0xF055, 0xF065

};

#define NO_OF_INSTR sizeof(instr_identifier) / sizeof(instr_identifier[0])

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
void execute_instr(Instr instr);
int init_memory(int fd);
int init_cpu();
uint16_t pop_stack();
void push_stack(uint16_t addr);

// #ifdef TESTING
uint8_t *get_memory();

struct registers get_registers();
struct stack *get_stack();
// #endif
