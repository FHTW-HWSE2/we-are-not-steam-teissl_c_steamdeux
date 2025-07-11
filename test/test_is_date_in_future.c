#include "unity/unity.h"
#include "logic/logic.h"
#include <time.h>

void setUp(void) {}
void tearDown(void) {}

void test_is_date_in_future(void) {
    // Test: future date
    TEST_ASSERT_TRUE(is_date_in_future("31.12.2099"));
    // Test: today (should be true)
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);
    char today[11];
    strftime(today, sizeof(today), "%d.%m.%Y", now_tm);
    TEST_ASSERT_TRUE(is_date_in_future(today));
    // Test: past date
    TEST_ASSERT_FALSE(is_date_in_future("01.01.2000"));
    // Test: invalid date
    TEST_ASSERT_FALSE(is_date_in_future("xx.xx.xxxx"));
    TEST_ASSERT_FALSE(is_date_in_future(NULL));
    TEST_ASSERT_FALSE(is_date_in_future(""));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_is_date_in_future);
    return UNITY_END();
}
