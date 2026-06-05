#include "dsp56k.h"

#define ENTRY_POINT 0x4000

//Instruction decode lookup table
struct decode_table_row decode_table[2] = {
    {"JMP xxx", 0xFFF000, 0x0C0000},
    {"NOP", 0xFFFFFF, 0x000000}
};

//DSP has 512 word program ram which can be bootstrapped from external ROM on startup
uint32_t program_ram[512];

//Program RAM address of next instruction to be executed, execution begins at address 0
uint32_t program_counter = 0x0000;

uint32_t assemble_word_from_bytes(unsigned char *buffer) {
    //dsp uses 24-bit words, expanding to 32 bit here for simplicity, MSB will be 0x00
    //external ROM is little endian, dsp is big endian
    uint32_t word = buffer[0];
    word += buffer[1] << 8;
    word += buffer[2] << 16;
    return word;
}

void bootstrap(uint32_t *program_ram, FILE *program_rom) {
    //Bootstrap program on DSP loads 512 consecutive 3 byte words from program ROM into program RAM
    fseek(program_rom, ENTRY_POINT, SEEK_SET);
    unsigned char current_word[3];
    for (int i = 0; i < 512; i++) {
        fread(current_word, 3, 1, program_rom);
        program_ram[i] = assemble_word_from_bytes(current_word);
    }
}

char* decode(uint32_t instruction) {
    for (int i = 0; i < 2; i++) {
        if ((instruction & decode_table[i].mask) == decode_table[i].constant) {
            return decode_table[i].mnemonic;
        }
    }
    return "NIL";
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("No input file specified\n");
        return -1;
    }
    //Parse input filename
    char *filename = argv[1];

    //Attempt to open input file
    FILE *fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        fprintf(stderr, "Cannot open file\n");
        exit(-1);
    }

    //If successful, bootstrap from binary file
    bootstrap(program_ram, fptr);

    //print first 32 words of program RAM
    for (int i = 0; i < 128; i++) {
        printf("$%04X: %s\n", i, decode(program_ram[i]));
    }
    return 0;
}