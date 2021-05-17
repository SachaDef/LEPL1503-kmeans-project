#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../headers/structures/kmComputation.h"
#include "../headers/file_headers/distortion.h"
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/combinations.h"
#include "../headers/file_headers/algorithme.h"
#include "../headers/file_headers/createCSV.h"
#include "../headers/file_headers/launcher.h"
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// variables communes

uint16_t tailleBuffers;
int16_t errorFlag = 0;  // devient -3 si un thread rencontre une erreur
pthread_mutex_t mutex_error;

// variables du buffer d'initialisation

point_t ** bufferInits;
sem_t initEmpty;
sem_t initFull;
pthread_mutex_t mutex_inits;
pthread_t threadCombis;
uint8_t headBufferInit;
uint8_t tailBufferInit;

// variables du buffer des computations

sem_t compEmpty;
sem_t compFull;
km_computation_t ** bufferComputations;
pthread_mutex_t mutex_computations;
pthread_t *threadKmeans;
uint64_t trackerComputations;
uint8_t headBufferComp;
uint8_t tailBufferComp;
pthread_mutex_t mutex_trackerComps;

// variables concernant CSV

pthread_t threadCSV;



int8_t launchComputations()
{
    /* Lance tous les threads du problème producteur-consommateur et 
    les "join"
    */

   int8_t retour = 0;
    // initialisations partie producteur de combinaisons initiales

    tailleBuffers = 2*n_threads;
    sem_init(&initEmpty, 0, tailleBuffers);  
    sem_init(&initFull, 0, 0);               
    pthread_mutex_init(&mutex_inits, NULL);
    pthread_mutex_init(&mutex_error, NULL);
    headBufferInit = 0;
    tailBufferInit = 0;
    bufferInits = malloc(sizeof(point_t*)*tailleBuffers);
    if(bufferInits == NULL)
    {
        fprintf(stderr, "mémoire insuffisante\n."); 
        return -1;
    }
    pthread_create(&threadCombis, NULL, Combinations, NULL); 


    // initialisations partie consommateur de combinaisons initiales et producteur de kmeans

    bufferComputations = malloc(sizeof(km_computation_t*)*tailleBuffers);
    if(bufferComputations == NULL)
    {
        fprintf(stderr, "mémoire insuffisante\n."); 
        free(bufferInits);
        return -1;
    }    
    sem_init(&compEmpty, 0, tailleBuffers);
    sem_init(&compFull, 0, 0);
    pthread_mutex_init(&mutex_computations, NULL);
    pthread_mutex_init(&mutex_trackerComps, NULL);
    threadKmeans = malloc(sizeof(pthread_t)*n_threads);
    if(threadKmeans == NULL){ 

        fprintf(stderr, "mémoire insuffisante\n."); 
        free(bufferInits);
        free(bufferComputations);        
        return -1;
    }
    trackerComputations = 0;

    for(int i = 0; i<n_threads; i++)
    {
        pthread_create(threadKmeans+i, NULL, launch_kmeans, NULL); // rajouter arguments nécessaires 
    }

    // initialisation de partie consommateur de kmeans

    pthread_create(&threadCSV, NULL, launch_csv, NULL);


    // "joinage" des threads

    int8_t * retourDesThreads;

    // "joinage" du thread de combis

    pthread_join(threadCombis, (void **)&retourDesThreads);
    if(retourDesThreads == NULL)
    {
        retour = -1;
    }else if (*retourDesThreads == -1)
    {
        retour = -1;
        free(retourDesThreads);
    }else{
        free(retourDesThreads);
    }

    // "joinage" des threads de calcul de kmeans

    for(uint8_t i = 0; i< n_threads; i++)
    {
        pthread_join(*(threadKmeans+i), (void **)&retourDesThreads);
        if(retourDesThreads == NULL)
        {
            retour = -1;
        }
        else if(*retourDesThreads == -1)
        {
            retour = -1;
            free(retourDesThreads);
        }
        else{
            free(retourDesThreads);
        }
    }

    // "joinage du thread imprimant dans le csv"
    
    pthread_join(threadCSV, (void **)&retourDesThreads);
    if(retourDesThreads == NULL)
    {
        retour = -1;
    }
    else if(*retourDesThreads == -1)
    {
        retour = -1; 
        free(retourDesThreads);

    }
    else{
        free(retourDesThreads);
    }

    // gestion des erreurs

    if(errorFlag == -3)
    {
        cleanBuffers();
    }

    // libération des ressources et retour

    sem_destroy(&initEmpty);  
    sem_destroy(&initFull);    
    sem_destroy(&compEmpty);
    sem_destroy(&compFull);
    pthread_mutex_destroy(&mutex_computations);           
    pthread_mutex_destroy(&mutex_inits);
    pthread_mutex_destroy(&mutex_error);
    pthread_mutex_destroy(&mutex_trackerComps);
    
    free(bufferComputations);
    free(bufferInits);
    free(threadKmeans);
    return retour;
}

void* launch_kmeans(void * arg){

    /* Fonction appelée avec chaque thread de calcul des kmeans,
    Consomme des combinaisons initiales, et produit des calculs de 
    kmeans (kmComputation) dans le deuxième buffer, retourne un 
    pointeur d'int8_t  indiquant si la fonction a rencontré une erreur, */
    
    int8_t* returnValue = malloc(sizeof(int8_t));
    if(returnValue == NULL)
    {
        return NULL;
    }
    *returnValue = 0;  

    while(1)
    {

        // check si toutes les combinaisons ont été traitées

        pthread_mutex_lock(&mutex_trackerComps);
        if(trackerComputations == nbCombis)
        {
            pthread_mutex_unlock(&mutex_trackerComps);
            return returnValue; 
        }
        trackerComputations++;
        pthread_mutex_unlock(&mutex_trackerComps);
        
        // alloue mémoire nécessaire aux calculs et libère toutes les ressources en cas d'erreur

        cluster_t * clusters = malloc(sizeof(cluster_t)*k);
        if(clusters == NULL)
        {
            pthread_mutex_lock(&mutex_error);
            errorFlag = -3;
            pthread_mutex_unlock(&mutex_error);
            fprintf(stderr, "mémoire insuffisante dans le calcul des kmeans\n");
            *returnValue = -1;
            sem_post(&initEmpty); // en cas d'erreur, le signale aux autres threads
            sem_post(&compFull);
            return returnValue;
        }
        uint32_t * clusterParPoint = malloc(sizeof(uint32_t)*nbPts);
        if(clusterParPoint == NULL)
        {
            pthread_mutex_lock(&mutex_error);
            errorFlag = -3;
            pthread_mutex_unlock(&mutex_error);
            fprintf(stderr, "mémoire insuffisante  dans le calcul des kmeans\n");
            free(clusters);
            *returnValue = -1;
            sem_post(&initEmpty);
            sem_post(&compFull);
            return returnValue;          
        }

        km_computation_t * computation = malloc(sizeof(km_computation_t));
        if(computation == NULL)
        {
            pthread_mutex_lock(&mutex_error);
            errorFlag = -3;
            pthread_mutex_unlock(&mutex_error);
            fprintf(stderr, "mémoire insuffisante  dans le calcul des kmeans\n");
            free(clusters);
            free(clusterParPoint);
            *returnValue = -1;
            sem_post(&initEmpty);
            sem_post(&compFull);
            return returnValue;
        }
        for(uint32_t i = 0; i<k; i++)
        {
            clusters[i].centroid.coords = malloc(sizeof(int64_t)*dim);        
            if(clusters[i].centroid.coords == NULL)
            {
                for(uint32_t j = 0; j<i; j++)
                {
                    free(clusters[j].centroid.coords);
                }
                pthread_mutex_lock(&mutex_error);
                errorFlag = -3;
                pthread_mutex_unlock(&mutex_error);
                fprintf(stderr, "mémoire insuffisante  dans le calcul des kmeans\n");
                free(clusters);
                free(clusterParPoint);
                free(computation);
                *returnValue = -1;
                sem_post(&initEmpty);
                sem_post(&compFull);
                return returnValue;
            }
        }
        point_t* inits;

        // attend pour consommer une combinaison initiale

        sem_wait(&initFull);
        pthread_mutex_lock(&mutex_error);
        if(errorFlag == -3) // vérifie qu'il n'a pas été réveillé à cause d'une erreur
        {
            pthread_mutex_unlock(&mutex_error);
            for(uint32_t i = 0; i<k ; i++)
            {
                 free(clusters[i].centroid.coords);                     
             }
            free(clusters);
            free(clusterParPoint);
            free(computation);
            sem_post(&initEmpty);
            sem_post(&compFull);
            *returnValue = - 1;

            return returnValue;
        }
        pthread_mutex_unlock(&mutex_error);

        // consomme une combinaison initiale

        pthread_mutex_lock(&mutex_inits);
        inits = bufferInits[headBufferInit];
        headBufferInit++;
        if(headBufferInit == tailleBuffers)
        {
            headBufferInit = 0;
        }
        pthread_mutex_unlock(&mutex_inits);

        for(uint32_t i = 0; i<k ; i++)
        {
            memcpy(clusters[i].centroid.coords, inits[i].coords, sizeof(int64_t)*dim);     
        }
        sem_post(&initEmpty);

        // exécution de l'algorithme et vérification en cas d'erreur
        
        *returnValue = algoLoyd(clusters, clusterParPoint);
        if(*returnValue == -1)
        {
            fprintf(stderr, "mémoire insuffisante dans calcul des kmeans\n");
            pthread_mutex_lock(&mutex_error);
            errorFlag = -3;
            pthread_mutex_unlock(&mutex_error); // pas de free ici car si algo s'est planté l'erreur est catchée un peu plus en bas
        }
        computation->clusterParPoint = clusterParPoint;
        computation->clusters = clusters;
        computation->inits = inits;

        // attend pour insérer résultat dans buffer de calculs de kmeans
        sem_wait(&compEmpty);

        pthread_mutex_lock(&mutex_error);
        if(errorFlag == -3) // vérifie qu'il n'a pas été réveillé en cas d'erreur
        {
            pthread_mutex_unlock(&mutex_error);
            for(uint32_t i = 0; i<k ; i++)
            {
                free(clusters[i].centroid.coords);
            }
            free(inits);
            free(clusters);
            free(clusterParPoint);
            free(computation);
            sem_post(&initEmpty);
            sem_post(&compFull);
            *returnValue = - 1;
            return returnValue;
        }
        pthread_mutex_unlock(&mutex_error);

        // insère calcul de kmeans dans deuxième buffer

        pthread_mutex_lock(&mutex_computations);
        bufferComputations[tailBufferComp] = computation;
        tailBufferComp++;
        if(tailBufferComp == tailleBuffers)
        {
            tailBufferComp = 0;
        }
        pthread_mutex_unlock(&mutex_computations);
        sem_post(&compFull);

        // fin de boucle, 1 a été combinaisons initiales gérée
    }
    return returnValue;
}
void* launch_csv(void* arg)
{
    /* Fonction appelée avec le thread imprimant les résultats de kmeans
    dans le fichier d'output CSV, consomme des calculs de kmeans du deuxième
    buffer et retourne un pointeur d'int8_t indiquant si la fonction a rencontré une erreur */

    int8_t* returnValue = malloc(sizeof(int8_t));
    if(returnValue == NULL)
    {
        return NULL;
    }
    *returnValue = 0;

    uint64_t trackerCSV = 0;
    while(trackerCSV < nbCombis)
    {
        trackerCSV++;
        uint64_t distortion_comp;
        cluster_t * clusters;
        point_t * inits;
        uint32_t* clusterParPoint;
        sem_wait(&compFull);
        pthread_mutex_lock(&mutex_error);
        if(errorFlag == -3)
        {
            pthread_mutex_unlock(&mutex_error);
            *returnValue = -1;
            return returnValue;
        }
        pthread_mutex_unlock(&mutex_error);

        pthread_mutex_lock(&mutex_computations);
        clusters = bufferComputations[headBufferComp]->clusters;
        inits = bufferComputations[headBufferComp]->inits;
        clusterParPoint = bufferComputations[headBufferComp]->clusterParPoint;
        free(bufferComputations[headBufferComp]);
        headBufferComp++;
        if(headBufferComp == tailleBuffers)
        {
            headBufferComp = 0;
        }
        pthread_mutex_unlock(&mutex_computations);
        sem_post(&compEmpty);
        distortion_comp = distortion(clusters, clusterParPoint);
              
        int8_t retour = create_CSV(inits, clusters, distortion_comp, clusterParPoint);

        freeComputationsByProducts(clusters, inits, clusterParPoint); 

        if(retour == -1)
        {
            fprintf(stderr, "mémoire insuffisante dans l'impression du CSV\n");
            *returnValue = -1;
            pthread_mutex_lock(&mutex_error);
            errorFlag = -3;
            pthread_mutex_unlock(&mutex_error);
            for(uint8_t i = 0; i<n_threads; i++)
            {
                sem_post(&compEmpty);
            }
            return returnValue;
        }
        
    }
    return returnValue;  
}

void freeComputationsByProducts(cluster_t* clusters, point_t* inits, uint32_t* clusterParPoint)
{
    /* Libère les ressources utilisées dans le calcul de kmeans pour 1 combinaison initiale */

    for(uint32_t i = 0; i<k; i++)
    {
        free(clusters[i].centroid.coords); 
    }
    free(clusterParPoint);
    free(clusters);
    free(inits);
}

void * Combinations(void * arg) {

    /* Fonction appelée avec le thread produisant les combinaisons, 
    remplit le premier buffer de combinaisons initiales et retourne un pointeur 
    d'int8_t indiquant si la fonction a rencontré une erreur */

    // initialisaton de variables

    int8_t* retour = malloc(sizeof(int8_t));
    if(retour == NULL)
    {
        return NULL;
    }
    uint32_t limPts = p;
    nbCombis = quotientFactorial(limPts, limPts-k)/ factorial(k);
    point_t * temp = malloc(sizeof(point_t)*k);
    if (temp == NULL)
    {
        fprintf(stderr, "mémoire insuffisante dans la génération des combinaisons initiales\n");
        pthread_mutex_lock(&mutex_error);
        errorFlag = -3;
        pthread_mutex_unlock(&mutex_error);
        for(uint8_t i = 0; i<n_threads; i++) {
            sem_post(&initFull);
        }
        *retour = -1;
        return retour;
    }

    // production des combinaisons
    if(CombinationRecur(temp, 0, limPts-1, 0) == -1)
    {
        free(temp);
        *retour = -1;
        return retour;
    }
    free(temp);
    *retour = 0;
    return retour;

}


int8_t CombinationRecur(point_t * temp, int64_t start, int64_t end,int64_t index) {

    /* génère les combinaisons initiales et les insère dans le premier buffer
    de manière récursive, retourne un int8_t indiquant si une erreur a été rencontrée*/

    if (index == k) {
        sem_wait(&initEmpty);
        pthread_mutex_lock(&mutex_error);
        if(errorFlag == -3)
        {
            pthread_mutex_unlock(&mutex_error);
            free(temp);
            return -1;
        }
        pthread_mutex_unlock(&mutex_error);
        point_t *singleCombination = array_copy(temp, k);
        if (singleCombination == NULL) {
            fprintf(stderr, "mémoire insuffisante dans la génération des combinaisons\n");
            pthread_mutex_lock(&mutex_error);
            errorFlag = -3;
            pthread_mutex_unlock(&mutex_error);
            free(temp);
            for(uint8_t i = 0; i<n_threads; i++)
            {
                sem_post(&initFull);
            }
            return -1;
        }
        pthread_mutex_lock(&mutex_inits);
        bufferInits[tailBufferInit] = singleCombination;

        tailBufferInit ++;
        if (tailBufferInit == 2*n_threads) {
            tailBufferInit = 0;
        }
        pthread_mutex_unlock(&mutex_inits);
        sem_post(&initFull);
        return 0;
    }
    

    for (int64_t i = start; i <= end && end-i+1 >= k-index; i++) {
        *(temp+index) = pts[i];
        CombinationRecur(temp, i+1, end, index+1);
    }
    return 0;
}

void cleanBuffers()
{
    /* fonction appelée en cas d'erreur dans un des threads, 
    libère les ressources des combinaisons initiales et calculs de kmeans
    toujours présent dans le premier ou deuxième buffer*/

    if(headBufferInit<tailBufferInit)
    {
        for(uint8_t i = headBufferInit; i<tailBufferInit; i++)
        {
            free(bufferInits[i]);
        }
    }else{

        for(uint8_t i = headBufferInit; i<tailleBuffers; i++)
        {
            free(bufferInits[i]);
        }
        for(uint8_t i = 0; i< tailBufferInit; i++)
        {
            free(bufferInits);
        }
    }

    if(headBufferComp<tailBufferComp)
    {
        for(uint8_t i = headBufferComp; i<tailBufferComp; i++)
        {
            free(bufferComputations[i]->clusterParPoint);
            free(bufferComputations[i]->clusters);
            free(bufferInits[i]->coords);
            free(bufferInits[i]);
        }
    }else{

        for(uint8_t i = headBufferComp; i<tailleBuffers; i++)
        {
            free(bufferComputations[i]->clusterParPoint);
            free(bufferComputations[i]->clusters);
            free(bufferInits[i]->coords);
            free(bufferInits[i]);
        }
        for(uint8_t i = 0; i< tailBufferComp; i++)
        {
            free(bufferComputations[i]->clusterParPoint);
            free(bufferComputations[i]->clusters);
            free(bufferInits[i]->coords);
            free(bufferInits[i]);
        }
    }

}

