#include "chip8.h"
#include <assert.h>

void test_decode_instr() {
  assert(decode_instr(0x00E0) == _00E0);

  // b4d9 a1a4 8c66 8c67 6487 1847 a5a4 b4da
  assert(decode_instr(0xb4d9) == _BNNN);
  assert(decode_instr(0xa1a4) == _ANNN);
  assert(decode_instr(0x8c66) == _8XY6);
  assert(decode_instr(0x6487) == _6XKK);
  assert(decode_instr(0x1847) == _1NNN);
  assert(decode_instr(0xa5a4) == _ANNN);
}
