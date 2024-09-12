#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include "raylib.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320
#define UPSCALE_FACTOR 10
#define Hz 60

char memory[4096];

int main() {
  // load rom
  FILE* rom;

  rom = fopen("roms/GUESS", "rb");

  if (rom == NULL) {
    fprintf(stderr, "ERROR: ROM could not be loaded");
    return EXIT_FAILURE;
  }

  // allocate space for file
  unsigned short *buffer;
  fseek(rom, 0, SEEK_END);
  int rom_len = ftell(rom);
  fseek(rom, 0, SEEK_SET);
  buffer = (short *)malloc(sizeof(short) * rom_len);

  if (buffer == NULL) {
    fprintf(stderr, "ERROR: Memory Error");
    return EXIT_FAILURE;
  }

  // write to the buffer
  fread(buffer, sizeof(short), rom_len, rom);
  fclose(rom);

  // raylib init stuff
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "c8");
  SetTargetFPS(60);

  // loop through opcodes (loop index as pc)
  for (int pc = 0; pc < rom_len; pc++) {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
    // big to little endianess (FORGOT TO MAKE IT UNSIGNED) 
    unsigned short opcode = bswap_16(buffer[pc]);
    printf("%x\n", opcode);
    if (opcode == 0x00) {
      // nuthin
    }
    else if (opcode == 0xe0) {
      // clear the screen 
    } else if (opcode == 0xee) {
      // return from subroutine
    } else if (opcode >= 0x1000 && opcode <= 0x1FFF) {
      // jump to given address
    } else if (opcode >= 0x2000 && opcode <= 0x2FFF) {
      // call subroutine at given address
    } else if (opcode >= 0x3000 && opcode <= 0x3FFF) {
      // skips next instruction if register VX == NN 
    } else if (opcode >= 0x4000 && opcode <= 0x4FFF) {
      // skips next instruction if register VX != NN 
    } else if (opcode >= 0x5000 && opcode <= 0x5FFF) {
      // skips next instruction if register VX != VY
      // (compairing two registers)
    } else if (opcode >= 0x6000 && opcode <= 0x6FFF) {
      // sets given register to given value (V(X) = NN)
    } else if (opcode >= 0x7000 && opcode <= 0x7FFF) {
      // adds to given register with given value 
      // (VX += NN)
    } else if (opcode >= 0x8000 && opcode <= 0x8FFF) {
      // 0. asigins the value of a register to another
      // 1. logically ors the given registers
      // 2. logically ands the given registers
      // 3. logically xors the given registers
      // 4. adds the two registers
      //    if there is an overflow VF is set to 1
      // 5. same as 4 but subtracting
      // 6. shifts a register to the right by 1 bit
      //    and stores the lsb in VF
      // 7. sets Vx to Vy - Vx 
      //    VF is set to 0 if there is a underflow
      //    and vice versa
      // E. same as 6 but shifted to the left
    } else if (opcode >= 0x9000 && opcode <= 0x9FFF) {
      // checks if two register are NOT the same
    } else if (opcode >= 0xa000 && opcode <= 0xaFFF) {
      // sets I to given address
    } else if (opcode >= 0xb000 && opcode <= 0xbFFF) {
      // sets the pc to the given address + V0
    } else if (opcode >= 0xc000 && opcode <= 0xcFFF) {
      // sets the given register to a random number 
      // (0-255)
    } else if (opcode >= 0xd000 && opcode <= 0xdFFF) {
      // draws a sprite with the two given registers
      // as cordinates and N as the height
      // (width is always 8)
      // xor screen pixel with sprite pixel
    } else if (opcode >= 0xe000 && opcode <= 0xeFFF) {
      // 9E. skips instruction if key is pressed
      // A1. skips instruction if key is NOT pressed
    } else if (opcode >= 0xf000 && opcode <= 0xfFFF) {
    } else {
      // TODO FIX: opcodes padded wit fs how
      fprintf(stderr, "ERROR: Opcode not found: %x\n", opcode);
    }


  }

  // free stuff
  free(buffer);
  CloseWindow();

  return EXIT_SUCCESS;
}
