#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/structures/cluster.h"
#include "../headers/structures/point.h"
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/distortion.h"
#include "../headers/files_headers_tests/Tests_distortion.h"


u_int32_t generateRandom(int lower, int upper) {
    
    u_int32_t randomized = (rand() % (upper - lower + 1)) + lower;
    return randomized;
}



void test_distortion1(void) {
    // La fonction distortion ne supporte pas un argument comportant plusieurs centroïdes
    
    k = 2;
    dim = 3;
    nbPts = 10;
    squared_distance_func = squared_euclidean_distance;
    
    int64_t c_coords1[3] = {0, 0, 0};
    int64_t p_coords11[3] = {1, 2, 3};
    int64_t p_coords12[3] = {4, 5, 6};
    int64_t p_coords13[3] = {-2, 3, 1};
    int64_t p_coords14[3] = {4, 1, 0};
    int64_t p_coords15[3] = {-10, 10, 9};
    point_t c1 = {c_coords1};
    point_t p11 = {p_coords11};
    point_t p12 = {p_coords12};
    point_t p13 = {p_coords13};
    point_t p14 = {p_coords14};
    point_t p15 = {p_coords15};
    cluster_t clust1 = {c1, 5};

    int64_t c_coords2[3] = {3, 5, -7};
    int64_t p_coords21[3] = {1, 1, 1};
    int64_t p_coords22[3] = {-2, -3, 5};
    int64_t p_coords23[3] = {0, 7, 0};
    int64_t p_coords24[3] = {9, 1, -2};
    int64_t p_coords25[3] = {11, 14, 7};
    point_t c2 = {c_coords2};
    point_t p21 = {p_coords21};
    point_t p22 = {p_coords22};
    point_t p23 = {p_coords23};
    point_t p24 = {p_coords24};
    point_t p25 = {p_coords25};
    cluster_t clust2 = {c2, 5};
    pts = (point_t*) malloc(sizeof(point_t)*nbPts);
    *(pts+0) = p11;
    *(pts+1) = p12;
    *(pts+2) = p13;
    *(pts+3) = p14;
    *(pts+4) = p15;
    *(pts+5) = p21;
    *(pts+6) = p22;
    *(pts+7) = p23;
    *(pts+8) = p24;
    *(pts+9) = p25;
    cluster_t clist[] = {clust1, clust2};
    u_int32_t clusterParPoint[] = {0,0,0,0,0,1,1,1,1,1};
    u_int64_t distor = distortion(clist, clusterParPoint);
    free(pts);
    CU_ASSERT_EQUAL(distor, 1200);
}


void test_distortion2(void) {
    
    k = 1;
    dim = 2;
    nbPts = 2;
    squared_distance_func = squared_euclidean_distance;


    int64_t c_coords1[2] = {0,0};
    int64_t p_coords11[2] = {2,0};
    int64_t p_coords12[2] = {3,4};

    point_t c1 = {c_coords1};
    point_t p11 = {p_coords11};
    point_t p12 = {p_coords12};
    cluster_t clust1 = {c1, 2};
    cluster_t*  clist = &clust1;
    u_int32_t clusterParPoint[2] = {0,0};
    pts = (point_t*) malloc(sizeof(point_t)*nbPts);
    *(pts+0) = p11;
    *(pts+1) = p12;

    u_int64_t distor = distortion(clist, clusterParPoint);
    //printf("%"PRIu64"\n", distor);
    free(pts);
    CU_ASSERT_EQUAL(distor, 29);
 }
 