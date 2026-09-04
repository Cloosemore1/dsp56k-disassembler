#include "instruction_decode.h"

#define PARALLEL_MOVE_INSTRUCTION_START 81

//Instruction decode lookup table
struct opcode_decode_table_row opcode_decode_table[112] = {
    //Instructions which do not allow parallel moves
    //Mnemonic      Mask      Constant
    {JSCC_XXX,      0xFF0000, 0x0F0000},
    {JCC_XXX,       0xFF0000, 0x0E0000},
    {JSR_XXX,       0xFFF000, 0x0D0000},
    {JMP_XXX,       0xFFF000, 0x0C0000},
    {JSCC_EA,       0xFFC0A0, 0x0BC0A0},
    {JSR_EA,        0xFFC0FF, 0x0BC080},
    {JCC_EA,        0xFFC0F0, 0x0AC0A0},
    {JMP_EA,        0xFFC0FF, 0x0AC080},
    {JSSET_PP,      0xFFC0A0, 0x0B80A0},
    {JSCLR_PP,      0xFFC0A0, 0x0B8080},
    {JSET_PP,       0xFFC0A0, 0x0A80A0},
    {JCLR_PP,       0xFFC0A0, 0x0A8080},
    {JSSET_EA,      0xFFC0A0, 0x0B40A0},
    {JSCLR_EA,      0xFFC0A0, 0x0B4080},
    {JSET_EA,       0xFFC0A0, 0x0A40A0},
    {JCLR_EA,       0xFFC0A0, 0x0A4080},
    {JSSET_AA,      0xFFC0A0, 0x0B00A0},
    {JSCLR_AA,      0xFFC0A0, 0x0B0080},
    {JSET_AA,       0xFFC0A0, 0x0A00A0},
    {JCLR_AA,       0xFFC0A0, 0x0A0080},
    {JSSET_S,       0xFFC0E0, 0x0BC020},
    {JSCLR_S,       0xFFC0E0, 0x0BC000},
    {JSET_S,        0xFFC0E0, 0x0AC020},
    {JCLR_S,        0xFFC0E0, 0x0AC000},
    {BTST_PP,       0xFFC0A0, 0x0B8020},
    {BCHG_PP,       0xFFC0A0, 0x0B8000},
    {BSET_PP,       0xFFC0A0, 0x0A8020},
    {BCLR_PP,       0xFFC0A0, 0x0A8000},
    {BTST_EA,       0xFFC0A0, 0x0B4020},
    {BCHG_EA,       0xFFC0A0, 0x0B4000},
    {BSET_EA,       0xFFC0A0, 0x0A4020},
    {BCLR_EA,       0xFFC0A0, 0x0A4000},
    {BTST_AA,       0xFFC0A0, 0x0B0020},
    {BCHG_AA,       0xFFC0A0, 0x0B0000},
    {BSET_AA,       0xFFC0A0, 0x0A0020},
    {BCLR_AA,       0xFFC0A0, 0x0A0000},
    {BTST_D,        0xFFC0E0, 0x0BC060},
    {BCHG_D,        0xFFC0E0, 0x0BC040},
    {BSET_D,        0xFFC0E0, 0x0AC060},
    {BCLR_D,        0xFFC0E0, 0x0AC040},
    {MOVEP_XY,      0xFE4080, 0x084080},
    {MOVEP_P,       0xFE40C0, 0x084040},
    {MOVEP_R,       0xFE40C0, 0x084000},
    {MOVEM_EA,      0xFF40C0, 0x074080},
    {MOVEM_AA,      0xFF40C0, 0x070000},
    {REP_XXX,       0xFF00F0, 0x0600A0},
    {REP_S,         0xFFC0FF, 0x06C020},
    {REP_EA,        0xFFC0BF, 0x064020},
    {REP_AA,        0xFFC0BF, 0x060020},
    {DO_XXX,        0xFF00F0, 0x060080},
    {DO_S,          0xFFC0FF, 0x06C000},
    {DO_EA,         0xFFC0B0, 0x064000},
    {DO_AA,         0xFFC0B0, 0x060000},
    {MOVEC_XX,      0xFF00E0, 0x0500A0},
    {MOVEC_EA,      0xFF40A0, 0x054020},
    {MOVEC_AA,      0xFF40A0, 0x050020},
    {MOVEC_S,       0xFF40E0, 0x0440A0},
    {LUA,           0xFFE0F0, 0x044010},
    {TCC_2,         0xFF0880, 0x030000},
    {TCC_1,         0xFF0F87, 0x020000},
    {NORM,          0xFFF8F7, 0x01D815},
    {DIV,           0xFFFFC7, 0x018040},
    {MAC,           0xFFE0C3, 0x0100C2},
    {MACR,          0xFFE0C3, 0x0100C3},
    {MPY,           0xFFE0C3, 0x0100C0},
    {MPYR,          0xFFE0C3, 0x0100C1},
    {DEBUGCC,       0xFFFFF0, 0x000300},
    {DEBUG,         0xFFFFFF, 0x000200},
    {OR_XX,         0xFF00FC, 0x0000F8},
    {AND_XX,        0xFF00FC, 0x0000B8},
    {ENDDO,         0xFFFFFF, 0x00008C},
    {STOP,          0xFFFFFF, 0x000087},
    {WAIT,          0xFFFFFF, 0x000086},
    {RESET,         0xFFFFFF, 0x000084},
    {RTS,           0xFFFFFF, 0x00000C},
    {DEC,           0xFFFFFE, 0x00000A},
    {INC,           0xFFFFFE, 0x000008},
    {SWI,           0xFFFFFF, 0x000006},
    {ILLEGAL,       0xFFFFFF, 0x000005},
    {RTI,           0xFFFFFF, 0x000004},
    {NOP,           0xFFFFFF, 0x000000},
    //Instructions which allow parallel moves
    //Mnemonic      Mask      Constant
    {MACR_P,        0x000083, 0x000083},
    {MAC_P,         0x000083, 0x000082},
    {MPYR_P,        0x000083, 0x000081},
    {MPY_P,         0x000083, 0x000080},
    {CMPM,          0x000087, 0x000007},
    {AND,           0x0000C7, 0x000046},
    {CMP,           0x000087, 0x000005},
    {SUB,           0x000087, 0x000004},
    {EOR,           0x0000C7, 0x000043},
    {OR,            0x0000C7, 0x000042},
    {TFR,           0x000087, 0x000001},
    {SBC,           0x0000E5, 0x000025},
    {ADC,           0x0000E5, 0x000021},
    {ROL,           0x0000F7, 0x000037},
    {NEG,           0x0000F7, 0x000036},
    {LSL,           0x0000F7, 0x000033},
    {ASL,           0x0000F7, 0x000032},
    {ROR,           0x0000F7, 0x000027},
    {ABS,           0x0000F7, 0x000026},
    {LSR,           0x0000F7, 0x000023},
    {ASR,           0x0000F7, 0x000022},
    {NOT,           0x0000F7, 0x000017},
    {SUBL,          0x0000F7, 0x000016},
    {CLR,           0x0000F7, 0x000013},
    {ADDL,          0x0000F7, 0x000012},
    {RND,           0x0000F7, 0x000011},
    {SUBR,          0x0000F7, 0x000006},
    {TST,           0x0000F7, 0x000003},
    {ADDR,          0x0000F7, 0x000002},
    {MOVE,          0x0000FF, 0x000000},
    {ADD,           0x000087, 0x000000}
};

struct parallel_move_decode_table_row parallel_move_decode_table[15] = {
    //Mnemonic      Mask    Constant
    {NOP_MOVE,      0xFFFF, 0x2000},
    {U_MOVE,        0xFFE0, 0x2040},
    {RY_2_MOVE,     0xFEC0, 0x0880},
    {XR_2_MOVE,     0xFEC0, 0x0800},
    {R_MOVE,        0xFC00, 0x2000},
    {L_EA_MOVE,     0xF440, 0x4040},
    {L_AA_MOVE,     0xF440, 0x4000},
    {RY_1_MOVE,     0xF040, 0x1040},
    {XR_1_MOVE,     0xF040, 0x1000},
    {X_EA_MOVE,     0xC840, 0x4040},
    {X_AA_MOVE,     0xC840, 0x4000},
    {Y_EA_MOVE,     0xC840, 0x4840},
    {Y_AA_MOVE,     0xC840, 0x4800},
    {I_MOVE,        0xE000, 0x2000},
    {XY_MOVE,       0x8000, 0x8000}
};

int opcode_decode(uint32_t instruction) {
    for (int i = 0; i < 112; i++) {
        if ((instruction & opcode_decode_table[i].mask) == opcode_decode_table[i].constant) {
            return opcode_decode_table[i].mnemonic;
        }
    }
    return -1;
}

int parallel_move_opcode_decode(unsigned short parallel_move_instruction) {
    for (int i = 0; i < 15; i++) {
        if ((parallel_move_instruction & parallel_move_decode_table[i].mask) == parallel_move_decode_table[i].constant) {
            return parallel_move_decode_table[i].parallel_move_mnemonic;
        }
    }
    return -1;
}

void parallel_move_decode(unsigned short parallel_move_instruction, uint32_t extension_word, uint32_t *program_counter, int parallel_move_mnemonic, char *parallel_move) {
    unsigned char register_code;
    char source_register[4];
    char target_register[4];

    unsigned char effective_address_mode;
    char effective_address[10];

    unsigned char short_address;

    char register_1[3];
    char register_2[3];

    char move_x[16];
    char move_y[16];

    char s1_register;
    char s2_register;

    switch (parallel_move_mnemonic) {
        case XY_MOVE:
            effective_address_mode = (parallel_move_instruction & 0x0018) >> 3;
            unsigned char effective_address_mode_2 = (parallel_move_instruction & 0x3000) >> 12;
            char effective_address_2[10];
            unsigned char register_num_1 = parallel_move_instruction & 0x0007;
            switch (effective_address_mode) {
                case 0x00:
                    snprintf(effective_address, 10, "(R%d)", register_num_1);
                    break;
                case 0x01:
                    snprintf(effective_address, 10, "(R%d)+N%d", register_num_1, register_num_1);
                    break;
                case 0x02:
                    snprintf(effective_address, 10, "(R%d)-", register_num_1);
                    break;
                case 0x03:
                    snprintf(effective_address, 10, "(R%d)+", register_num_1);
                    break;
            }
            register_code = (parallel_move_instruction & 0x0C00) >> 10;
            switch (register_code) {
                case 0x00:
                    snprintf(register_1, 4, "X0");
                    break;
                case 0x01:
                    snprintf(register_1, 4, "X1");
                    break;
                case 0x02:
                    snprintf(register_1, 4, "A");
                    break;
                case 0x03:
                    snprintf(register_1, 4, "B");
                    break;
            }
            //Determine read or write for X move
            if (parallel_move_instruction & 0x4000) { //write
                snprintf(move_x, 16, "X:%s,%s", effective_address, register_1);
            } else { //read
                snprintf(move_x, 16, "%s,X:%s", register_1, effective_address);
            }
            unsigned char register_num_2 = 0x00;
            if (register_num_1 < 4) {
                register_num_2 = 0x04;
            }
            register_num_2 += (parallel_move_instruction & 0x0060) >> 5;
            switch (effective_address_mode_2) {
                case 0x00:
                    snprintf(effective_address_2, 10, "(R%d)", register_num_2);
                    break;
                case 0x01:
                    snprintf(effective_address_2, 10, "(R%d)+N%d", register_num_2, register_num_2);
                    break;
                case 0x02:
                    snprintf(effective_address_2, 10, "(R%d)-", register_num_2);
                    break;
                case 0x03:
                    snprintf(effective_address_2, 10, "(R%d)+", register_num_2);
                    break;
            }
            register_code = (parallel_move_instruction & 0x0300) >> 8;
            switch (register_code) {
                case 0x00:
                    snprintf(register_2, 4, "Y0");
                    break;
                case 0x01:
                    snprintf(register_2, 4, "Y1");
                    break;
                case 0x02:
                    snprintf(register_2, 4, "A");
                    break;
                case 0x03:
                    snprintf(register_2, 4, "B");
                    break;
            }
            //Determine read or write for X move
            if (parallel_move_instruction & 0x0080) { //write
                snprintf(move_y, 16, "Y:%s,%s", effective_address_2, register_2);
            } else { //read
                snprintf(move_y, 16, "%s,Y:%s", register_2, effective_address_2);
            }
            snprintf(parallel_move, 32, "%s\t%s", move_x, move_y);
            break;
        case X_EA_MOVE:
            //Get effective address
            effective_address_mode = parallel_move_instruction & 0x003F;
            effective_address_decode(extension_word, program_counter, 'X', effective_address_mode, effective_address);
            //Get target register
            register_code = (parallel_move_instruction & 0x3000) >> 9;
            register_code += (parallel_move_instruction & 0x0700) >> 8;
            register_decode(register_code, target_register);
            //Determine read/write
            if (parallel_move_instruction & 0x0080) {
                snprintf(parallel_move, 32, "%s,%s", effective_address, target_register);
            } else {
                snprintf(parallel_move, 32, "%s,%s", target_register, effective_address);
            }
            break;
        case X_AA_MOVE:
            short_address = parallel_move_instruction & 0x003F;
            register_code = (parallel_move_instruction & 0x3000) >> 9;
            register_code += (parallel_move_instruction & 0x0700) >> 8;
            register_decode(register_code, target_register);
            //Determine read/write
            if (parallel_move_instruction & 0x0080) {
                snprintf(parallel_move, 32, "X:$%02X,%s", short_address, target_register);
            } else {
                snprintf(parallel_move, 32, "%s,X:$%02X", target_register, short_address);
            }
            break;
        case Y_EA_MOVE:
            //Get effective address
            effective_address_mode = parallel_move_instruction & 0x003F;
            effective_address_decode(extension_word, program_counter, 'Y', effective_address_mode, effective_address);
            //Get target register
            register_code = (parallel_move_instruction & 0x3000) >> 9;
            register_code += (parallel_move_instruction & 0x0700) >> 8;
            register_decode(register_code, target_register);
            //Determine read/write
            if (parallel_move_instruction & 0x0080) {
                snprintf(parallel_move, 32, "%s,%s", effective_address, target_register);
            } else {
                snprintf(parallel_move, 32, "%s,%s", target_register, effective_address);
            }
            break;
            break;
        case Y_AA_MOVE:
            short_address = parallel_move_instruction & 0x003F;
            register_code = (parallel_move_instruction & 0x3000) >> 9;
            register_code += (parallel_move_instruction & 0x0700) >> 8;
            register_decode(register_code, target_register);
            //Determine read/write
            if (parallel_move_instruction & 0x0080) {
                snprintf(parallel_move, 32, "Y:$%02X,%s", short_address, target_register);
            } else {
                snprintf(parallel_move, 32, "%s,Y:$%02X", target_register, short_address);
            }
            break;
        case L_EA_MOVE:
            effective_address_mode = parallel_move_instruction & 0x003F;
            effective_address_decode(extension_word, program_counter, 'L', effective_address_mode, effective_address);
            unsigned char long_register_code = (parallel_move_instruction & 0x0300) >> 8;
            long_register_code += (parallel_move_instruction & 0x0800) >> 9;
            char long_register[4];
            switch (long_register_code) {
                case 0:
                    snprintf(long_register, 4, "A10");
                    break;
                case 1:
                    snprintf(long_register, 4, "B10");
                    break;
                case 2:
                    snprintf(long_register, 4, "X");
                    break;
                case 3:
                    snprintf(long_register, 4, "Y");
                    break;
                case 4:
                    snprintf(long_register, 4, "A");
                    break;
                case 5:
                    snprintf(long_register, 4, "B");
                    break;
                case 6:
                    snprintf(long_register, 4, "AB");
                    break;
                case 7:
                    snprintf(long_register, 4, "BA");
                    break;
            }
            //Determine read/write
            if (parallel_move_instruction & 0x0080) {
                snprintf(parallel_move, 32, "%s,%s", effective_address, long_register);
            } else {
                snprintf(parallel_move, 32, "%s,%s", long_register, effective_address);
            }
            break;
        case L_AA_MOVE:
            snprintf(parallel_move, 32, "L_AA_MOVE");
            break;
        case I_MOVE:
            //Get target register
            register_code = (parallel_move_instruction & 0x1F00) >> 8;
            register_decode(register_code, target_register);
            unsigned char immediate_data = parallel_move_instruction & 0x00FF;
            //Format string
            snprintf(parallel_move, 32, "#%02X,%s", immediate_data, target_register);
            break;
        case R_MOVE:
            //Get source register
            register_code = (parallel_move_instruction & 0x03E0) >> 5;
            register_decode(register_code, source_register);
            //Get destination register
            register_code = parallel_move_instruction & 0x001F;
            register_decode(register_code, target_register); 
            snprintf(parallel_move, 32, "%s,%s", source_register, target_register);
            break;
        case U_MOVE:
            //Get effective address
            effective_address_mode = parallel_move_instruction & 0x003F;
            effective_address_decode(extension_word, program_counter, ' ', effective_address_mode, effective_address);
            snprintf(parallel_move, 32, "%s", effective_address + 2);
            break;
        case RY_1_MOVE:
            s1_register = 'A';
            char *d1_register = "X0";
            if (parallel_move_instruction & 0x0800) {
                s1_register = 'B';
            }
            if (parallel_move_instruction & 0x0400) {
                d1_register = "X1";
            }
            effective_address_mode = parallel_move_instruction & 0x003F;
            effective_address_decode(extension_word, program_counter, 'Y', effective_address_mode, effective_address);
            unsigned char reg_2_code = (parallel_move_instruction & 0x0300) >> 8;
            switch (reg_2_code) {
                case 0x00:
                    snprintf(register_2, 3, "Y0");
                    break;
                case 0x01:
                    snprintf(register_2, 3, "Y1");
                    break;
                case 0x02:
                    snprintf(register_2, 3, "A");
                    break;
                case 0x03:
                    snprintf(register_2, 3, "B");
                    break;
            }
            //Determine read or write
            if (parallel_move_instruction & 0x0080) { //Write D2
                snprintf(parallel_move, 32, "%c,%s \t%s,%s", s1_register, d1_register, effective_address, register_2);
            } else { //Read S2
                snprintf(parallel_move, 32, "%c,%s \t%s,%s", s1_register, d1_register, register_2, effective_address);
            }
            break;
        case RY_2_MOVE:
            snprintf(parallel_move, 32, "RY_2_MOVE");
            break;
        case XR_1_MOVE:
            s2_register = 'A';
            char *d2_register = "Y0";
            if (parallel_move_instruction & 0x0200) {
                s2_register = 'B';
            }
            if (parallel_move_instruction & 0x0100) {
                d2_register = "Y1";
            }
            effective_address_mode = parallel_move_instruction & 0x003F;
            effective_address_decode(extension_word, program_counter, 'X', effective_address_mode, effective_address);
            unsigned char reg_1_code = (parallel_move_instruction & 0x0C00) >> 10;
            switch (reg_1_code) {
                case 0x00:
                    snprintf(register_1, 3, "X0");
                    break;
                case 0x01:
                    snprintf(register_1, 3, "X1");
                    break;
                case 0x02:
                    snprintf(register_1, 3, "A");
                    break;
                case 0x03:
                    snprintf(register_1, 3, "B");
                    break;
            }
            //Determine read or write
            if (parallel_move_instruction & 0x0080) { //Write D2
                snprintf(parallel_move, 32, "%s,%s \t%c,%s", effective_address, register_1, s2_register, d2_register);
            } else { //Read S2
                snprintf(parallel_move, 32, "%s,%s \t%c,%s", register_1, effective_address, s2_register, d2_register);
            }
            break;
        case XR_2_MOVE:
            snprintf(parallel_move, 32, "XR_2_MOVE");
            break;
        case NOP_MOVE:
            snprintf(parallel_move, 32, " ");
            break;
    }
}

void effective_address_decode(uint32_t extension_word, uint32_t *program_counter, char memory_space, unsigned char effective_address_mode, char *effective_address) {
    if (effective_address_mode == 0x30) { //absolute address
        snprintf(effective_address, 10, "%c:$%04X", memory_space, extension_word & 0x00FFFF);
        (*program_counter)++;
    } else if (effective_address_mode == 0x34) { //immediate data
        snprintf(effective_address, 10, "#%06X", extension_word);
        (*program_counter)++;
    } else {
        unsigned char register_num = effective_address_mode & 0x07;
        unsigned char mode = (effective_address_mode & 0x38) >> 3;
        switch (mode) {
            case 0:
                snprintf(effective_address, 10, "%c:(R%d)-N%d", memory_space, register_num, register_num);
                break;
            case 1:
                snprintf(effective_address, 10, "%c:(R%d)+N%d", memory_space, register_num, register_num);
                break;
            case 2:
                snprintf(effective_address, 10, "%c:(R%d)-", memory_space, register_num);
                break;
            case 3:
                snprintf(effective_address, 10, "%c:(R%d)+", memory_space, register_num);
                break;
            case 4:
                snprintf(effective_address, 10, "%c:(R%d)", memory_space, register_num);
                break;
            case 5:
                snprintf(effective_address, 10, "%c:(R%d+N%d)", memory_space, register_num, register_num);
                break;
            case 7:
                snprintf(effective_address, 10, "%c:-(R%d)", memory_space, register_num);
                break;
        }
    }
}

void condition_decode(unsigned char condition_code, char *condition_mnemonic) {
    switch (condition_code) {
        case 0:
            snprintf(condition_mnemonic, 3, "CC");
            break;
        case 1:
            snprintf(condition_mnemonic, 3, "GE");
            break;
        case 2:
            snprintf(condition_mnemonic, 3, "NE");
            break;
        case 3:
            snprintf(condition_mnemonic, 3, "PL");
            break;
        case 4:
            snprintf(condition_mnemonic, 3, "NN");
            break;
        case 5:
            snprintf(condition_mnemonic, 3, "EC");
            break;
        case 6:
            snprintf(condition_mnemonic, 3, "LC");
            break;
        case 7:
            snprintf(condition_mnemonic, 3, "GT");
            break;
        case 8:
            snprintf(condition_mnemonic, 3, "CS");
            break;
        case 9:
            snprintf(condition_mnemonic, 3, "LT");
            break;
        case 10:
            snprintf(condition_mnemonic, 3, "EQ");
            break;
        case 11:
            snprintf(condition_mnemonic, 3, "MI");
            break;
        case 12:
            snprintf(condition_mnemonic, 3, "NR");
            break;
        case 13:
            snprintf(condition_mnemonic, 3, "ES");
            break;
        case 14:
            snprintf(condition_mnemonic, 3, "LS");
            break;
        case 15:
            snprintf(condition_mnemonic, 3, "LE");
            break;
    }
    return;
}

void register_decode(unsigned char register_code, char *target_register) {
    unsigned char register_number = register_code & 0x07;
    unsigned char register_code_high = (register_code & 0x38) >> 3;
    if (register_code_high == 0x02) {
        snprintf(target_register, 4, "R%d", register_number);
    } else if (register_code_high == 0x03) {
        snprintf(target_register, 4, "N%d", register_number);
    } else if (register_code_high == 0x04) {
        snprintf(target_register, 4, "M%d", register_number);
    } else {
        switch (register_code) {
        case 0x04:
            snprintf(target_register, 4, "X0");
            break;
        case 0x05:
            snprintf(target_register, 4, "X1");
            break;
        case 0x06:
            snprintf(target_register, 4, "Y0");
            break;
        case 0x07:
            snprintf(target_register, 4, "Y1");
            break;
        case 0x08:
            snprintf(target_register, 4, "A0");
            break;
        case 0x09:
            snprintf(target_register, 4, "B0");
            break;
        case 0x0A:
            snprintf(target_register, 4, "A2");
            break;
        case 0x0B:
            snprintf(target_register, 4, "B2");
            break;
        case 0x0C:
            snprintf(target_register, 4, "A1");
            break;
        case 0x0D:
            snprintf(target_register, 4, "B1");
            break;
        case 0x0E:
            snprintf(target_register, 4, "A");
            break;
        case 0x0F:
            snprintf(target_register, 4, "B");
            break;
        case 0x39:
            snprintf(target_register, 4, "SR");
            break;
        case 0x3A:
            snprintf(target_register, 4, "OMR");
            break;
        case 0x3B:
            snprintf(target_register, 4, "SP");
            break;
        case 0x3C:
            snprintf(target_register, 4, "SSH");
            break;
        case 0x3D:
            snprintf(target_register, 4, "SSL");
            break;
        case 0x3E:
            snprintf(target_register, 4, "LA");
            break;
        case 0x3F:
            snprintf(target_register, 4, "LC");
            break;
        }
    }
}

void alu_register_decode(unsigned char alu_register_codes, char *alu_registers) {
    switch (alu_register_codes) {
        case 0x00:
            snprintf(alu_registers, 5, "B,A");
            break;
        case 0x01:
            snprintf(alu_registers, 5, "A,B");
            break;
        case 0x02:
            snprintf(alu_registers, 5, "B,A");
            break;
        case 0x03:
            snprintf(alu_registers, 5, "A,B");
            break;
        case 0x04:
            snprintf(alu_registers, 5, "X,A");
            break;
        case 0x05:
            snprintf(alu_registers, 5, "X,B");
            break;
        case 0x06:
            snprintf(alu_registers, 5, "Y,A");
            break;
        case 0x07:
            snprintf(alu_registers, 5, "Y,B");
            break;
        case 0x08:
            snprintf(alu_registers, 5, "X0,A");
            break;
        case 0x09:
            snprintf(alu_registers, 5, "X0,B");
            break;
        case 0x0A:
            snprintf(alu_registers, 5, "Y0,A");
            break;
        case 0x0B:
            snprintf(alu_registers, 5, "Y0,B");
            break;
        case 0x0C:
            snprintf(alu_registers, 5, "X1,A");
            break;
        case 0x0D:
            snprintf(alu_registers, 5, "X1,B");
            break;
        case 0x0E:
            snprintf(alu_registers, 5, "Y1,A");
            break;
        case 0x0F:
            snprintf(alu_registers, 5, "Y1,B");
            break;
    }
}

void alu_product_register_decode(unsigned char alu_product_register_codes, char *alu_product_registers) {
    switch (alu_product_register_codes) {
                case 0x00:
                    snprintf(alu_product_registers, 6, "X0,X0");
                    break;
                case 0x01:
                    snprintf(alu_product_registers, 6, "Y0,Y0");
                    break;
                case 0x02:
                    snprintf(alu_product_registers, 6, "X1,X0");
                    break;
                case 0x03:
                    snprintf(alu_product_registers, 6, "Y1,Y0");
                    break;
                case 0x04:
                    snprintf(alu_product_registers, 6, "X0,Y1");
                    break;
                case 0x05:
                    snprintf(alu_product_registers, 6, "Y0,X0");
                    break;
                case 0x06:
                    snprintf(alu_product_registers, 6, "X1,Y0");
                    break;
                case 0x07:
                    snprintf(alu_product_registers, 6, "Y1,X1");
                    break;
            }
}

void instruction_decode(uint32_t instruction, uint32_t extension_word, uint32_t *program_counter, int mnemonic, char *assembly_instruction) {
    //Local decoding variables, may not be used by all instructions, but are common to many
    unsigned char condition_code;
    char condition_mnemonic[3];
    unsigned char effective_address_mode;
    char effective_address[10];
    unsigned char source_register_code;
    char source_register[4];
    unsigned char register_code;
    char target_register[4];

    unsigned char alu_register_codes;
    char alu_registers[5];

    char memory_space = 'X';
    unsigned short jmp_address;

    unsigned short parallel_move_code;
    int parallel_move_mnemonic;
    char parallel_move[32];

    unsigned short io_short_address;
    char peripheral_space;

    char alu_sign = '+';
    unsigned char alu_product_register_codes;
    char alu_product_registers[6];

    unsigned short imm_short_data;
    unsigned char bit_number;

    unsigned char absolute_short_address;

    char logical_source_register[3];
    char logical_destination_register;
    unsigned char logical_source_register_code;

    char control_register[4];
    unsigned char control_register_code;

    unsigned short loop_cnt;
    unsigned char register_num;

    if (mnemonic >= PARALLEL_MOVE_INSTRUCTION_START) { //If the opcode corresponds to a parallel move instruction, validate the move opcode
        parallel_move_code = (instruction & 0xFFFF00) >> 8;
        parallel_move_mnemonic = parallel_move_opcode_decode(parallel_move_code);
        if (parallel_move_mnemonic == -1) {
            (*program_counter)++;
            snprintf(assembly_instruction, 64, "data %06X", instruction);
            return;
        }
        parallel_move_decode(parallel_move_code, extension_word, program_counter, parallel_move_mnemonic, parallel_move);
    }

    switch (mnemonic) {
        case -1: //Not a recognized instruction, categorize as data
            snprintf(assembly_instruction, 64, "data %06X", instruction);
            break;
        case NOP:
            snprintf(assembly_instruction, 64, "NOP");
            break;
        case JSCC_XXX:
            condition_code = (instruction & 0x00F000) >> 12;
            jmp_address = instruction & 0x000FFF;
            condition_decode(condition_code, condition_mnemonic);
            snprintf(assembly_instruction, 64, "JS%s \t$%03X", condition_mnemonic, jmp_address);
            break;
        case JCC_XXX:
            condition_code = (instruction & 0x00F000) >> 12;
            jmp_address = instruction & 0x000FFF;
            condition_decode(condition_code, condition_mnemonic);
            snprintf(assembly_instruction, 64, "J%s \t$%03X", condition_mnemonic, jmp_address);
            break;
        case JSR_XXX:
            jmp_address = instruction & 0x000FFF;
            snprintf(assembly_instruction, 64, "JSR \t$%03X", jmp_address);
            break;
        case JMP_XXX:
            jmp_address = instruction & 0x000FFF;
            snprintf(assembly_instruction, 64, "JMP \t$%03X", jmp_address);
            break;
        case JSR_EA:
            memory_space = 'P';
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            snprintf(assembly_instruction, 64, "JSR \t%s", effective_address);
            break;
        case JCC_EA:
            condition_code = instruction & 0x00000F;
            condition_decode(condition_code, condition_mnemonic);
            memory_space = 'P';
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            snprintf(assembly_instruction, 64, "J%s \t%s", condition_mnemonic, effective_address);
            break;
        case JMP_EA:
            memory_space = 'P';
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            snprintf(assembly_instruction, 64, "JMP \t%s", effective_address);
            break;
        case JSET_PP:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            //Get I/O short address and 1 extend
            io_short_address = ((instruction & 0x003F00) >> 8) + 0xFFC0;
            bit_number = instruction & 0x00001F;
            //Get absolute address for jump
            jmp_address = extension_word & 0x00FFFF;
            (*program_counter)++;
            snprintf(assembly_instruction, 64, "JSET \t#%d,%c:<<$%04X,$%04X", bit_number, memory_space, io_short_address, jmp_address);
            break;
        case JCLR_PP:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            //Get I/O short address and 1 extend
            io_short_address = ((instruction & 0x003F00) >> 8) + 0xFFC0;
            bit_number = instruction & 0x00001F;
            //Get absolute address for jump
            jmp_address = extension_word & 0x00FFFF;
            (*program_counter)++;
            snprintf(assembly_instruction, 64, "JCLR \t#%d,%c:<<$%04X,$%04X", bit_number, memory_space, io_short_address, jmp_address);
            break;
        case JSET_EA:
            bit_number = instruction & 0x00001F;
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            jmp_address = extension_word & 0x00FFFF;
            (*program_counter)++;
            snprintf(assembly_instruction, 64, "JSET \t#%d,%s,$%04X", bit_number, effective_address, jmp_address);
            break;
        case JCLR_EA:
            bit_number = instruction & 0x00001F;
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            jmp_address = extension_word & 0x00FFFF;
            (*program_counter)++;
            snprintf(assembly_instruction, 64, "JCLR \t#%d,%s,$%04X", bit_number, effective_address, jmp_address);
            break;
        case JSET_AA:
            bit_number = instruction & 0x00001F;
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            absolute_short_address = (instruction & 0x003F00) >> 8;
            //Get absolute address for jump
            jmp_address = extension_word & 0x00FFFF;
            (*program_counter)++;
            snprintf(assembly_instruction, 64, "JSET \t#%d,%c:$%02X,$%04X", bit_number, memory_space, absolute_short_address, jmp_address);
            break;
        case JCLR_AA:
            bit_number = instruction & 0x00001F;
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            absolute_short_address = (instruction & 0x003F00) >> 8;
            //Get absolute address for jump
            jmp_address = extension_word & 0x00FFFF;
            (*program_counter)++;
            snprintf(assembly_instruction, 64, "JCLR \t#%d,%c:$%02X,$%04X", bit_number, memory_space, absolute_short_address, jmp_address);
            break;
        case JSET_S:
            bit_number = instruction & 0x00001F;
            register_code = (instruction & 0x003F00) >> 8;
            register_decode(register_code, target_register);
            jmp_address = extension_word & 0x00FFFF;
            (*program_counter)++;
            snprintf(assembly_instruction, 64, "JSET \t#%d,%s,$%04X", bit_number, target_register, jmp_address);
            break;
        case JCLR_S:
            bit_number = instruction & 0x00001F;
            register_code = (instruction & 0x003F00) >> 8;
            register_decode(register_code, target_register);
            jmp_address = extension_word & 0x00FFFF;
            (*program_counter)++;
            snprintf(assembly_instruction, 64, "JCLR \t#%d,%s,$%04X", bit_number, target_register, jmp_address);
            break;
        case BTST_PP:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            //Get I/O short address and 1 extend
            io_short_address = ((instruction & 0x003F00) >> 8) + 0xFFC0;
            bit_number = instruction & 0x00001F;
            snprintf(assembly_instruction, 64, "BTST \t#%d,%c:<<$%04X", bit_number, memory_space, io_short_address);
            break;
        case BCHG_PP:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            //Get I/O short address and 1 extend
            io_short_address = ((instruction & 0x003F00) >> 8) + 0xFFC0;
            bit_number = instruction & 0x00001F;
            snprintf(assembly_instruction, 64, "BCHG \t#%d,%c:<<$%04X", bit_number, memory_space, io_short_address);
            break;
        case BSET_PP:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            //Get I/O short address and 1 extend
            io_short_address = ((instruction & 0x003F00) >> 8) + 0xFFC0;
            bit_number = instruction & 0x00001F;
            snprintf(assembly_instruction, 64, "BSET \t#%d,%c:<<$%04X", bit_number, memory_space, io_short_address);
            break;
        case BCLR_PP:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            //Get I/O short address and 1 extend
            io_short_address = ((instruction & 0x003F00) >> 8) + 0xFFC0;
            bit_number = instruction & 0x00001F;
            snprintf(assembly_instruction, 64, "BCLR \t#%d,%c:<<$%04X", bit_number, memory_space, io_short_address);
            break;
        case BTST_EA:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            bit_number = instruction & 0x00001F;
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            snprintf(assembly_instruction, 64, "BTST \t#%d,%s", bit_number, effective_address);
            break;
        case BSET_EA:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            bit_number = instruction & 0x00001F;
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            snprintf(assembly_instruction, 64, "BCLR \t#%d,%s", bit_number, effective_address);
            break;
        case BCLR_EA:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            bit_number = instruction & 0x00001F;
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            snprintf(assembly_instruction, 64, "BCLR \t#%d,%s", bit_number, effective_address);
            break;
        case BSET_AA:
            //Get bit number
            bit_number = instruction & 0x00001F;
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            absolute_short_address = (instruction & 0x003F00) >> 8;
            snprintf(assembly_instruction, 64, "BSET \t#%d,%c:$%02X", bit_number, memory_space, absolute_short_address);
            break;
        case BCLR_AA:
            //Get bit number
            bit_number = instruction & 0x00001F;
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            absolute_short_address = (instruction & 0x003F00) >> 8;
            snprintf(assembly_instruction, 64, "BCLR \t#%d,%c:$%02X", bit_number, memory_space, absolute_short_address);
            break;
        case BSET_D:
            bit_number = instruction & 0x00001F;
            register_code = (instruction & 0x003F00) >> 8;
            register_decode(register_code, target_register);
            snprintf(assembly_instruction, 64, "BSET \t#%d,%s", bit_number, target_register);
            break;
        case BCLR_D:
            bit_number = instruction & 0x00001F;
            register_code = (instruction & 0x003F00) >> 8;
            register_decode(register_code, target_register);
            snprintf(assembly_instruction, 64, "BCLR \t#%d,%s", bit_number, target_register);
            break;
        case MOVEP_XY:
            //Get I/O short address and 1 extend
            io_short_address = (instruction & 0x00003F) + 0xFFC0;

            //Get memory spaces
            memory_space = 'X';
            peripheral_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            if (instruction & 0x010000) {
                peripheral_space = 'Y';
            }

            //Get effective address
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
        
            //Determine read/write
            if (instruction & 0x008000) {
                snprintf(assembly_instruction, 64, "MOVEP \t%s,%c:<<$%04X", effective_address, peripheral_space, io_short_address);
            } else {
                snprintf(assembly_instruction, 64, "MOVEP \t%c:<<$%04X,%s", peripheral_space, io_short_address, effective_address);
            }
            break;
        case MOVEP_P:
            snprintf(assembly_instruction, 64, "MOVEP_P");
            break;
        case MOVEP_R:
            //Get I/O short address and 1 extend
            io_short_address = (instruction & 0x00003F) + 0xFFC0;
            //Get peripheral memory space
            peripheral_space = 'X';
            if (instruction & 0x010000) {
                peripheral_space = 'Y';
            }
            //Get target register
            //Get the source/destination target register
            register_code = (instruction & 0x003F00) >> 8;
            register_decode(register_code, target_register);
            //Determine read/write
            if (instruction & 0x008000) {
                snprintf(assembly_instruction, 64, "MOVEP \t%s,%c:<<$%04X", target_register, peripheral_space, io_short_address);
            } else {
                snprintf(assembly_instruction, 64, "MOVEP \t%c:<<$%04X,%s", peripheral_space, io_short_address, target_register);
            }
            break;
        case MOVEM_EA:
            memory_space = 'P';
            //Get effective address
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            
            //Get the source/destination target register
            register_code = instruction & 0x00003F;
            register_decode(register_code, target_register);

            //Determine read/write
            if (instruction & 0x008000) {
                snprintf(assembly_instruction, 64, "MOVEM \t%s,%s", effective_address, target_register);
            } else {
                snprintf(assembly_instruction, 64, "MOVEM \t%s,%s", target_register, effective_address);
            }
            break;
        case REP_XXX:
            imm_short_data = (instruction & 0x00FF00) >> 8;
            imm_short_data += (instruction & 0x00000F) << 8;
            snprintf(assembly_instruction, 64, "REP \t%03X", imm_short_data);
            break;
        case REP_S:
            register_code = (instruction & 0x003F00) >> 8;
            register_decode(register_code, target_register);
            snprintf(assembly_instruction, 64, "REP \t%s", target_register);
            break;
        case DO_XXX:
            loop_cnt = (instruction & 0x00000F) << 8;
            loop_cnt += (instruction & 0x00FF00) >> 8;
            snprintf(assembly_instruction, 64, "DO \t#%03X,$%04X", loop_cnt, extension_word);
            (*program_counter)++;
            break;
        case DO_S:
            register_code = (instruction & 0x0033F00) >> 8;
            register_decode(register_code, target_register);
            snprintf(assembly_instruction, 64, "DO \t%s,$%04X", target_register, extension_word);
            (*program_counter)++;
            break;
        case DO_EA:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            //Get effective address
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            snprintf(assembly_instruction, 64, "DO \t%s,$%04X", effective_address, extension_word);
            break;
        case DO_AA:
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            absolute_short_address = (instruction & 0x003F00) >> 8;
            snprintf(assembly_instruction, 64, "DO \t%c:$%02X,$%04X", memory_space, absolute_short_address, extension_word);
            (*program_counter)++;
            break;
        case MOVEC_XX:
            register_code = instruction & 0x00003F;
            register_decode(register_code, target_register);
            imm_short_data = (instruction & 0x00FF00) >> 8;
            snprintf(assembly_instruction, 64, "MOVEC \t#%02X,%s", imm_short_data, target_register);
            break;
        case MOVEC_EA:
            register_code = instruction & 0x00003F;
            register_decode(register_code, target_register);
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            //Get effective address
            effective_address_mode = ((instruction & 0x003F00) >> 8);
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            if (instruction & 0x008000) {
                snprintf(assembly_instruction, 64, "MOVEC \t%s,%s", effective_address, target_register);
            } else {
                snprintf(assembly_instruction, 64, "MOVEC \t%s,%s", target_register, effective_address);
            }
            break;
        case MOVEC_AA:
            register_code = instruction & 0x00003F;
            register_decode(register_code, target_register);
            //Get memory space
            memory_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            absolute_short_address = (instruction & 0x003F00) >> 8;
            if (instruction & 0x008000) {
                snprintf(assembly_instruction, 64, "MOVEC \t%c:$%02X,%s", memory_space, absolute_short_address, target_register);
            } else {
                snprintf(assembly_instruction, 64, "MOVEC \t%s,%c:$%02X", target_register, memory_space, absolute_short_address);
            }
            break;
        case MOVEC_S:
            source_register_code = instruction & 0x00003F;
            register_decode(source_register_code, source_register);
            register_code = (instruction & 0x003F00) >> 8;
            register_decode(register_code, target_register);
            if (instruction & 0x008000) {
                snprintf(assembly_instruction, 64, "MOVEC \t%s,%s", target_register, source_register);
            } else {
                snprintf(assembly_instruction, 64, "MOVEC \t%s,%s", source_register, target_register);
            }
            break;
        case LUA:
            //Get effective address
            effective_address_mode = (instruction & 0x001F00) >> 8;
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
            //Get destination register
            register_code = instruction & 0x00001F;
            register_decode(register_code, target_register);
            snprintf(assembly_instruction, 64, "LUA \t%s,%s", effective_address + 2, target_register);
            break;
        case TCC_1:
            condition_code = (instruction & 0x00F000) >> 12;
            condition_decode(condition_code, condition_mnemonic);
            alu_register_codes = (instruction & 0x000078) >> 3;
            alu_register_decode(alu_register_codes, alu_registers);
            snprintf(assembly_instruction, 64, "T%s \t%s", condition_mnemonic, alu_registers);
            break;
        case NORM:
            register_num = (instruction & 0x000700) >> 8;
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "NORM \tR%d,B", register_num);
            } else {
                snprintf(assembly_instruction, 64, "NORM \tR%d,A", register_num);
            }
            break;
        case DIV:
            alu_register_codes = (instruction & 0x000078) >> 3;
            alu_register_decode(alu_register_codes, alu_registers);
            snprintf(assembly_instruction, 64, "DIV \t%s", alu_registers);
            break;
        case OR_XX:
            control_register_code = instruction & 0x000003;
            switch (control_register_code) {
                case 0x00:
                    snprintf(control_register, 4, "MR");
                    break;
                case 0x01:
                    snprintf(control_register, 4, "CCR");
                    break;
                case 0x02:
                    snprintf(control_register, 4, "OMR");
                    break;
            }
            imm_short_data = (instruction & 0x00FF00) >> 8;
            snprintf(assembly_instruction, 64, "OR(I) \t#%02X,%s", imm_short_data, control_register);
            break;
        case AND_XX:
            control_register_code = instruction & 0x000003;
            switch (control_register_code) {
                case 0x00:
                    snprintf(control_register, 4, "MR");
                    break;
                case 0x01:
                    snprintf(control_register, 4, "CCR");
                    break;
                case 0x02:
                    snprintf(control_register, 4, "OMR");
                    break;
            }
            imm_short_data = (instruction & 0x00FF00) >> 8;
            snprintf(assembly_instruction, 64, "AND(I) \t#%02X,%s", imm_short_data, control_register);
            break;
        case ENDDO:
            snprintf(assembly_instruction, 64, "ENDDO");
            break;
        case RTS:
            snprintf(assembly_instruction, 64, "RTS");
            break;
        case RTI:
            snprintf(assembly_instruction, 64, "RTI");
            break;
        case MACR_P:
            //Determine sign
            if (instruction & 0x000004) {
                alu_sign = '-';
            }
            //Get product registers
            alu_product_register_codes = (instruction & 0x000070) >> 4;
            alu_product_register_decode(alu_product_register_codes, alu_product_registers);
            //Get destination register
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "MACR \t%c%s,B \t%s", alu_sign, alu_product_registers, parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "MACR \t%c%s,A \t%s", alu_sign, alu_product_registers, parallel_move);
            }
            break;
        case MAC_P:
            //Determine sign
            if (instruction & 0x000004) {
                alu_sign = '-';
            }
            //Get product registers
            alu_product_register_codes = (instruction & 0x000070) >> 4;
            alu_product_register_decode(alu_product_register_codes, alu_product_registers);
            //Get destination register
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "MAC \t%c%s,B \t%s", alu_sign, alu_product_registers, parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "MAC \t%c%s,A \t%s", alu_sign, alu_product_registers, parallel_move);
            }
            break;
        case MPYR_P:
            //Determine sign
            if (instruction & 0x000004) {
                alu_sign = '-';
            }
            //Get product registers
            alu_product_register_codes = (instruction & 0x000070) >> 4;
            alu_product_register_decode(alu_product_register_codes, alu_product_registers);
            //Get destination register
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "MPYR \t%c%s,B \t%s", alu_sign, alu_product_registers, parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "MPYR \t%c%s,A \t%s", alu_sign, alu_product_registers, parallel_move);
            }
            break;
        case MPY_P:
            //Determine sign
            if (instruction & 0x000004) {
                alu_sign = '-';
            }
            //Get product registers
            alu_product_register_codes = (instruction & 0x000070) >> 4;
            alu_product_register_decode(alu_product_register_codes, alu_product_registers);
            //Get destination register
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "MPY \t%c%s,B \t%s", alu_sign, alu_product_registers, parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "MPY \t%c%s,A \t%s", alu_sign, alu_product_registers, parallel_move);
            }
            break;
        case CMPM:
            alu_register_codes = (instruction & 0x000078) >> 3;
            alu_register_decode(alu_register_codes, alu_registers);
            snprintf(assembly_instruction, 64, "CMPM \t%s \t%s", alu_registers, parallel_move);
            break;
        case AND:
            logical_destination_register = 'A';
            
            //Determine destination register
            if (instruction & 0x000008) {
                logical_destination_register = 'B';
            }
            //Determine source register
            logical_source_register_code = (instruction & 0x000030) >> 4;
            switch (logical_source_register_code) {
                case 0x00:
                    snprintf(logical_source_register, 3, "X0");
                    break;
                case 0x01:
                    snprintf(logical_source_register, 3, "Y0");
                    break;
                case 0x02:
                    snprintf(logical_source_register, 3, "X1");
                    break;
                case 0x03:
                    snprintf(logical_source_register, 3, "Y1");
                    break;
            }
            snprintf(assembly_instruction, 64, "AND \t%s,%c \t%s", logical_source_register, logical_destination_register, parallel_move);
            break;
        case CMP:
            alu_register_codes = (instruction & 0x000078) >> 3;
            alu_register_decode(alu_register_codes, alu_registers);
            snprintf(assembly_instruction, 64, "CMP \t%s \t%s", alu_registers, parallel_move);
            break;
        case SUB:
            alu_register_codes = (instruction & 0x000078) >> 3;
            alu_register_decode(alu_register_codes, alu_registers);
            snprintf(assembly_instruction, 64, "SUB \t%s \t%s", alu_registers, parallel_move);
            break;
        case EOR:
            logical_destination_register = 'A';
            //Determine destination register
            if (instruction & 0x000008) {
                logical_destination_register = 'B';
            }
            //Determine source register
            logical_source_register_code = (instruction & 0x000030) >> 4;
            switch (logical_source_register_code) {
                case 0x00:
                    snprintf(logical_source_register, 3, "X0");
                    break;
                case 0x01:
                    snprintf(logical_source_register, 3, "Y0");
                    break;
                case 0x02:
                    snprintf(logical_source_register, 3, "X1");
                    break;
                case 0x03:
                    snprintf(logical_source_register, 3, "Y1");
                    break;
            }
            snprintf(assembly_instruction, 64, "EOR \t%s,%c \t%s", logical_source_register, logical_destination_register, parallel_move);
            break;
        case OR:
            logical_destination_register = 'A';
            //Determine destination register
            if (instruction & 0x000008) {
                logical_destination_register = 'B';
            }
            //Determine source register
            logical_source_register_code = (instruction & 0x000030) >> 4;
            switch (logical_source_register_code) {
                case 0x00:
                    snprintf(logical_source_register, 3, "X0");
                    break;
                case 0x01:
                    snprintf(logical_source_register, 3, "Y0");
                    break;
                case 0x02:
                    snprintf(logical_source_register, 3, "X1");
                    break;
                case 0x03:
                    snprintf(logical_source_register, 3, "Y1");
                    break;
            }
            snprintf(assembly_instruction, 64, "OR \t%s,%c \t%s", logical_source_register, logical_destination_register, parallel_move);
            break;
        case TFR:
            alu_register_codes = (instruction & 0x000078) >> 3;
            alu_register_decode(alu_register_codes, alu_registers);
            snprintf(assembly_instruction, 64, "TFR \t%s \t%s", alu_registers, parallel_move);
            break;
        case NEG:
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "NEG \tB \t%s", parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "NEG \tA \t%s", parallel_move);
            }
            break;
        case ASL:
           if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "ASL \tB \t%s", parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "ASL \tA \t%s", parallel_move);
            }
            break;
        case ABS:
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "ABS \tB \t%s", parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "ABS \tA \t%s", parallel_move);
            }
            break;
        case SUBL:
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "SUBL \tA,B \t%s", parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "SUBL \tB,A \t%s", parallel_move);
            }
            break;
        case CLR:
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "CLR \tB \t%s", parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "CLR \tA \t%s", parallel_move);
            }
            break;
        case ADD:
            alu_register_codes = (instruction & 0x000078) >> 3;
            alu_register_decode(alu_register_codes, alu_registers);
            snprintf(assembly_instruction, 64, "ADD \t%s \t%s", alu_registers, parallel_move);
            break;
        case ADC:
            alu_register_codes = (instruction & 0x000038) >> 3;
            alu_register_decode(alu_register_codes, alu_registers);
            snprintf(assembly_instruction, 64, "ADC \t%s \t%s", alu_registers, parallel_move);
            break;
        case ASR:
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "ASR \tB \t%s", parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "ASR \tA \t%s", parallel_move);
            }
            break;
        case TST:
            if (instruction & 0x000008) {
                snprintf(assembly_instruction, 64, "TST \tB \t%s", parallel_move);
            } else {
                snprintf(assembly_instruction, 64, "TST \tA \t%s", parallel_move);
            }
            break;
        case MOVE:
            snprintf(assembly_instruction, 64, "MOVE \t%s", parallel_move);
            break;
        default:
            snprintf(assembly_instruction, 64, "%d \t%s", mnemonic, parallel_move);
            break;
    }

    (*program_counter)++;
    return;
}