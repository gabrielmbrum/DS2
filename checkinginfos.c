#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vehicle.h"
#include "btree.h"

int main () {
  Vehicle aux;
  FILE *f;
  f = fopen("veiculos.dat", "r");
  if (f == NULL) {
    puts("error opening veiculos.dat file!!!! :<");
    return 0;
  }

  size_t tamanho_registro = sizeof(Vehicle);
  size_t registros_lidos;
  while (!feof(f)) {
    registros_lidos = fread(&aux, tamanho_registro, 1, f);
    printVehicle(aux);
  }

  fclose(f);
}