#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct decode_table_row {
    char mnemonic[10]; //Size may change, have to see what the longest mnemonic length is
    uint32_t mask; //Mask to determine which bits need to be evaluated to decode the instruction
    uint32_t constant; //Opcode
} decode_table_row;

uint32_t assemble_word_from_bytes(unsigned char *buffer);

void bootstrap(uint32_t *program_ram, FILE *program_rom);

char* decode(uint32_t instruction);