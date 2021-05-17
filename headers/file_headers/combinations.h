
#ifndef COMBINATION_H
#define COMBINATION_H
#include "../structures/cluster.h"


uint64_t factorial(uint64_t n);
uint64_t quotientFactorial(uint64_t, uint64_t); 

point_t * array_copy(point_t * array, int64_t size);
void * Combinations(void * limitPts);
int8_t CombinationRecur(point_t * temp, int64_t start, int64_t end, int64_t index);

#endif //COMBINATION_H
