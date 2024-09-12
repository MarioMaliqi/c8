#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>

char memory[4096];

int main() {
  // intial stuff to do:
  // 1. load rom
  FILE* rom;

  rom = fopen("roms/Cave.ch8", "rb");

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

  // 2. loop through every 2 bytes
  for (int opcode = 0; opcode < rom_len; opcode++) {
    buffer[opcode] = bswap_16(buffer[opcode]);
    printf("%02x\n", buffer[opcode]);
  }

  // 3. print opcodes in right order
  // and get window open with raylib maybe
  
  // 4. free stuff
  free(buffer);

  return EXIT_SUCCESS;
}
