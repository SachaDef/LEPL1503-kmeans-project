
#ifndef CLUSTER_DISTORTION_H
#define CLUSTER_DISTORTION_H

#include "cluster.h"

typedef struct
{
    cluster_t **cluster_list;
    uint64_t *distortion_value;
} cluster_distortion_t;
/*
définit le type cluster_distortion_t, structure contenant :
- cluster_list: l'addresse vers une liste d'adresses des différents clusters considérés (cluster_t)
- distortion_value: l'addresse vers une liste des valeurs de distortion (uint64_t) des différents clusters considérés
*/

#endif //CLUSTER_DISTORTION_H
