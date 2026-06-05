#pragma once

#include <string.h>

#include "dsp56k.h"

//Instruction opcode enum
typedef enum {
    //Non parallel move instructions
    JSCC_XXX,
    JCC_XXX,
    JSR_XXX,
    JMP_XXX,
    JSCC_EA,
    JSR_EA,
    JCC_EA,
    JMP_EA,
    JSSET_PP,
    JSCLR_PP,
    JSET_PP,
    JCLR_PP,
    JSSET_EA,
    JSCLR_EA,
    JSET_EA,
    JCLR_EA,
    JSSET_AA,
    JSCLR_AA,
    JSET_AA,
    JCLR_AA,
    JSSET_S,
    JSCLR_S,
    JSET_S,
    JCLR_S,
    BTST_PP,
    BCHG_PP,
    BSET_PP,
    BCLR_PP,
    BTST_EA,
    BCHG_EA,
    BSET_EA,
    BCLR_EA,
    BTST_AA,
    BCHG_AA,
    BSET_AA,
    BCLR_AA,
    BTST_D,
    BCHG_D,
    BSET_D,
    BCLR_D,
    MOVEP_XY,
    MOVEP_P,
    MOVEP_R,
    MOVEM_EA,
    MOVEM_AA,
    REP_XXX,
    REP_S,
    REP_EA,
    REP_AA,
    DO_XXX,
    DO_S,
    DO_EA,
    DO_AA,
    MOVEC_XX,
    MOVEC_EA,
    MOVEC_AA,
    MOVEC_S,
    LUA,
    TCC_2,
    TCC_1,
    NORM,
    DIV,
    MAC,
    MACR,
    MPY,
    MPYR,
    DEBUGCC,
    DEBUG,
    OR_XX,
    AND_XX,
    ENDDO,
    STOP,
    WAIT,
    RESET,
    RTS,
    DEC,
    INC,
    SWI,
    ILLEGAL,
    RTI,
    NOP
    //Parallel move instructions
} Mnemonic;

typedef struct decode_table_row {
    Mnemonic mnemonic;
    //char mnemonic[10]; //Size may change, have to see what the longest mnemonic length is
    uint32_t mask; //Mask to determine which bits need to be evaluated to decode the instruction
    uint32_t constant; //Opcode
} decode_table_row;

int opcode_decode(uint32_t instruction);

void instruction_decode(uint32_t instruction, uint32_t extension_word, uint32_t *program_counter, int mnemonic, char *assembly_instruction);