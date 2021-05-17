#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include "../headers/structures/cluster.h"
#include "../headers/structures/point.h"
#include "../headers/file_headers/main.h"
#include "../headers/file_headers/parseBinary.h"
#include "../headers/files_headers_tests/Tests_binary.h"





void test1(){ // teste si l'exemple fonctionne bien

	FILE * file = fopen("../src/example.bin", "r");
	parseBin(file);

}
void test2(){ // erreur doit se produire produit car pas assez de coordonnées
      
    u_int32_t dimLE = 2; // en format Big Endian
	u_int64_t nbLE = 4; // en format Big Endian
	int64_t coordLE = 3;
	u_int32_t dim = htobe32(dimLE);
	u_int64_t nb  = htobe64(nbLE);
    int64_t coord =(int64_t) htobe64((int64_t) coordLE);
    FILE * file = fopen("test2.bin", "w+");
    fwrite(&dim, sizeof(u_int32_t), 1, file);
    fwrite(&nb, sizeof(u_int64_t), 1, file);      // à adapter, pas tout à fait au point
    fwrite(&coord, sizeof(int64_t), 1, file);
    fclose(file);
    FILE * f = fopen("test2.bin", "r");
    parseBin(f);

	fclose(f);
}
void testBin200pts()
{
    FILE * fp = fopen("../InputFiles/test200.bin", "r");
    if(fp == NULL)
    {
        printf("nope, ,nope");
        return;
    }
    parseBin(fp);
}
