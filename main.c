#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include "raylib.h"
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320
#define UPSCALE_FACTOR 10
#define HZ 60
#define VF 15
#define SPRITE_WIDTH 8

char memory[4096];

int main() {
  srand(time(NULL)); // set random seed based on time

  FILE* rom;

  rom = fopen("roms/GUESS", "rb");

  if (rom == NULL) {
    fprintf(stderr, "ERROR: ROM could not be loaded");
    return EXIT_FAILURE;
  }

  // allocate space for file and get rom size
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

  char rs[16];
  unsigned short I;
  unsigned short sound_timer;
  unsigned short delay_timer;

  int subroutine_origin;
  // for comparisons
  char r;
  // betwen two registers
  char fr;
  char sr;

  // for setting a register
  char val; 

  // for getting the last digits of a number
  char last_digit;
  char ltd;

  // loop through opcodes (loop index as pc)
  for (int pc = 0; pc < rom_len;)  {
    BeginDrawing();
    ClearBackground(BLACK);
    // big to little endianess (FORGOT TO MAKE IT UNSIGNED) 
    unsigned short opcode = bswap_16(buffer[pc]);
    printf("%x\n", opcode);
    if (opcode == 0x00) {
      // nuthin
    }
    else if (opcode == 0xe0) {
      ClearBackground(BLACK);

    } else if (opcode == 0xee) {
      pc = subroutine_origin;

    } else if (opcode >= 0x1000 && opcode <= 0x1FFF) {
      opcode = opcode & 0x0FFF;

    } else if (opcode >= 0x2000 && opcode <= 0x2FFF) {
      subroutine_origin = opcode & 0x0FFF;
      pc = subroutine_origin;

    } else if (opcode >= 0x3000 && opcode <= 0x3FFF) {
      r = (opcode & 0x0F00) >> 2;
      cmp_num = opcode & 0x00FF;

      if (rs[r] == cmp_num) {
        pc += 2;
        continue;
      }
      pc++;

    } else if (opcode >= 0x4000 && opcode <= 0x4FFF) {
      r = (opcode & 0x0F00) >> 2;
      cmp_num = opcode & 0x00FF;

      if (rs[r] != cmp_num) {
        pc += 2;
        continue;
      }
      pc++;

    } else if (opcode >= 0x5000 && opcode <= 0x5FFF) {
      // skips next instruction if register VX != VY
      // (compairing two registers)
      fr = rs[(opcode & 0x0F00) >> 2];
      sr = rs[(opcode & 0x00F0) >> 1];

      if (fr == sr) {
        pc += 2;
        continue;
      }
      pc++;

    } else if (opcode >= 0x6000 && opcode <= 0x6FFF) {
      r = (opcode & 0x0F00) >> 2;
      val  = (opcode $ 0x00FF);
      rs[r] = value;

    } else if (opcode >= 0x7000 && opcode <= 0x7FFF) {
      r = (opcode & 0x0F00) >> 2;
      val  = (opcode $ 0x00FF);
      rs[r] += value;

    } else if (opcode >= 0x8000 && opcode <= 0x8FFF) {
      last_digit = opcode & 0x000F;
      fr = (opcode & 0x0F00) >> 2;
      sr = (opcode & 0x00F0) >> 1;

      switch(last_digit) {
        case 0x0:
          rs[fr] = rs[sr];
          break;

        case 0x1:
          rs[fr] |= rs[sr];
          break;

        case 0x2:
          rs[fr] &= rs[sr];
          break;

        case 0x3:
          rs[fr] ^= rs[sr];
          break;

        case 0x4:
          rs[fr] += rs[sr];
          if (rs[fr] + rs[sr] > 255) {
            rs[VR] = 1;
            break;
          } 
          rs[VR] = 0;
          break;

        case 0x5:
          rs[fr] -= rs[sr];
          if (rs[fr] - rs[sr] < 0) {
            rs[VR] = 0;
            break;
          } 
          rs[VR] = 1;
          break;

        case 0x6:
          rs[VF] = rs[fr] & 0x000F;
          rs[fr] >>= 1;
          break;

        case 0x7:
          // TODO: look through all opcodes with VF and
          // implement the neccessary functionality
          rs[fr] = rs[sr] - rs[fr];
          break;

        case 0xe:
          rs[VF] = rs[fr] & 0xF000;
          rs[fr] <<= 1;
          break;
      }

    } else if (opcode >= 0x9000 && opcode <= 0x9FFF) {
      fr = rs[(opcode & 0x0F00) >> 2];
      sr = rs[(opcode & 0x00F0) >> 1];

      if (fr == sr) {
        pc += 2;
        continue;
      }
      pc++;

    } else if (opcode >= 0xa000 && opcode <= 0xaFFF) {
      val = opcode & 0x0FFF;
      I = val;

    } else if (opcode >= 0xb000 && opcode <= 0xbFFF) {
      val = opcode & 0x0FFF;
      pc = rs[0] + val;

    } else if (opcode >= 0xc000 && opcode <= 0xcFFF) {
      r = (opcode & 0x0F00) >> 2;
      rs[r] = (rand() % 256) & (opcode & 0x00FF);

    } else if (opcode >= 0xd000 && opcode <= 0xdFFF) {
      fr = rs[(opcode & 0x0F00) >> 2];
      sr = rs[(opcode & 0x00F0) >> 1];
      sprite_height = (opcode & 0x000F);

      DrawRectangle(
          fr, sr, SPRITE_WIDTH, sprite_height, RAYWHITE
      );
      break;

    } else if (opcode >= 0xe000 && opcode <= 0xeFFF) {
      ltd = opcode & 0x00FF;

      switch (ltd) {
        case 0x9E:
          if (GetKeyPressed()) {
          }
          break;

        case 0xA1:
          if (!GetKeyPressed()) {
          }
          break;
      }

    } else if (opcode >= 0xf000 && opcode <= 0xfFFF) {
      ltd = opcode & 0x00FF;
      r = (opcode & 0x0F00) >> 2;

      switch(ltd) {
        case 0x7:
          rs[r] = delay_timer();
          break;

        case 0xA:
          break;

        case 0x15:
          break;

        case 0x18:
          break;

        case 0x1E:
          break;

        case 0x29:
          break;

        case 0x33:
          break;

        case 0x55:
          break;

        case 0x65:
          break;
      }

    } else {
      fprintf(stderr, "ERROR: Opcode not found: %x\n", opcode);
      return EXIT_FAILURE;
    }

    EndDrawing();
  }

  // free stuff
  free(buffer);
  CloseWindow();

  return EXIT_SUCCESS;
}
