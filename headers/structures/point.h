
#ifndef POINT_H
#define POINT_H

#include <stdint.h>

typedef struct
{
    int64_t *coords;
} point_t;
/*
définit le type point_t, structure contenant :
- coords: l'adresse d'une liste de coordonnées (int64_t)
*/

#endif //POINT_H
