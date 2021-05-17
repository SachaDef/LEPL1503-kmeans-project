#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/combinations.h"





uint64_t quotientFactorial(uint64_t start, uint64_t stop) { 

    /* arguments :
    - start : début du calcul
    - stop : fin du calcul

    Calcule start*(start-1)* .... (stop+1)
    */

    uint64_t result = 1;
    for(uint64_t i = start; i> stop; i--)
    {
        result*= i;

    }
    return result;
}
uint64_t factorial(uint64_t n)
{
    /* Calcule la factorielle de n */

    uint64_t result = 1;
    for(uint32_t i = n; i>0; i--)
    {
        result*=i;
        
    }
    return result;  
}

point_t * array_copy(point_t * array, int64_t size) {

    /* arguments :
    - array = liste de point
    - size : taille de la liste

    Copie size*éléments de la liste array et retourne la copie */

    point_t * copy = (point_t *) malloc(sizeof(point_t)*size);
    if (copy == NULL) {return NULL;}
    for (int i = 0; i < size; i++) {
        *(copy+i) = *(array+i);
    }
    return copy;
}

