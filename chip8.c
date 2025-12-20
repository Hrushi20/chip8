#include "chip8.h"
#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

uint8_t memory[MAX_ROM_SIZE];

regstr registers;

uint16_t fetch_instr() {
  return (memory[registers.pc] << 2) | memory[registers.pc + 1];
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

void execute_instr(Instr instr) {
  // Execute Instr
  printf("Executing Instr: %d", instr);
}

void start_program() {
  while (true) {

    // Fetch
    uint16_t raw_instr = fetch_instr();
    // Decode
    Instr instr = decode_instr(raw_instr);
    // Execute
    execute_instr(instr);
  }
}

// int main(int argc, char *argv[]) {
//
//   assert(argc == 2);
//
//   int rom = open(argv[1], O_RDONLY);
//   assert(read(rom, memory, MAX_ROM_SIZE) == MAX_ROM_SIZE);
//
//   start_program();
//   return 0;
// }
