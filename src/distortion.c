#include <math.h>
#include <stdlib.h>
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/distortion.h"
#include "../headers/file_headers/distance.h"



uint64_t distortion(cluster_t * cluster , uint32_t* clusterParPoint)
/*
Pre
-----
@param cluster: list of all clusters
@param clusterParPoint : list of the corresponding number of cluster for some point
@param squared_distance_func: function used to calculate the distance (either manhattan or euclidean distance)
NB: cluster_list_t struct (see ../headers/cluster_list.h) have a int64_t k member
@requirement:

Post
-----
@return: current_sum, an int64_t value equal to the distance between a centroid and all points in this cluster, calculate either with the manhattan 
         or the euclidian distance
*/
{
    
    uint64_t current_sum = 0;
    for(uint64_t i = 0; i<nbPts; i++)
    {
        current_sum += squared_distance_func(pts+i, &(cluster[clusterParPoint[i]].centroid));
    }   
    return current_sum;
}



