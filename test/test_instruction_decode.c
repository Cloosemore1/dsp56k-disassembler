#include "unity/unity.h"
#include "instruction_decode.h"

void setUp() {

}

void tearDown() {

}

void test_opcode_decode_nop(void) {
    int result = opcode_decode(0x000000);
    
    TEST_ASSERT_EQUAL_INT(NOP, result);
}

void test_instruction_decode_nop(void) {
    char result[64];
    uint32_t program_counter = 0;
    instruction_decode(0x000000, 0, &program_counter, NOP, result);

    TEST_ASSERT_EQUAL_STRING("NOP", result);
}

void test_register_decode_r7(void) {
    unsigned char register_code = 0x17;
    char result[4];
    register_decode(register_code, result);

    TEST_ASSERT_EQUAL_STRING("R7", result);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_opcode_decode_nop);
    RUN_TEST(test_instruction_decode_nop);
    RUN_TEST(test_register_decode_r7);

    UNITY_END();

    return 0;
}