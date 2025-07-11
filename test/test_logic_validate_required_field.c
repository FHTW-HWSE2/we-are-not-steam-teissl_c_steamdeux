#include "unity.h"
#include <string.h>

// --- Mock für logic_is_only_spaces ---
int mock_is_only_spaces_return = 0;
int logic_is_only_spaces(const char *str) {
    return mock_is_only_spaces_return;
}
// --- Ende Mock ---

int logic_validate_required_field(const char* str) {
    return str && strlen(str) > 0 && !logic_is_only_spaces(str);
}

void setUp(void) {}
void tearDown(void) {}

void test_logic_validate_required_field(void) {
    // Valid: normal string, mock gibt 0 zurück
    mock_is_only_spaces_return = 0;
    TEST_ASSERT_TRUE(logic_validate_required_field("abc"));
    // Invalid: NULL
    TEST_ASSERT_FALSE(logic_validate_required_field(NULL));
    // Invalid: empty string
    TEST_ASSERT_FALSE(logic_validate_required_field(""));
    // Invalid: nur Leerzeichen (mock gibt 1 zurück)
    mock_is_only_spaces_return = 1;
    TEST_ASSERT_FALSE(logic_validate_required_field("   "));
    // Valid: string mit spaces und chars (mock gibt 0 zurück)
    mock_is_only_spaces_return = 0;
    TEST_ASSERT_TRUE(logic_validate_required_field("  a "));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_validate_required_field);
    return UNITY_END();
}
