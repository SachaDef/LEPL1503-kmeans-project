
#ifndef TESTS_ALGO_H
#define TESTS_ALGO_H
#include <stdint.h>
#include <stdlib.h>

#include "../structures/point.h"

void testEasy();
void freePoints();
point_t * createPoints(uint64_t);
cluster_t * createCluster(uint64_t);
void freeClusters(cluster_t*);
void fprintCoordPoint2(point_t pt);
void test200();

#endif //TESTS_ALGO_H
