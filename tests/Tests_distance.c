#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../headers/structures/cluster.h"
#include "../headers/structures/point.h"
#include "../headers/file_headers/main.h"
#include "../headers/files_headers_tests/Tests_distance.h"

uint32_t dim = 3;

void test_distance1(void) {
    int64_t coords1[] = {1, 2, 3};
    int64_t coords2[] = {4, 5, 6};
    point_t p1 = {coords1};
    point_t p2 = {coords2};
    CU_ASSERT_EQUAL(squared_euclidean_distance(&p1, &p2), 27);
    CU_ASSERT_EQUAL(squared_manhattan_distance(&p1, &p2), 81);
}

void test_distance2(void) {
    int64_t coords1[] = {0, 0, 0};
    int64_t coords2[] = {0, 0, 0};
    point_t p1 = {coords1};
    point_t p2 = {coords2};
    CU_ASSERT_EQUAL(squared_euclidean_distance(&p1, &p2), 0);
    CU_ASSERT_EQUAL(squared_manhattan_distance(&p1, &p2), 0);
}

void test_distance3(void) {
    int64_t coords1[] = {-1, -2, -20};
    int64_t coords2[] = {1, 2, 20};
    point_t p1 = {coords1};
    point_t p2 = {coords2};
    CU_ASSERT_EQUAL(squared_euclidean_distance(&p1, &p2), 1620);
    CU_ASSERT_EQUAL(squared_manhattan_distance(&p1, &p2), 2116);
}

void test_distance4(void) {
    int64_t coords1[] = {0,0,0};
    int64_t coords2[] = {3,4,3};
    point_t p1 = {coords1};
    point_t p2 = {coords2};
    CU_ASSERT_EQUAL(squared_euclidean_distance(&p1,&p2), 34);
}


