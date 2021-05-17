
#ifndef TESTS_CREATECSV_H
#define TESTS_CREATECSV_H
#include <stdint.h>
#include <stdlib.h>

#include "../structures/point.h"

void printcoords(int64_t* coords);
void printclusterParPoint(uint32_t* clusterParPoint);
int64_t generateRandom64(int64_t lower, int64_t upper);
uint32_t* generateClusterParPoint(void);
void generatepoints(void);
point_t generateArbitraryPoints(void);cluster_t generateCluster(void);
void test_createCSV1(void);
void test_createCSV2(void);

#endif //TESTS_CREATECSV_H