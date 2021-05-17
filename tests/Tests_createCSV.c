#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/structures/cluster.h"
#include "../headers/structures/point.h"
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/createCSV.h"
#include "../headers/file_headers/distortion.h"
#include "../headers/files_headers_tests/Tests_createCSV.h"


void printcoords(int64_t* coords) {
    for (uint32_t i = 0; i < dim; i++) {
        printf("coordonne %"PRIu32" : %"PRId64"\n", i, coords[i]);
    }
}

void printclusterParPoint(uint32_t* clusterParPoint) {
    for (uint32_t i = 0; i < nbPts; i++) {
        printf("clusterParPoint %"PRIu32" : %"PRIu32"\n", i, clusterParPoint[i]);
    }
}
/*
void printpts(point_t* pts) {
    for (uint32_t i = 0; i < dim; i++) {
        for (int j = ; j < dim; j++) {
            printf("pts %"PRIu32" : %"PRId64"\n", i, *(*(pts+i)+j));
        }
        
    }
}*/


int64_t generateRandom64(int64_t lower, int64_t upper) {
    
    int64_t randomized = (int64_t) (rand() % (upper - lower + 1)) + lower;
    return randomized;
}


uint32_t* generateClusterParPoint(void) {
    uint32_t* clusterParPoint = (uint32_t*) malloc(sizeof(uint32_t)*nbPts);

    for (int i = 0; i < nbPts; i++) {
        *(clusterParPoint+i) = (rand() % (k));
    }
    return clusterParPoint;
}

void generatepoints(void) {
    pts = (point_t*) malloc(sizeof(point_t)*nbPts);
    
    for (int i = 0; i < nbPts; i++) {

        int64_t* coords = (int64_t*) malloc(sizeof(int64_t)*dim);
        for (int j = 0; j < dim; j ++) {
            *(coords+j) = generateRandom64(-10, 10);
        }
        pts[i].coords = coords;        
    }
}

point_t generateArbitraryPoints(void) {
    int64_t* coords = (int64_t*) malloc(sizeof(int64_t)*dim);
    for (int i = 0; i < dim; i++) {
        *(coords+i) = generateRandom64(-10,10);
    }
    point_t point = {coords};
    return point;
}

cluster_t generateCluster(void) {

    
    int64_t* coords = (int64_t*) malloc(sizeof(int64_t)*dim);
    for (int i = 0; i < dim; i++) {
        *(coords+i) = generateRandom64(-10,10);
    }
    point_t currentCluster = {coords};
    cluster_t cluster = {currentCluster, (uint64_t) nbPts/k};
    return cluster;
}


void test_createCSV1(void) {

    fp = fopen("Tests_outputCSV/test_output1.csv", "w");
    if(fp==NULL) {
        printf("could not open file");
        exit(-1);
    }

    dim = 2;
    k = 1;
    nbPts = 2;
    squared_distance_func = squared_euclidean_distance;

    int64_t inits_coords[] = {0,0} ;
    point_t inits = {inits_coords};

    cluster_t cluster = {inits, 2};

    int64_t p_coords11[2] = {2,0};
    int64_t p_coords12[2] = {3,4};
    point_t p11 = {p_coords11};
    point_t p12 = {p_coords12};

    pts = (point_t*) malloc(sizeof(point_t)*nbPts);
    *(pts+0) = p11;
    *(pts+1) = p12;
    uint32_t clusterParPoint[] = {0,0};
    uint64_t distor = distortion(&cluster, clusterParPoint);

    create_CSV(&inits, &cluster, distor, clusterParPoint);

    free(pts);
}

void test_createCSV2(void) {

    fp = fopen("TestFiles/test_output2.csv", "w");
    if(fp==NULL) {
        printf("could not open file");
        exit(-1);
    }

    dim = 3;
    k = 3;
    nbPts = 8;
    squared_distance_func = squared_euclidean_distance;

    pts = (point_t*) malloc(sizeof(point_t)*nbPts);
    
    for (int i = 0; i < nbPts; i++) {

        int64_t* coords = (int64_t*) malloc(sizeof(int64_t)*dim);
        for (int j = 0; j < dim; j ++) {
            *(coords+j) = generateRandom64(-10, 10);
        }
        point_t list = {coords};
        *(pts+i) = list;       
    }



    cluster_t* clist = (cluster_t*) malloc(sizeof(cluster_t)*k);
    for (int i = 0; i < k; i++) {
        *(clist+i) = generateCluster();
    }
    //cluster_t list_clist[] = {clist};

    uint32_t* clusterParPoint = generateClusterParPoint();
    
    printclusterParPoint(clusterParPoint);


    point_t* inits = (point_t*) malloc(sizeof(point_t)*k);
    for (int i = 0; i < k; i++) {
        *(inits+i) = generateArbitraryPoints();
    }
    point_t* list_inits = {inits};

    uint64_t distor = distortion(clist, clusterParPoint);

    create_CSV(list_inits, clist, distor, clusterParPoint);

    free(pts);
    free(clusterParPoint);
    free(clist);
    free(inits);
    
}


