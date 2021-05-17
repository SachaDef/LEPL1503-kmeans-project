#ifndef PARSE_BINARY_H
#define PARSE_BINARY_H
#include <stdlib.h>
#include <stdio.h>
#include "../structures/point.h"

int8_t parseBin(FILE *);


typedef struct nuage{

	uint64_t nb;
	point_t * pts;
	uint32_t dim;
}nuage_t;


#endif
