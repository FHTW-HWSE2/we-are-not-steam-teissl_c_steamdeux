#include "unity.h"
#include "logic/logic.h"

void setUp(void) {}
void tearDown(void) {}

void test_logic_perform_startup_tasks_success(void) {
    int removed = 0, changed = 0;
    int result = logic_perform_startup_tasks(&removed, &changed);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(42, removed);
    TEST_ASSERT_EQUAL(7, changed);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_logic_perform_startup_tasks_success);
    return UNITY_END();
}
