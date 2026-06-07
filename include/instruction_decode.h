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
    NOP,
    //Parallel move instructions
    MACR_P,
    MAC_P,
    MPYR_P,
    MPY_P,
    CMPM,
    AND,
    CMP,
    SUB,
    EOR,
    OR,
    TFR,
    ADD,
    SBC,
    ADC,
    ROL,
    NEG,
    LSL,
    ASL,
    ROR,
    ABS,
    LSR,
    ASR,
    NOT,
    SUBL,
    CLR,
    ADDL,
    RND,
    SUBR,
    TST,
    ADDR,
    MOVE
} Mnemonic;

typedef enum {
    XY_MOVE,
    X_EA_MOVE,
    X_AA_MOVE,
    Y_EA_MOVE,
    Y_AA_MOVE,
    L_EA_MOVE,
    L_AA_MOVE,
    I_MOVE,
    R_MOVE,
    U_MOVE,
    NOP_MOVE,
    RY_1_MOVE,
    RY_2_MOVE,
    XR_1_MOVE,
    XR_2_MOVE
} Parallel_Move;

typedef struct opcode_decode_table_row {
    Mnemonic mnemonic;
    uint32_t mask; //Mask to determine which bits need to be evaluated to decode the instruction
    uint32_t constant; //Instruction Opcode
} opcode_decode_table_row;

typedef struct parallel_move_decode_table_row {
    Parallel_Move parallel_move_mnemonic;
    unsigned short mask; //Mask to determine which bits need to be evaluated to decode the parallel move
    unsigned short constant; //Parallel Move Opcode
} parallel_move_decode_table_row;

int opcode_decode(uint32_t instruction);

void instruction_decode(uint32_t instruction, uint32_t extension_word, uint32_t *program_counter, int mnemonic, char *assembly_instruction);

void register_decode(unsigned char register_code, char *target_register);