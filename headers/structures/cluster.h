
#ifndef CLUSTER_H
#define CLUSTER_H

#include <stdint.h>
#include "point.h"

typedef struct
{
    point_t centroid;
    uint64_t size_cluster;
} cluster_t;
/*
définit le type cluster_t, structure contenant :
- centroid: l'adresse du point centroïde (point_t) de ce cluster
- size_cluster: le nombre de points (uint64_t) contenus dans ce cluster
*/

#endif //CLUSTER_H
