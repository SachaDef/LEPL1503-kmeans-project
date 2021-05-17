#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../headers/structures/cluster.h"
#include "../headers/structures/point.h"
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/combinations.h"
#include "../headers/files_headers_tests/Tests_combinations.h"



void test_quotientFactorial1(void) {
    // Simple test
    u_int64_t start1 = 5;
    u_int64_t stop1 = 2;

    u_int64_t start2 = 0;
    u_int64_t stop2 = 0;

    u_int64_t start3 = 10;
    u_int64_t stop3 = 5;


    CU_ASSERT_EQUAL(quotientFactorial(start1,stop1), 60);
    CU_ASSERT_EQUAL(quotientFactorial(start2, stop2), 1);
    CU_ASSERT_EQUAL(quotientFactorial(start3, stop3), 30240);
}

void test_factorial1(void) {
    u_int64_t n1 = 0;
    u_int64_t n2 = 1;
    u_int64_t n3 = 10;
    u_int64_t n4 = 5;

    CU_ASSERT_EQUAL(factorial(n1), 1);
    CU_ASSERT_EQUAL(factorial(n2), 1);
    CU_ASSERT_EQUAL(factorial(n3), 3628800)
    CU_ASSERT_EQUAL(factorial(n4), 120);
}



