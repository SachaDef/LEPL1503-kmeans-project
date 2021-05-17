#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <pthread.h>
#include "../headers/file_headers/distance.h"
#include "../headers/structures/cluster.h"
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/algorithme.h"


int8_t algoLoyd(cluster_t *clusters, uint32_t *clusterParPoint)
{
    /* arguments :
    - clusters : les clusters contenant les centroïdes initiaux
    - clusterParPoint : tableau de taille nbPts dont le contenu final sera le numéro du cluster auquel appartient chaque point,
    exemple : pts[i] appartient à clusters[clusterParPoint[i]]
    
    Cette fonction effectue le calcul des kmeans pour une combinaison et retourne un int8_t indiquant si elle a
    rencontré une erreur */
    
    int8_t sameCentroids = 0;
    while(!sameCentroids)
    {
        for(uint32_t i = 0; i<k; i++)
        {
            clusters[i].size_cluster = 0;
        }
        for(uint64_t i = 0; i< nbPts; i++) 
        {
            int64_t Distmin =  squared_manhattan_distance(pts+i, &(clusters->centroid)); 
            uint32_t numCentroid = 0;
            int64_t dist;

            for(u_int32_t j = 1; j<k ; j++) 
            {
                dist = squared_manhattan_distance(pts+i, &(clusters[j].centroid));
                if(dist<Distmin)
                {
                    Distmin = dist;
                    numCentroid = j;
                }
            }
            clusterParPoint[i] = numCentroid;
            clusters[numCentroid].size_cluster++;

        }
        sameCentroids = update_centroids(clusters, clusterParPoint);
    }

    return 0;
}


int8_t update_centroids(cluster_t* clusters, uint32_t * clusterParPoint)
{
    /* arguments :
    - clusters : les clusters contenant les centroïdes courants
    - clusterParPoint : tableau de taille nbPts contenant le numéro du cluster auquel appartient chaque point

    Cette fonction calcule des nouveaux centroïdes et renvoie un int8_t indiquant si la fonction a rencontré (retournera -1)
    une erreur, si les centroïdes recalculés sont les mêmes que les précédents (retournera 1) ou si ceux-ci sont différents 
    (retournera 0).
    */


    int8_t sameCentroids = 1;
    point_t* oldCentroids = malloc(sizeof(point_t)*k);
    if(oldCentroids == NULL){
        for(uint32_t i = 0; i<k; i++)
        {
            free(clusters[i].centroid.coords);
        }
        return -1;
    }

    for(uint32_t i = 0; i<k; i++)
    {
        oldCentroids[i].coords = calloc(dim, sizeof(int64_t));  
        if(oldCentroids[i].coords == NULL){ 
            for(uint32_t j = 0; j<i; j++)
            {
                free(oldCentroids[j].coords);
            }
            for(uint32_t i = 0; i<k; i++)
            {
                free(clusters[i].centroid.coords);
            }
            free(oldCentroids);
            return -1;
        }
        memcpy(oldCentroids[i].coords, clusters[i].centroid.coords, sizeof(int64_t)*dim);
        for(uint32_t j = 0; j<dim; j++)
        {
            clusters[i].centroid.coords[j] = 0; 
        }
    }

    for(uint64_t i = 0; i< nbPts; i++)
    {

        uint32_t indiceCluster = clusterParPoint[i];
        for(uint32_t j = 0; j<dim; j++)
        {
            clusters[indiceCluster].centroid.coords[j] += pts[i].coords[j];
        }
    }
    for(uint32_t i = 0; i<k; i++)
    {

        for(uint32_t j = 0; j<dim; j++)
        {
            clusters[i].centroid.coords[j] /= (int64_t)clusters[i].size_cluster;
            if(clusters[i].centroid.coords[j] != oldCentroids[i].coords[j]) 
            {
                sameCentroids = 0;
            }  
        }  
    }

    for(uint32_t i = 0; i<k; i++)
    {
        free(oldCentroids[i].coords);
    }
    free(oldCentroids);
    return sameCentroids;
}
