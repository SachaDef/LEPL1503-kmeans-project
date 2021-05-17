#ifndef CREATE_CSV_H
#define CREATE_CSV_H

#include<stdio.h>
#include<string.h>
#include <inttypes.h>
#include "../structures/cluster.h"
#include "main.h"

int8_t create_CSV(point_t* inits, cluster_t *clusters, uint64_t distortion, uint32_t *clusterParPoint);
void print_in_csv(point_t* inits, cluster_t* clusters, uint64_t distortion);
void fprintCoordPoint(point_t coordPt); 
void fprintArrayPoints( point_t* arrayPts, uint64_t nb); 
void fprintCentroids(cluster_t*);
int8_t print_clusts_in_csv(cluster_t* clusters, uint32_t*  clusterParPoint);
point_t ** getPointsParCluster(cluster_t* clusters, uint32_t*  clusterParPoint);


#endif