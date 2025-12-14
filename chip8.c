#include "chip8.h"
#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

uint8_t memory[MAX_ROM_SIZE];

regstr registers;

uint16_t fetch_instr() {
  return (memory[registers.pc] << 2) | memory[registers.pc + 1];
}

Instr decode_instr(uint16_t raw_instr) { return UNKNOWN; }

void execute_instr(Instr instr) { printf("Nothing to execute"); }

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

int main(int argc, char *argv[]) {

  assert(argc == 2);

  int rom = open(argv[1], O_RDONLY);
  assert(read(rom, memory, MAX_ROM_SIZE) == MAX_ROM_SIZE);

  start_program();
  return 0;
}
