#include <stdio.h>

#define TAMANHO_PLACA 8
#define TAMANHO_MODELO 20
#define TAMANHO_MARCA 20
#define TAMANHO_CATEGORIA 15
#define TAMANHO_STATUS 16

typedef struct {
  char placa[TAMANHO_PLACA];
  char modelo[TAMANHO_MODELO];
  char marca[TAMANHO_MARCA];
  int ano;
  char categoria[TAMANHO_CATEGORIA];
  int quilometragem;
  char status[TAMANHO_STATUS];
} Veiculo;

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
