
#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include "distance.h"

typedef struct {
    FILE * input_stream;
    FILE * output_stream;
    uint32_t n_threads;
    uint32_t k;
    uint32_t n_first_initialization_points;
    bool quiet;
    squared_distance_func_t squared_distance_func;
} args_t;

uint32_t dim;                                       // Dimension of each points
uint32_t k;                                         // Number of cluster
point_t * pts;                                      // all points
uint64_t nbPts;                                     // number of points
uint64_t nbCombis;                                  // number of combinations
long timeKmeans;                                    
uint8_t n_threads;                                  // number of threads
uint8_t quiet;
FILE*fp;    
uint64_t p;                                         // number of point to consider in combinations
squared_distance_func_t squared_distance_func;
pthread_mutex_t printDebug;

#endif