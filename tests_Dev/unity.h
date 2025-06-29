
#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>

static int test_count = 0;
static int tests_failed = 0;

void UnityBegin(const char* name) {
    printf("Running test suite: %s\n", name);
    test_count = 0;
    tests_failed = 0;
}

void UnityDefaultTestRun(void (*Func)(void), const char* FuncName, int FuncLineNum) {
    test_count++;
    printf("--------------------------------------------------\n");
    printf("Test %d: %s (Line %d)\n", test_count, FuncName, FuncLineNum);
    Func();
}

int UnityEnd(void) {
    printf("--------------------------------------------------\n");
    printf("Test run complete. Total: %d, Failed: %d, Passed: %d\n",
           test_count, tests_failed, test_count - tests_failed);
    return tests_failed;
}

#define TEST_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s:%d: Assertion failed: %s\n", __FILE__, __LINE__, #condition); \
            tests_failed++; \
        } else { \
            printf("PASS: %s:%d\n", __FILE__, __LINE__); \
        } \
    } while (0)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            printf("FAIL: %s:%d: Expected %d but got %d\n", __FILE__, __LINE__, expected, actual); \
            tests_failed++; \
        } else { \
            printf("PASS: %s:%d\n", __FILE__, __LINE__); \
        } \
    } while (0)

#endif
