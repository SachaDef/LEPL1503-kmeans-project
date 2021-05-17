#ifndef MULTITHREADS_H
#define MULTITHREADS_H

#include<inttypes.h>
#include "../structures/cluster.h"

void* launch_kmeans(void* args);
void* launch_csv(void* args);
int8_t launchComputations(void);
void freeComputationsByProducts(cluster_t* clusters, point_t* inits, uint32_t* clusterParPoint);
void cleanBuffers();


#endif //MULTITHREADS_H