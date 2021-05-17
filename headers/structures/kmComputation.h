
#ifndef KMCOMP_H
#define KMCOMP_H

#include "cluster.h"

typedef struct
{
    uint32_t *clusterParPoint;
    cluster_t *clusters;
    point_t *inits;
} km_computation_t;
/*
définit le type km_computation_t, structure contenant :
- clusterParPoint: l'adresse vers une liste de valeurs (unint32_t) => la valeur à l'index i représente l'index du cluster auquel appartient pts[i] (pts étant une variable globale)
- clusters: l'addresse vers une liste des différents clusters à considérer (cluster_t)
- inits: l'adresse vers une liste des points centroïdes initiaux (point_t) des différents clusters
*/

#endif //KMCOMP_H
