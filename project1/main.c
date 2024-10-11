#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "veiculo.h"

char *namingfile() {
  int n;
  char *filename, *aux_filename;
  filename = (char*)malloc(sizeof(char) * 50); //13 is the maximum that the name can be 
  aux_filename = (char*)malloc(sizeof(char) * 4);
  
  printf("M?\n");
  scanf("%d", &n);
  
  if (filename == NULL || aux_filename == NULL) {
    perror("not enought memory\n");
    return 0;return
  }

  sprintf(aux_filename, "%d", n);
     
  strcpy(filename, "btree_");
  
  strcat( filename, aux_filename);
  
  strcat(filename, ".idx");

  free(aux_filename);
  return filename;
}

bool fileexist(char *filename) {
  FILE *f;
  f = fopen(filename, "r");
  return !(f == NULL);
}

int main () {
  FILE *btreeIdx, *datFile;
  char *filename;
  Veiculo a, b;
  size_t tamanho_registro = sizeof(Veiculo);

  filename = (char*)malloc(sizeof(char) * 15); //13 is the maximum that the name can be 
  filename = namingfile();

  datFile = fopen("veiculos.dat", "r");
  if (datFile == NULL) {
    perror("failed to open veiculos.dat\n");
    return 0;
  }

  if (fileexist(filename)) {
    // printf("already exist this file\n");
    btreeIdx = fopen(filename, "w+");

    size_t registro_lido = fread(&a, tamanho_registro, 1, datFile);
    imprimeVeiculo(a);
    
    fwrite(a, tamanho_registro, 1, btreeIdx); 
  }
  else { 
    printf("doesnt exist this file\nit will be necessary to read all infos and create a btree with new order\n" );
    
  }   

  return 0;
}
