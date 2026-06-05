#include "instruction_decode.h"

//Instruction decode lookup table
struct decode_table_row decode_table[81] = {
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
    {NOP,           0xFFFFFF, 0x000000}
    
    //Instructions which allow parallel moves
    //Mnemonic      Mask      Constant
};

int opcode_decode(uint32_t instruction) {
    for (int i = 0; i < 81; i++) {
        if ((instruction & decode_table[i].mask) == decode_table[i].constant) {
            return decode_table[i].mnemonic;
        }
    }
    return -1;
}

void effective_address_decode(uint32_t extension_word, uint32_t *program_counter, char memory_space, unsigned char effective_address_mode, char *effective_address) {
    if (effective_address_mode == 0x30) { //absolute address
        snprintf(effective_address, 32, "%c:$%04X", memory_space, extension_word & 0x00FFFF);
        (*program_counter)++;
    } else if (effective_address_mode == 0x34) { //immediate data
        snprintf(effective_address, 32, "#%06X", extension_word);
        (*program_counter)++;
    } else {
        unsigned char register_num = effective_address_mode & 0x07;
        unsigned char mode = (effective_address_mode & 0x38) >> 3;
        switch (mode) {
            case 0:
                snprintf(effective_address, 32, "%c:(R%d)-N%d", memory_space, register_num, register_num);
                break;
            case 1:
                snprintf(effective_address, 32, "%c:(R%d)+N%d", memory_space, register_num, register_num);
                break;
            case 2:
                snprintf(effective_address, 32, "%c:(R%d)-", memory_space, register_num);
                break;
            case 3:
                snprintf(effective_address, 32, "%c:(R%d)+", memory_space, register_num);
                break;
            case 4:
                snprintf(effective_address, 32, "%c:(R%d)", memory_space, register_num);
                break;
            case 5:
                snprintf(effective_address, 32, "%c:(R%d+N%d)", memory_space, register_num, register_num);
                break;
            case 7:
                snprintf(effective_address, 32, "%c:-(R%d)", memory_space, register_num);
                break;
        }
    }
}

void instruction_decode(uint32_t instruction, uint32_t extension_word, uint32_t *program_counter, int mnemonic, char *assembly_instruction) {
    
    switch (mnemonic) {
        case -1:
            snprintf(assembly_instruction, 32, "data %06X", instruction);
            break;
        case NOP:
            snprintf(assembly_instruction, 32, "NOP");
            break;
        case JMP_XXX:
            short jmp_address = instruction & 0x000FFF;
            snprintf(assembly_instruction, 32, "JMP $%03X", jmp_address);
            *program_counter = jmp_address - 1;
            break;
        case MOVEP_XY:
            //Get I/O short address and 1 extend
            unsigned short io_short_address = (instruction & 0x00003F) + 0xFFC0;

            //Get memory spaces
            char memory_space = 'X';
            char peripheral_space = 'X';
            if (instruction & 0x000040) {
                memory_space = 'Y';
            }
            if (instruction & 0x010000) {
                peripheral_space = 'Y';
            }

            //Get effective address
            unsigned char effective_address_mode = ((instruction & 0x003F00) >> 8);
            unsigned char effective_address[32];
            effective_address_decode(extension_word, program_counter, memory_space, effective_address_mode, effective_address);
        
            //Determine read/write
            if (instruction & 0x008000) {
                snprintf(assembly_instruction, 32, "MOVEP %s,%c:<<$%04X", effective_address, peripheral_space, io_short_address);
            } else {
                snprintf(assembly_instruction, 32, "MOVEP %c:<<$%04X,%s", peripheral_space, io_short_address, effective_address);
            }
            break;
        case MOVEP_P:
            break;
        case MOVEP_R:
            break;
        default:
            snprintf(assembly_instruction, 20, "");
            break;
    }
    (*program_counter)++;
    return;
}