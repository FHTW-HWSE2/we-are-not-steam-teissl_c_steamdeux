#define _XOPEN_SOURCE
#undef static
#define static
#define USERS_JSON_PATH "test/users_malloc_fail.json"
#include "cJSON.h"

// 💡 malloc vor dem Include umleiten!
#define malloc(x) test_malloc(x)
void* test_malloc(size_t size) {
    (void)size;
    return NULL;  // Simuliere Malloc-Failure
}

#include "unity.h"
#include "../../src/data/data.c"  // Muss NACH dem malloc-define kommen
#include "../../inc/data/data.h"

void setUp(void) {
    FILE *f = fopen(USERS_JSON_PATH, "w");
    fputs("[{\"gamertag\":\"mallocfail\"}]", f);
    fclose(f);
}

void tearDown(void) {}

void test_load_json_from_file_returns_null_when_malloc_fails(void) {
    cJSON *json = load_json_from_file(USERS_JSON_PATH);
    TEST_ASSERT_NULL(json);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_load_json_from_file_returns_null_when_malloc_fails);
    return UNITY_END();
}
