#include "unity.h"
#include <string.h>

// --- zu testende Funktion ---
// Hinweis: Diese Funktion ist "pure" und hat keine Abhängigkeiten, daher ist Mocking hier nicht nötig oder sinnvoll.
static int is_valid_ssn_format(const char* ssn) {
    if (!ssn || strlen(ssn) != 11) return 0;
    if (ssn[4] != '-') return 0;
    for (int i = 0; i < 11; i++) {
        if (i == 4) continue;
        if (ssn[i] < '0' || ssn[i] > '9') return 0;
    }
    return 1;
}
// --- Ende ---

void setUp(void) {}
void tearDown(void) {}

void test_is_valid_ssn_format(void) {
    // Gültig
    TEST_ASSERT_TRUE(is_valid_ssn_format("1234-567890"));
    // Zu kurz
    TEST_ASSERT_FALSE(is_valid_ssn_format("123-567890"));
    // Kein Bindestrich
    TEST_ASSERT_FALSE(is_valid_ssn_format("12345678901"));
    // Bindestrich an falscher Stelle
    TEST_ASSERT_FALSE(is_valid_ssn_format("12345-67890"));
    // Enthält Buchstaben
    TEST_ASSERT_FALSE(is_valid_ssn_format("12a4-567890"));
    // NULL
    TEST_ASSERT_FALSE(is_valid_ssn_format(NULL));
    // Leerer String
    TEST_ASSERT_FALSE(is_valid_ssn_format(""));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_is_valid_ssn_format);
    return UNITY_END();
}
