#include "unity/unity.h"
#include "../inc/logic/logic.h"
#include "mocks/mock_time.h"
#include <string.h>

// --- Mock helpers ---
static time_t fixed_time_value;
static struct tm fixed_tm_value;

static time_t fake_time(time_t *t) {
    if (t) *t = fixed_time_value;
    return fixed_time_value;
}

static struct tm *fake_localtime(const time_t *timep) {
    (void)timep;
    return &fixed_tm_value;
}

static time_t fake_mktime(struct tm *tm) {
    // Just return a fixed value for simplicity
    if (tm->tm_year == 2099-1900) return 4102444800; // 31.12.2099
    if (tm->tm_year == 2000-1900) return 946684800;  // 01.01.2000
    if (tm->tm_year == 2025-1900) return 1752192000; // 11.07.2025
    return 0;
}

static double fake_difftime(time_t time1, time_t time0) {
    return (double)(time1 - time0);
}

void setUp(void) {
    // Set up mocks
    mock_time_func = fake_time;
    mock_localtime_func = fake_localtime;
    mock_mktime_func = fake_mktime;
    mock_difftime_func = fake_difftime;
    // Set fixed date: today
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);
    memcpy(&fixed_tm_value, now_tm, sizeof(struct tm));
    fixed_time_value = mktime(&fixed_tm_value);
}

void tearDown(void) {
    // Reset to real functions if needed
}

void test_is_date_in_future(void) {
    // Format today's date as dd.mm.yyyy
    char today_str[32];
    snprintf(today_str, sizeof(today_str), "%02d.%02d.%04d", fixed_tm_value.tm_mday, fixed_tm_value.tm_mon + 1, fixed_tm_value.tm_year + 1900);
    // Test: future date
    TEST_ASSERT_TRUE(is_date_in_future("31.12.2099"));
    // Test: today (should be true)
    TEST_ASSERT_TRUE(is_date_in_future(today_str));
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
