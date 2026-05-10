# CHIP-8 Emulator (C + ncurses)

Simple CHIP-8 emulator written in C, rendered in terminal using `ncurses`.

## Prerequisites

- C compiler (`cc` / `clang` / `gcc`)
- `make`
- `ncurses` development headers and library

On macOS (Homebrew), if needed:

```bash
brew install ncurses
```

## Build

From project root:

```bash
make
```

This creates the executable:

```bash
./chip8
```

## Run

Pass a CHIP-8 ROM file path as argument:

```bash
./chip8 assets/1-chip8-logo.ch8
```

Other sample ROMs:

```bash
./chip8 assets/2-ibm-logo.ch8
./chip8 assets/3-corax+.ch8
./chip8 assets/4-flags.ch8
./chip8 assets/5-quirks.ch8
./chip8 assets/RPS.ch8
```

## Run Tests

From project root:

```bash
cd test
make test
./test
```

## Clean Build Artifacts

Project binary/objects:

```bash
make clean
```

Test binary/objects:

```bash
cd test
make clean
```

## Notes

- Rendering is terminal-based (`ncurses`), so use a reasonably sized terminal window.
- If build fails with `ncursesw/ncurses.h` not found, install/link ncurses headers for your platform.
