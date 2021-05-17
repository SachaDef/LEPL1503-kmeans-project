
#ifndef DISTORTION_H
#define DISTORTION_H
#include "../structures/cluster.h"
#include "distance.h"

typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;

uint64_t distortion(cluster_t * cluster , uint32_t* clusterParPoint);

#endif // DISTORTION_H