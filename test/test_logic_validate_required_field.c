#include "unity.h"
#include "logic/logic.h"

void setUp(void) {}
void tearDown(void) {}

void test_logic_validate_required_field(void) {
    // Valid: normal string
    TEST_ASSERT_TRUE(logic_validate_required_field("abc"));
    // Invalid: NULL
    TEST_ASSERT_FALSE(logic_validate_required_field(NULL));
    // Invalid: empty string
    TEST_ASSERT_FALSE(logic_validate_required_field(""));
    // Invalid: only spaces
    TEST_ASSERT_FALSE(logic_validate_required_field("   "));
    // Valid: string with spaces and chars
    TEST_ASSERT_TRUE(logic_validate_required_field("  a "));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_validate_required_field);
    return UNITY_END();
}
