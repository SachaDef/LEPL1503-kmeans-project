#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include "../headers/structures/cluster.h"
#include "../headers/structures/point.h"
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/parseBinary.h"
#include "../headers/file_headers/algorithme.h"
#include "../headers/files_headers_tests/Tests_algo.h"











void testEasy() // test l'exemple page 5 de l'énoncé
{
    k = 2;
    dim = 2;
    nbPts = 4;
    nbCombis = 1;
    point_t* inits = malloc(sizeof(point_t)*k);
    inits->coords = malloc(sizeof(int64_t)*dim);
    inits[1].coords = malloc(sizeof(int64_t)*dim);

    point_t * points = createPoints(nbPts);
    points[0].coords[0] = 1;
    points[0].coords[1] = 1;
    points[1].coords[0] = 2;
    points[1].coords[1] = 2;
    points[2].coords[1] = 4;
    points[2].coords[0] = 3;
    points[3].coords[0] = 4;
    points[3].coords[1] = 5;
    inits[0].coords[0] = 1;
    inits[0].coords[1] = 1;
    inits[1].coords[0] = 2;
    inits[1].coords[1] = 2;
    pts = points;
    cluster_t*clusters = malloc(sizeof(cluster_t)*k);
    uint32_t* clusterParPoint = malloc(sizeof(uint32_t)*nbPts);
    clusters[0].size_cluster = 0;
    clusters[1].size_cluster = 0;
    algoLoyd(clusters, clusterParPoint);

    for(uint64_t i = 0; i<nbPts; i++)
    {
        printf("le point numéro %" PRIu64 " appartient au cluster numéro %" PRIu32 " \n", i, clusterParPoint[i]);
    }
    freePoints();
    for(uint32_t i = 0; i<k; i++)
    {
        free(clusters[i].centroid.coords);
    }
    free(clusters);
    free(clusterParPoint);
    // pas besoin de free init car l'algo le fait déjà

}

void test200()
{
    printf("non et non!");
    k = 3;
    FILE *fpIn = fopen("../InputFiles/test200.bin", "r");
    int err = parseBin(fpIn);
    if(err!=0) {exit(-3);}
    uint32_t *clusterParPoint = malloc(sizeof(uint32_t)*nbPts);
    point_t* initAlgo = malloc(sizeof(point_t)*k);
    for(uint32_t i = 0; i<k ; i++)
    {
        initAlgo[i].coords = malloc(sizeof(int64_t)*dim);
        memcpy(initAlgo[i].coords, pts[i].coords, sizeof(int64_t)*dim);
    }
    cluster_t* clusters = malloc(sizeof(cluster_t)*k);
    printf("go run l'algo\n");
    algoLoyd(clusters, clusterParPoint);
    printf("algo s'est run!");
    for(uint32_t i = 0; i<k; i++)
    {
        //fprintCoordPoint2(clusters[i].centroid);
        printf("oui\n");
    }
}

void fprintCoordPoint2(point_t pt) // imprime un point ds csv
{
    printf("(%" PRId64 "", pt.coords[0]);
    for(uint32_t j = 1; j<dim; j++)
    {
        printf(" , %" PRId64 "", pt.coords[j]);
    }
    printf( ")");    
}


point_t * createPoints(uint64_t nb)
{
    point_t * pts = malloc(sizeof(point_t)*nb);
    if(pts == NULL)
    {
        printf("erreur création des points\n");
        return NULL;
    }
    for(int64_t i = 0; i<nb; i++)
    {
        pts[i].coords = malloc(sizeof(int64_t)*dim);
    }
    return pts;
}
void freePoints()
{
    for(uint64_t i = 0; i<nbPts; i++)
    {
        free(pts[i].coords);
    }
    free(pts);
}
cluster_t * createCluster(uint64_t nbPts)
{
    cluster_t * clusts = malloc(k*sizeof(cluster_t));
    if(clusts == NULL)
    {
        printf("erreur creation clusters\n");
        return NULL;
    }
    for(uint32_t i = 0; i<k; i++)
    {
        clusts[i].size_cluster = 0;
    }
    return clusts;
}
void freeClusters(cluster_t* clusts)
{
    free(clusts);

}