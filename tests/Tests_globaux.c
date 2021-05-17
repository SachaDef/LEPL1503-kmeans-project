#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<pthread.h>
#include "../headers/structures/cluster.h"
#include "../headers/structures/point.h"
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/parseBinary.h"
#include "../headers/file_headers/launcher.h"
#include "../headers/files_headers_tests/Tests_globaux.h"




uint8_t test200pts()
{

    // initialisation des variables globales
    fp = fopen("outputMate.txt", "w");
    fprintf(fp, "initialization centroids ,distortion ,centroids, clusters\n");       
    uint8_t err = 1;
    //long startTime = clock();
    timeKmeans = 0;
    FILE * fpInput = fopen("../InputFiles/test200.bin", "r");
    if(fpInput == NULL)
    {
        printf("le fichier d'input n'existe pas\n");
        return -1;
    }
    if(fp == NULL)
    {
        printf("erreur fichier output\n");
        return -1;
    }
    k = 2; // nombre de clusters
    p = 30; // nb de points pour faire les combis initiales
    squared_distance_func = squared_euclidean_distance;
    quiet = 0;
    n_threads = 2;
    // parsing du fichier binaire
    err = parseBin(fpInput);
    fclose(fpInput);

    if(err == -1)
    {
        return -1;
    }
    // lancement du programme
    if(err == 0)
    {
        err = launchComputations();
        if(err == -1)
        {
            printf("y a une erreur frere\n");

        }else{
        }
        for(uint64_t i = 0; i<nbPts; i++)
        {
            free(pts[i].coords);
        }
        free(pts);
    }
    pthread_mutex_destroy(&printDebug);

    //long stopTime = clock();
    fclose(fp);
    
    //printf("time taken by kmeans %lf ", (double)timeKmeans/CLOCKS_PER_SEC);
    //printf("total time  %lf ", (double)(stopTime)/CLOCKS_PER_SEC);

    return err;
}
