#include "unity.h"
#include <string.h>

// --- zu testende Funktion ---
static int is_valid_email_format(const char* email) {
    if (!email || strlen(email) == 0) return 0;
    const char* at_pos = strchr(email, '@');
    if (!at_pos || strchr(at_pos, '.') == NULL) return 0;
    return 1;
}
// --- Ende ---

void setUp(void) {}
void tearDown(void) {}

void test_is_valid_email_format(void) {
    // Gültig
    TEST_ASSERT_TRUE(is_valid_email_format("foo@bar.com"));
    TEST_ASSERT_TRUE(is_valid_email_format("a@b.de"));
    // Kein @
    TEST_ASSERT_FALSE(is_valid_email_format("foobar.com"));
    // Kein Punkt nach @
    TEST_ASSERT_FALSE(is_valid_email_format("foo@barcom"));
    // Punkt vor @
    TEST_ASSERT_FALSE(is_valid_email_format("foo.bar@com"));
    // Mehrere @
    TEST_ASSERT_TRUE(is_valid_email_format("foo@bar.baz.com"));
    // NULL
    TEST_ASSERT_FALSE(is_valid_email_format(NULL));
    // Leerer String
    TEST_ASSERT_FALSE(is_valid_email_format(""));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_is_valid_email_format);
    return UNITY_END();
}
