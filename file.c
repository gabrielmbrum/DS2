#include "file.h"
#include "btree.h"

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

bool fileexist(char *filename) {
  return !(fopen(filename, "r") == NULL);
}