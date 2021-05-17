#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/parseBinary.h"


int8_t parseBin(FILE * file)
{

	/* argument :
	- file : FILE* correspondant au fichier binaire d'entrée

	Parse le fichier binaire d'entrée, initialise les variables globales pts (liste de tous les points),
	dim (dimension de chaque point), et nbPts (nombre total de points)
	Renvoie un int8_t indiquant si une erreur a été rencontrée*/

	uint32_t dimBE; // en format Big Endian
	uint64_t nbBE; // en format Big Endian
	if(fread(&dimBE, sizeof(uint32_t), 1, file) == 0)
	{
		fprintf(stderr, "Pas de dimension de point spécifiée.");
		return -1;
	}; 
	if(fread(&nbBE, sizeof(uint64_t), 1, file) == 0)
	{
		fprintf(stderr, "pas de nombre de points spécifié."); 
		return -1;
	}
	uint32_t dimension = be32toh(dimBE);
	uint64_t nb = be64toh(nbBE);
	
	dim = dimension; // variables globales du programme
	nbPts = nb;      // variables globales du programme

	pts = malloc(sizeof(point_t)*nb);
	if(pts == NULL)
	{
		fprintf(stderr, "mémoire insuffisante."); 
		return -1;
	}
	for(uint64_t i = 0; i<nb; i++)
	{
		int64_t * coord = malloc((sizeof(int64_t)*dim));
		if(coord == NULL)
		{
			for(uint64_t j = 0; j<i; j++)  // on free toute la mémoire déjà allouée pour terminer l'exécution
			{
				free(pts[j].coords);
			}
			free(pts);
			fprintf(stderr, "mémoire insuffisante."); 
			return -1;

		}
		if(fread(coord, sizeof(int64_t), dim, file) == 0) // on free toute la mémoire déjà allouée pour terminer l'exécution
		{
			for(uint64_t j = 0; j<i; j++)
			{
				free(pts[j].coords);
			}
			free(pts);
			fprintf(stderr, "problème lors de lecture des points du fichier d'input."); 
			return -1;
		}
		for(int j = 0; j<dim; j++)
		{
			coord[j] = (int64_t) be64toh((int64_t) coord[j]); //marche sans caster aussi en discuter avec tuteur
		}
		pts[i].coords = coord;
	}
	return 0;
}

void printPts()
{
	if(pts == NULL)
	{
		printf("pas encore de pts parsés\n");
	}
    printf("dimension :");
	printf("%" PRIu32 "\n", dim);
	printf("nombre :");
	printf("%" PRIu64 "\n", nbPts);
	point_t * pt = pts;
	for(int i = 0; i< nbPts; i++)
	{
		int64_t * coords = pt[i].coords;
		for(int j = 0; j< dim; j++)
		{
			printf("%" PRId64 "  ", coords[j]);
		}
		printf("\n");
	}

}
