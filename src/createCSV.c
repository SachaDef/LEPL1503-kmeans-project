#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <pthread.h>
#include "../headers/structures/cluster.h"
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/createCSV.h"


int8_t create_CSV(point_t* inits, cluster_t *clusters, uint64_t distortion, uint32_t *clusterParPoint)
{
    /* arguments :

    - inits : centroïdes initiaux
    - clusters : clusters contenant les centroïdes finaux
    - distortion : distortion correspondant au calcul de kmeans
    - clusterParPoint : tableau de taille nbPts contenant le numéro du cluster auquel appartient chaque point

    Imprime les résultats d'une combinaison dans le fichier d'output CSV et 
    retourne un int8_t indiquant si une erreur a été rencontrée */

    if(quiet)
    {
        print_in_csv(inits, clusters, distortion);
        fprintf(fp, "\n");
    }else{
        
        print_in_csv(inits, clusters, distortion);
        int8_t retour = print_clusts_in_csv(clusters, clusterParPoint);
        fprintf(fp, "\n");

        if(retour == -1)
        {
            return -1;
        }
    }
    return 0;
}

int8_t print_clusts_in_csv(cluster_t* clusters, uint32_t* clusterParPoint)
{

    /* arguments :
    - clusters : clusters contenant les centroïdes finaux
    - clusterParPoint : tableau de taille nbPts contenant le numéro du cluster auquel appartient chaque point
    
    Imprime les clusters et les points qu'ils contiennent dans le fichier d'output CSV et retourne 
    un int8_t indiquant si une erreur a été rencontrée */

    point_t ** points_des_clusters = getPointsParCluster(clusters, clusterParPoint);
    if(points_des_clusters == NULL)
    {
        return -1;
    }
    fprintf(fp, " ,\"[");
    fprintArrayPoints(*points_des_clusters, clusters->size_cluster);
    for(uint32_t i = 1; i<k ; i++)
    {
        fprintf(fp, ", ");
        fprintArrayPoints(points_des_clusters[i], clusters[i].size_cluster);
        
    }
    fprintf(fp, "]\"");
    for(uint32_t i = 0; i<k; i++)
    {
        free(points_des_clusters[i]);
    }
    free(points_des_clusters);
    return 0;
}
point_t ** getPointsParCluster(cluster_t* clusters, uint32_t* clusterParPoint)
{
    /* arguments :
    - clusters : clusters contenant les centroïdes finaux
    - clusterParPoint : tableau de taille nbPts contenant le numéro du cluster auquel appartient chaque point

    Récupère les points appartenant aux différents clusters afin qu'ils soient utilisables par print_clusts_in_csv
    et renvoie un tableau de point dont chaque ligne correspond aux points pour un cluster différent.
    */


    point_t ** points_des_clusters = malloc(sizeof(point_t*)*k);
    if(points_des_clusters == NULL){ return NULL;}
    uint64_t* trackers = calloc(k, sizeof(uint64_t));
    if(trackers == NULL)
    {
        fprintf(stderr, "mémoire insuffisante\n");
        free(points_des_clusters);
        return NULL;
    }
    for(uint32_t i = 0; i<k; i++)
    {
        points_des_clusters[i] = malloc(sizeof(point_t)*clusters[i].size_cluster);
        if(points_des_clusters[i] == NULL)
        {
            for(uint32_t j = 0; j<i ; j++)
            {
                free(points_des_clusters[j]);
            }
            fprintf(stderr, "mémoire insuffisante\n");
            free(points_des_clusters);
            free(trackers);
            return NULL;           
        }
    }
    for(int i = 0; i<nbPts; i++)
    {
        uint32_t numCluster = clusterParPoint[i];
        points_des_clusters[numCluster][trackers[numCluster]] = pts[i];
        trackers[numCluster]++;
    }
    free(trackers);
    return points_des_clusters;

}
void print_in_csv(point_t* inits, cluster_t* clusters, uint64_t distortion){

    /* arguments :

    - inits : centroïdes initiaux
    - clusters : clusters contenant les centroïdes finaux
    - distortion : distortion correspondant au calcul de kmeans 
    
    Imprime les centroïdes initiaux, finaux et la distortion pour une combinaison initiale
    */

    fprintf(fp, "\"");
    fprintArrayPoints(inits, k); //print les centroids initiaux
    fprintf(fp, "\",%" PRIu64 ", ", distortion); 
    fprintCentroids(clusters);
}

void fprintCoordPoint(point_t pt) // imprime un point ds csv
{
    /* argument :
    - pt : point

    Imprime les coordonnée de pt dans le fichier CSV d'output*/
    fprintf(fp, "(%" PRId64 "", pt.coords[0]);
    for(uint32_t j = 1; j<dim; j++)
    {
        fprintf(fp, " , %" PRId64 "", pt.coords[j]);
    }
    fprintf(fp, ")");    
}

void fprintArrayPoints(point_t* arrayPts, uint64_t nb) // imprime plusieurs points ds csv
{
    /* arguments :
    - arrayPts : liste de points
    - nb : nombre de points contenus dans la liste

    Imprime nb points en provenance de arrayPts dans le fichier d'output CSV */

    fprintf(fp, "[");
    fprintCoordPoint(*arrayPts);
    for(uint64_t i = 1; i<nb; i++)
    {
        fprintf(fp, ", ");
        fprintCoordPoint(*(arrayPts+i));
    }
    fprintf(fp, "]");
}

void fprintCentroids(cluster_t* clusters)
{
    /* argument :
    - clusters : clusters contenant les centroïdes finaux

    Imprime les centroïdes finaux dans le fichier d'output CSV */

    fprintf(fp, "\"[");
    fprintCoordPoint(clusters->centroid);
    for(uint32_t i = 1; i<k; i++)
    {
        fprintf(fp, ", ");
        fprintCoordPoint(clusters[i].centroid);
    }
    fprintf(fp, "]\"");   
}

