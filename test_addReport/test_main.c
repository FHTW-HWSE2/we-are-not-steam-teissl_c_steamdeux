#include "unity.h"
#include "Mocklogic.h"

void setUp(void) {}
void tearDown(void) {}
void test_logic_function(void);
void test_data_function(void);


int main(void) {
    UNITY_BEGIN();
    test_logic_function();
    test_data_function();
    //test_real_data_function(); //No RUN_TEST 
    return UNITY_END();
}