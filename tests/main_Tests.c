#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include "../headers/file_headers/main.h"
#include "../headers/structures/cluster.h"
#include "../headers/structures/point.h"
#include "../headers/files_headers_tests/Tests_algo.h"
#include "../headers/files_headers_tests/Tests_binary.h"
#include "../headers/files_headers_tests/Tests_combinations.h"
#include "../headers/files_headers_tests/Tests_createCSV.h"
#include "../headers/files_headers_tests/Tests_distance.h"
#include "../headers/files_headers_tests/Tests_distortion.h"
#include "../headers/files_headers_tests/Tests_globaux.h"
 #include "../headers/file_headers/algorithme.h"
#include "../headers/file_headers/combinations.h"
#include "../headers/file_headers/createCSV.h"
#include "../headers/file_headers/createCSV.h"
#include "../headers/file_headers/distance.h"
#include "../headers/file_headers/distortion.h"
#include "../headers/file_headers/launcher.h"
#include "../headers/file_headers/parseBinary.h"
#include "../headers/file_headers/launcher.h"
#include "../headers/file_headers/main.h" 






int setup(void)  { return 0; }
int teardown(void) { return 0; }

int main(int argc, char const *argv[])
{   

   // Tests_binary

    testBin200pts();




    // Tests_combination
    
    if (CUE_SUCCESS != CU_initialize_registry()) {return CU_get_error();}
    CU_pSuite combination_pSuite = NULL;
    combination_pSuite = CU_add_suite("Distortion tests", setup, teardown);
    if (NULL == combination_pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
         (NULL == CU_add_test(combination_pSuite, "test quotientFactorial 1", test_quotientFactorial1))
         || (NULL == CU_add_test(combination_pSuite, "test factorial 1", test_factorial1))
        ) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();


    

    // Tests_distance 
    if (CUE_SUCCESS != CU_initialize_registry()) {return CU_get_error();}
    CU_pSuite distance_pSuite = NULL;
    distance_pSuite = CU_add_suite("Distance tests", setup, teardown);
    if (NULL == distance_pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(distance_pSuite, "Test 1", test_distance1)) ||
        (NULL == CU_add_test(distance_pSuite, "Test 2", test_distance2)) ||
        (NULL == CU_add_test(distance_pSuite, "Test 3", test_distance3)) ||
        (NULL == CU_add_test(distance_pSuite, "Test 4", test_distance4)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();


    //test_distortion2();
    
    if (CUE_SUCCESS != CU_initialize_registry()) {return CU_get_error();}
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Distortion tests", setup, teardown);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
         (NULL == CU_add_test(pSuite, "Test 2", test_distortion2)) 
         || (NULL == CU_add_test(pSuite, "Test 1", test_distortion1))
        ) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();

    

    //Tests_globaux
    test200pts();

    //Tests_algo

    
    
    k = 3;
    FILE *fpIn = fopen("../InputFiles/test200.bin", "r");
    if (fpIn == NULL) {return 49;}
    int err = parseBin(fpIn);
    if(err!=0){return -1;}
    uint32_t *clusterParPoint = malloc(sizeof(uint32_t)*nbPts);
    cluster_t* clusters = malloc(sizeof(cluster_t)*k);

    for(uint32_t i = 0; i<k ; i++)
    {
        clusters[i].centroid.coords = malloc(sizeof(int64_t)*dim);
        memcpy(clusters[i].centroid.coords, pts[i].coords, sizeof(int64_t)*dim);
    }
    printf("go run l'algo\n");
    algoLoyd(clusters, clusterParPoint);
    printf("algo s'est run!\n");
    for(uint32_t i = 0; i<k; i++)
    {
        fprintCoordPoint(clusters[i].centroid);
    } 

    // Tests_createCSV

    test_createCSV1();
    test_createCSV2();  






    return 0;
}


    

