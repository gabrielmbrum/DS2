#include "veiculo.h"

void imprimeVeiculo (Veiculo v) {
  printf("---------------\n");
  printf("placa: %s\n", v.placa);
  printf("modelo: %s\n", v.modelo);
  printf("marca: %s\n", v.marca);
  printf("ano: %d\n", v.ano);
  printf("categoria: %s\n", v.categoria);
  printf("quilometragem: %d km\n", v.quilometragem);
  printf("status: %s", v.status);
  printf("---------------\n");
}