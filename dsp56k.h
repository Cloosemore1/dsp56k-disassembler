#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint32_t assemble_word_from_bytes(unsigned char *buffer);

void bootstrap(uint32_t *program_ram, FILE *program_rom);