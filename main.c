#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vehicle.h"
#include "btree.h"

//the filename it will be determinated by M at "btree.h"
void build_name (char **name) {
  char *aux;
  *name = (char*)malloc(sizeof(char) * 14); //the biggest name would be btree_100.idx (13 chars + \0) = 14 
  aux = malloc(sizeof(char) * 4); //it will receive the numeric order (max it's 100)

  sprintf(aux, "%d", M);
  strcpy(*name, "btree_");
  strcat(*name, aux);
  strcat(*name, ".idx");

  free(aux);
}

//@return true if exist a file with the name passed
bool fileexist(char *filename) {
  return !(fopen(filename, "r") == NULL);
}

int main () {
  FILE *btreeIdx, *datFile;
  char *filename;
  Vehicle a, b;
  size_t tamanho_registro = sizeof(Vehicle);

  build_name(&filename);

  datFile = fopen("veiculos.dat", "rb"); //mode rb is for read binary files
  if (datFile == NULL) {
    printf("failed to open veiculos.dat\n");
    return 0;
  }

  if (fileexist(filename)) {
    // load the root and show interface
  }
  else { 
    // create file from datfile, inserting pkey on btree; using the queue and saving on btree_M.idx
    btreeIdx = fopen(filename, "w+");

    // load root and show interface  
  }   

  return 0;
}
