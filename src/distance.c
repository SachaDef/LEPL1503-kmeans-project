#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/distance.h"

uint64_t squared_manhattan_distance(const point_t * p1, const point_t* p2)
/*
Pre
-----
@param p1: pointer to a point_t struct
@param p2: pointer to a point_t struct
NB: point_t struct (see ../headers/point.h) have a int64_t coords[10] member and a int64_t * coords_ptr member
@requirement: p1 and p2 have same dimension, i.e. their coords members hold the same number of values

Post
-----
@return: int64_t value equal to the squared manhattan distance between the two point_t struct, which is defined by
         Σ i:0->n-1( abs( x_i - y_i ) )², with x_i and y_i the i-th coordinates of the x and y points respectively and n the dimension
         of those 2 points
*/
{
    uint64_t sum = 0;
    uint32_t iteration;
    for (iteration = 0; iteration < dim; iteration ++) {
        sum += (uint64_t) abs((int) (p1->coords[iteration] - p2->coords[iteration]));    // converting coordinates into int for abs
    }                                                                                   // abs function, then converting back to
    return (uint64_t) sum*sum;                                            // int64_t
}


uint64_t squared_euclidean_distance(const point_t * p1, const point_t* p2)
    /*
Pre
-----
@param p1: pointer to a point_t struct
@param p2: pointer to a point_t struct
NB: point_t struct (see ../headers/point.h) have a int64_t coords[10] member and a int64_t * coords_ptr member
@requirement: p1 and p2 have same dimension, i.e. their coords members hold the same number of values

Post
-----
@return: int64_t value equal to the squared euclidian distance between the two point_t struct, which is defined by
         Σ i:0->n-1( (x_i - y_i)² ), with x_i and y_i the i-th coordinates of the x and y points respectively and n the dimension
         of those 2 points
*/
{
    uint64_t sum = 0;
    uint32_t iteration;
    for (iteration = 0; iteration < dim; iteration ++) {
        sum += (uint64_t) (p1->coords[iteration] - p2->coords[iteration])*(p1->coords[iteration] - p2->coords[iteration]);   // converting coordinates into double
    }                                                                                          // for pow function, then converting
    return sum;                                                                                // back to int64_t
}

/*void main() // petit test pour voir si ça marche toujours
{
    dim = 2;
    int64_t * p1 = malloc(sizeof(int64_t)*2);
    int64_t * p2 = malloc(sizeof(int64_t)*2);
    p1[0] = -1;
    p1[1] = -2;
    p2[0] = -3;
    p2[1] = 5;
    printf("---- %" PRId64 " \n--", squared_euclidean_distance(p1, p2));
   
}*/
